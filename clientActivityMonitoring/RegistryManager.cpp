#include "RegistryManager.h"

void RegistryManager::AddToStartup(const std::wstring& exePath) {
    HKEY hKey;
    const wchar_t* czStartName = L"EmployeeMonitorClient";
    const wchar_t* czExePath = exePath.c_str();

    LONG lnRes = RegOpenKeyEx(HKEY_CURRENT_USER,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_WRITE, &hKey);
    if (ERROR_SUCCESS == lnRes) {
        lnRes = RegSetValueEx(hKey,
            czStartName,
            0,
            REG_SZ,
            (const BYTE*)czExePath,
            static_cast<DWORD>((wcslen(czExePath) + 1) * sizeof(wchar_t)));
    }
    RegCloseKey(hKey);
}
