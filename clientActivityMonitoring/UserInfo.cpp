// UserInfo.cpp
#include "UserInfo.h"

std::wstring UserInfo::GetCurrentUserName() {
    wchar_t username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);
    return std::wstring(username);
}
