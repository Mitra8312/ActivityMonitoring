#pragma once
#include <vector>
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class ScreenshotCapture {
public:
    static void CaptureScreenshot(std::vector<BYTE>& buffer);

private:
    static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};
