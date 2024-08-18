#pragma once
#include <windows.h>
#include <string>

class RegistryManager {
public:
    static void AddToStartup(const std::wstring& exePath);
};
