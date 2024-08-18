#pragma once
#include <string>
#include <Windows.h>
#include <Lmcons.h>

class UserInfo {
public:
    static std::wstring GetCurrentUserName();
};
