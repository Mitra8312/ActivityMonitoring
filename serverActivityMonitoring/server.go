package main

import (
	"encoding/base64"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"sync"
	"time"
)

// Структура данных для мониторинга
type MonitorData struct {
	User        string   `json:"user"`
	IP          string   `json:"ip"`
	LastActive  string   `json:"lastActive"`
	Screenshots []string `json:"screenshots"`
}

var (
	clientData   = make(map[string]MonitorData)
	clientDataMu sync.RWMutex
)

func handleMonitorData(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
		return
	}

	var data MonitorData
	err := json.NewDecoder(r.Body).Decode(&data)
	if err != nil {
		http.Error(w, "Invalid request payload", http.StatusBadRequest)
		return
	}

	clientDataMu.Lock()
	clientData[data.User] = data
	clientDataMu.Unlock()

	fmt.Printf("Received monitor data: %+v\n", data)

	w.WriteHeader(http.StatusOK)
}

func handleScreenshot(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
		return
	}

	user := r.URL.Query().Get("user")
	if user == "" {
		http.Error(w, "User parameter missing", http.StatusBadRequest)
		return
	}

	filename := fmt.Sprintf("ScreenShots/screenshot_%d_%s.jpg", time.Now().Unix(), user)
	file, err := os.Create(filename)
	if err != nil {
		http.Error(w, "Failed to save screenshot", http.StatusInternalServerError)
		return
	}
	defer file.Close()

	_, err = io.Copy(file, r.Body)
	if err != nil {
		http.Error(w, "Failed to save screenshot", http.StatusInternalServerError)
		return
	}

	fileData, err := os.ReadFile(filename)
	if err != nil {
		http.Error(w, "Failed to read screenshot file", http.StatusInternalServerError)
		return
	}

	encoded := base64.StdEncoding.EncodeToString(fileData)

	clientDataMu.Lock()
	if data, ok := clientData[user]; ok {
		data.Screenshots = append(data.Screenshots, encoded)
		clientData[user] = data
	}
	clientDataMu.Unlock()

	fmt.Printf("Screenshot saved and encoded\n")

	w.WriteHeader(http.StatusOK)
}

func handleClients(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		http.Error(w, "Invalid request method", http.StatusMethodNotAllowed)
		return
	}

	clientDataMu.RLock()
	defer clientDataMu.RUnlock()

	data := make([]MonitorData, 0, len(clientData))
	for _, client := range clientData {
		data = append(data, client)
	}

	response, err := json.Marshal(data)
	if err != nil {
		http.Error(w, "Failed to marshal data", http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.Write(response)
}

// Middleware для обработки CORS
func enableCors(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type")

		if r.Method == http.MethodOptions {
			w.WriteHeader(http.StatusOK)
			return
		}

		next.ServeHTTP(w, r)
	})
}

func main() {

	err := os.MkdirAll("ScreenShots", os.ModePerm)
	if err != nil {
		fmt.Printf("Failed to create screenshots directory: %s\n", err)
		return
	}

	mux := http.NewServeMux()
	mux.HandleFunc("/api/monitor", handleMonitorData)
	mux.HandleFunc("/api/screenshot", handleScreenshot)
	mux.HandleFunc("/api/clients", handleClients)

	fs := http.FileServer(http.Dir("ScreenShots"))
	mux.Handle("/screenshots/", http.StripPrefix("/screenshots/", fs))

	corsHandler := enableCors(mux)

	serverAddr := "localhost:3000"
	fmt.Printf("Server started at %s\n", serverAddr)
	err = http.ListenAndServe(serverAddr, corsHandler)
	if err != nil {
		fmt.Printf("Server failed: %s\n", err)
	}
}
