#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

void showMenu() {
    std::wcout << L"\n=== Process Manager ===\n";
    std::wcout << L"1. Create a new process (Notepad)\n";
    std::wcout << L"2. Show all processes\n";
    std::wcout << L"3. Terminate a process\n";
    std::wcout << L"4. Show threads of a process\n";
    std::wcout << L"5. Show modules of a process\n";
    std::wcout << L"6. Launch process with parameters\n";
    std::wcout << L"7. Change process priority\n";
    std::wcout << L"0. Exit\n";
    std::wcout << L"========================\n";
    std::wcout << L"Choose option: ";
}

void createProcess() {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::wstring command = L"\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\"";

    if (CreateProcessW(NULL, &command[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::wcout << L"Chrome started. PID: " << pi.dwProcessId << L"\n";
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::wcerr << L"Failed to launch Chrome. Error: " << GetLastError() << L"\n";
    }
}


void listProcesses() {
    PROCESSENTRY32W pe32 = { sizeof(pe32) };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32FirstW(hSnap, &pe32)) {
        do {
            std::wcout << L"[" << pe32.th32ProcessID << L"] " << pe32.szExeFile << L"\n";
        } while (Process32NextW(hSnap, &pe32));
    }
    else {
        std::wcerr << L"Error reading process list.\n";
    }
    CloseHandle(hSnap);
}

void terminateProcess() {
    DWORD pid;
    std::wcout << L"Enter Process ID: ";
    std::wcin >> pid;

    HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProc) {
        if (TerminateProcess(hProc, 0))
            std::wcout << L"Process terminated.\n";
        else
            std::wcerr << L"Failed to terminate. Error: " << GetLastError() << L"\n";
        CloseHandle(hProc);
    }
    else {
        std::wcerr << L"Unable to open process.\n";
    }
}

void listThreads(DWORD pid) {
    THREADENTRY32 te32 = { sizeof(te32) };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (Thread32First(hSnap, &te32)) {
        do {
            if (te32.th32OwnerProcessID == pid) {
                std::wcout << L"Thread ID: " << te32.th32ThreadID << L"\n";
            }
        } while (Thread32Next(hSnap, &te32));
    }
    else {
        std::wcerr << L"Failed to list threads.\n";
    }
    CloseHandle(hSnap);
}

void listModules(DWORD pid) {
    MODULEENTRY32W me32 = { sizeof(me32) };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

    if (Module32FirstW(hSnap, &me32)) {
        do {
            std::wcout << L"Module: " << me32.szModule << L"\n";
        } while (Module32NextW(hSnap, &me32));
    }
    else {
        std::wcerr << L"Failed to list modules.\n";
    }
    CloseHandle(hSnap);
}

void launchWithParams() {
    std::wcin.ignore();
    std::wstring input;
    std::wcout << L"Enter application and arguments: ";
    std::getline(std::wcin, input);

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    wchar_t* cmdLine = _wcsdup(input.c_str());
    if (CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::wcout << L"Process started with PID: " << pi.dwProcessId << L"\n";
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::wcerr << L"Error starting process. Error: " << GetLastError() << L"\n";
    }
    free(cmdLine);
}

void changePriority() {
    DWORD pid;
    std::wcout << L"Enter Process ID: ";
    std::wcin >> pid;

    HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
    if (!hProc) {
        std::wcerr << L"Failed to open process.\n";
        return;
    }

    std::wcout << L"Choose priority:\n";
    std::wcout << L"1. IDLE\n2. BELOW_NORMAL\n3. NORMAL\n4. ABOVE_NORMAL\n5. HIGH\n6. REALTIME\n";
    int option;
    std::wcin >> option;

    DWORD priority;
    switch (option) {
    case 1: priority = IDLE_PRIORITY_CLASS; break;
    case 2: priority = BELOW_NORMAL_PRIORITY_CLASS; break;
    case 3: priority = NORMAL_PRIORITY_CLASS; break;
    case 4: priority = ABOVE_NORMAL_PRIORITY_CLASS; break;
    case 5: priority = HIGH_PRIORITY_CLASS; break;
    case 6: priority = REALTIME_PRIORITY_CLASS; break;
    default: std::wcerr << L"Invalid choice.\n"; CloseHandle(hProc); return;
    }

    if (SetPriorityClass(hProc, priority))
        std::wcout << L"Priority changed.\n";
    else
        std::wcerr << L"Failed to change priority. Error: " << GetLastError() << L"\n";

    CloseHandle(hProc);
}

int main() {
    int choice;
    do {
        showMenu();
        std::wcin >> choice;

        switch (choice) {
        case 1: createProcess(); break;
        case 2: listProcesses(); break;
        case 3: terminateProcess(); break;
        case 4: {
            DWORD pid;
            std::wcout << L"Enter Process ID: ";
            std::wcin >> pid;
            listThreads(pid);
            break;
        }
        case 5: {
            DWORD pid;
            std::wcout << L"Enter Process ID: ";
            std::wcin >> pid;
            listModules(pid);
            break;
        }
        case 6: launchWithParams(); break;
        case 7: changePriority(); break;
        case 0: std::wcout << L"Exiting...\n"; break;
        default: std::wcerr << L"Invalid choice.\n"; break;
        }

    } while (choice != 0);

    return 0;
}
