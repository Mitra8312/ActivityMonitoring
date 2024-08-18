// ActivityMonitor.cpp
#include "ActivityMonitor.h"

bool ActivityMonitor::IsUserActive() {
    LASTINPUTINFO lii = { 0 };
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (GetLastInputInfo(&lii)) {
        DWORD tickCount = GetTickCount();
        DWORD idleTime = tickCount - lii.dwTime;
        return idleTime < 1000 * 60; 
    }
    return false;
}
