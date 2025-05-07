#include <windows.h>
#include <iostream>
#include <string>

void createOrEditRegistryKey() {
    HKEY hKey;
    std::string subKey = "Software\\asdzxc";
    std::string valueName, valueData;

    if (RegCreateKeyExA(HKEY_CURRENT_USER, subKey.c_str(), 0, NULL, 0,
        KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        std::cout << "Registry key created or opened successfully.\n";

        std::cout << "Enter the key name: ";
        std::getline(std::cin, valueName);
        std::cout << "Enter the key value: ";
        std::getline(std::cin, valueData);

        if (RegSetValueExA(hKey, valueName.c_str(), 0, REG_SZ,
            reinterpret_cast<const BYTE*>(valueData.c_str()),
            static_cast<DWORD>(valueData.size() + 1)) == ERROR_SUCCESS) {
            std::cout << "Key/value created or edited successfully.\n";
        }
        else {
            std::cout << "Failed to set registry value.\n";
        }

        RegCloseKey(hKey);
    }
    else {
        std::cout << "Failed to create or open registry key.\n";
    }
}

void saveRegistryToFile() {
    std::string filePath;
    std::cout << "Enter file path to save registry (e.g., C:\\backup.reg): ";
    std::getline(std::cin, filePath);

    std::string regKey = "HKCU\\Software\\asdzxc";
    std::string command = "reg export \"" + regKey + "\" \"" + filePath + "\" /y";

    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Registry saved successfully.\n";
    }
    else {
        std::cout << "Failed to save registry.\n";
        std::cout << "Make sure the program is running as Administrator and path is valid.\n";
    }
}

void restoreRegistryFromFile() {
    std::string filePath;
    std::cout << "Enter file path to restore registry (e.g., C:\\backup.reg): ";
    std::getline(std::cin, filePath);

    std::string command = "reg import \"" + filePath + "\"";

    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Registry restored successfully.\n";
    }
    else {
        std::cout << "Failed to restore registry.\n";
        std::cout << "Ensure the file path is correct and you run the program as Administrator.\n";
    }
}

int main() {
    while (true) {
        std::cout << "\n1. Create or edit registry key\n";
        std::cout << "2. Save registry to file\n";
        std::cout << "3. Restore registry from file\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";

        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        int choice = std::stoi(choiceStr);

        switch (choice) {
        case 1:
            createOrEditRegistryKey();
            break;
        case 2:
            saveRegistryToFile();
            break;
        case 3:
            restoreRegistryFromFile();
            break;
        case 4:
            return 0;
        default:
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
