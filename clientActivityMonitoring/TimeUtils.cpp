#include "TimeUtils.h"
#include <windows.h>
#include <string>

std::wstring TimeUtils::FetchCurrentTime() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t buffer[256];
    swprintf(buffer, 256, L"%04d-%02d-%02dT%02d:%02d:%02d",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);
    return std::wstring(buffer);
}
