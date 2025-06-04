#include <windows.h>
#include <iostream>
#include <string>

HWND foundWindow = nullptr;
std::wstring targetSubstring = L"Блокнот"; // або L"Notepad"

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    const int length = GetWindowTextLengthW(hwnd);
    if (length == 0) return TRUE;

    wchar_t* buffer = new wchar_t[length + 1];
    GetWindowTextW(hwnd, buffer, length + 1);
    std::wstring windowTitle(buffer);
    delete[] buffer;

    if (windowTitle.find(targetSubstring) != std::wstring::npos) {
        foundWindow = hwnd;
        return FALSE; // зупинити пошук
    }

    return TRUE;
}

VOID CALLBACK TimerCallback(PVOID lpParam, BOOLEAN TimerOrWaitFired) {
    foundWindow = nullptr;
    EnumWindows(EnumWindowsProc, 0);

    if (!foundWindow) {
        std::wcout << L" Window not found (partial match failed)." << std::endl;
        return;
    }

    std::wcout << L" Found window!" << std::endl;

    // Перемістити і змінити розмір
    if (SetWindowPos(foundWindow, HWND_TOP, 100, 100, 800, 600, SWP_SHOWWINDOW))
        std::wcout << L" Window moved and resized." << std::endl;
    else
        std::wcout << L" Failed to move window." << std::endl;

    // Мінімізувати і повернути
    ShowWindow(foundWindow, SW_MINIMIZE);
    Sleep(2000);
    ShowWindow(foundWindow, SW_RESTORE);
    std::wcout << L" Window minimized and restored." << std::endl;
}

int main() {
    HANDLE hTimer = NULL;
    HANDLE hTimerQueue = CreateTimerQueue();

    if (!hTimerQueue) {
        std::cerr << "Error: Failed to create timer queue." << std::endl;
        return 1;
    }

    if (!CreateTimerQueueTimer(&hTimer, hTimerQueue, TimerCallback, NULL, 3000, 10000, 0)) {
        std::cerr << "Error: Failed to create timer." << std::endl;
        return 1;
    }

    std::wcout << L" Timer started. Press Enter to stop..." << std::endl;
    std::cin.get();

    DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL);
    DeleteTimerQueue(hTimerQueue);

    return 0;
}
