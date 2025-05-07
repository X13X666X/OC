#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

struct Alarm {
    std::string name;
    int timeInSeconds;
};

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    MessageBeep(MB_OK);
    MessageBoxW(hwnd, L"Time for a break!", L"Timer finished", MB_OK);
}

void CALLBACK EventAlarm(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    if (idEvent == 1) {
        MessageBeep(MB_ICONINFORMATION);
        MessageBoxW(hwnd, L"Time for lunch!", L"Alarm", MB_OK);
    }
    else if (idEvent == 2) {
        MessageBeep(MB_ICONASTERISK);
        MessageBoxW(hwnd, L"Time for a meeting!", L"Alarm", MB_OK);
    }
}

void setSystemDate(SYSTEMTIME newTime)
{
    if (!SetSystemTime(&newTime)) {
        std::cerr << "Failed to set system time." << std::endl;
    }
}

void restoreSystemDate(SYSTEMTIME originalTime)
{
    SetSystemTime(&originalTime);
}

int main()
{
    int timerDuration;
    std::cout << "Enter the timer duration (in minutes): ";
    std::cin >> timerDuration;
    timerDuration *= 60000;

    SetTimer(NULL, 1, timerDuration, TimerProc);

    int numAlarms;
    std::cout << "Enter the number of alarms: ";
    std::cin >> numAlarms;

    std::vector<Alarm> alarms(numAlarms);

    for (int i = 0; i < numAlarms; i++) {
        std::cout << "Enter the event name for alarm " << i + 1 << ": ";
        std::cin.ignore();
        std::getline(std::cin, alarms[i].name);

        std::cout << "Enter the time for alarm " << i + 1 << " (in hours and minutes, e.g., 14:30): ";
        std::string timeInput;
        std::getline(std::cin, timeInput);

        int hours, minutes;
        sscanf(timeInput.c_str(), "%d:%d", &hours, &minutes);

        SYSTEMTIME currentTime;
        GetSystemTime(&currentTime);
        int currentTimeInSeconds = currentTime.wHour * 3600 + currentTime.wMinute * 60 + currentTime.wSecond;
        int alarmTimeInSeconds = hours * 3600 + minutes * 60;
        alarms[i].timeInSeconds = alarmTimeInSeconds - currentTimeInSeconds;

        SetTimer(NULL, i + 1, alarms[i].timeInSeconds * 1000, EventAlarm);
    }

    SYSTEMTIME originalTime, newTime;
    GetSystemTime(&originalTime);

    std::cout << "Enter the new date to change system time (year, month, day): ";
    std::cin >> newTime.wYear >> newTime.wMonth >> newTime.wDay;
    newTime.wHour = 0;
    newTime.wMinute = 0;
    newTime.wSecond = 0;
    newTime.wMilliseconds = 0;

    setSystemDate(newTime);

    Sleep(10000);

    restoreSystemDate(originalTime);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
