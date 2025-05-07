#include <windows.h>
#include <iostream>
#include <string>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char windowTitle[256];

    if (GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle))) {
        std::string title(windowTitle);

        if (title.find("Google Chrome") != std::string::npos) {
            std::cout << "Chrome window found!" << std::endl;

            SetWindowPos(hwnd, HWND_TOP, 100, 100, 800, 600, SWP_SHOWWINDOW);
            std::cout << "Position and size changed." << std::endl;

            ShowWindow(hwnd, SW_MAXIMIZE);
            std::cout << "Chrome window maximized." << std::endl;

            Sleep(3000);

            SendMessage(hwnd, WM_CLOSE, 0, 0);
            std::cout << "Chrome window closed." << std::endl;

            return FALSE;
        }
    }
    return TRUE; 
}

int main() {
    EnumWindows(EnumWindowsProc, 0);

    return 0;
}
