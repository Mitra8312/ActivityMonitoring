#include "DataSender.h"

void DataSender::SendDataToServer(const std::wstring& data, const std::vector<BYTE>& screenshotBuffer) {
    HINTERNET hSession = InternetOpen(L"EmployeeMonitorClient", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hSession) {
        HINTERNET hConnect = InternetConnect(hSession, L"localhost", 3000, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            HINTERNET hRequest = HttpOpenRequest(hConnect, L"POST", L"/api/monitor", NULL, NULL, NULL, 0, 0);
            if (hRequest) {
                const wchar_t* headers = L"Content-Type: application/json";
                std::string utf8Data(data.begin(), data.end());
                if (HttpSendRequest(hRequest, headers, static_cast<DWORD>(wcslen(headers)), (LPVOID)utf8Data.c_str(), static_cast<DWORD>(utf8Data.length()))) {
                    std::wcout << L"Data sent successfully\n";
                }
                else {
                    std::wcout << L"Failed to send data\n";
                }
                InternetCloseHandle(hRequest);
            }

            std::wstring username = UserInfo::GetCurrentUserName();
            std::wstring timestamp = TimeUtils::FetchCurrentTime();
            std::wstring screenshotPath = L"/api/screenshot?user=" + username + L"&timestamp=" + timestamp;

            HINTERNET hScreenshotRequest = HttpOpenRequest(hConnect, L"POST", screenshotPath.c_str(), NULL, NULL, NULL, 0, 0);
            if (hScreenshotRequest) {
                const wchar_t* screenshotHeaders = L"Content-Type: application/octet-stream";
                if (HttpSendRequest(hScreenshotRequest, screenshotHeaders, static_cast<DWORD>(wcslen(screenshotHeaders)), (LPVOID)screenshotBuffer.data(), static_cast<DWORD>(screenshotBuffer.size()))) {
                    std::wcout << L"Screenshot sent successfully to: " << screenshotPath << L"\n";
                }
                else {
                    std::wcout << L"Failed to send screenshot\n";
                }
                InternetCloseHandle(hScreenshotRequest);
            }

            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hSession);
    }
}
