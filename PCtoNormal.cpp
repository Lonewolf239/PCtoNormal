#include <iostream>
#include <locale>
#include <conio.h>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")

#define wait(x) this_thread::sleep_for(chrono::milliseconds(x))
#define pause system("pause >NUL")

using namespace std;

static bool FileExists(const wchar_t* filePath) {
    DWORD fileAttr = GetFileAttributesW(filePath);
    return (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY));
}

static bool SetWallpaperUsingSPI(const wchar_t* wallpaperPath) {
    if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)wallpaperPath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        cout << "\033[32mОбои установлены с помощью SystemParametersInfoW.\033[0m" << endl;
        return true;
    }
    else {
        cerr << "\033[31mНе удалось установить обои с помощью SystemParametersInfoW.\033[0m" << endl;
        return false;
    }
}

static bool SetWallpaperUsingRegistry(const wchar_t* wallpaperPath) {
    HKEY hKey;
    LONG lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, KEY_SET_VALUE, &hKey);
    if (lResult == ERROR_SUCCESS) {
        lResult = RegSetValueExW(hKey, L"Wallpaper", 0, REG_SZ, (const BYTE*)wallpaperPath, (wcslen(wallpaperPath) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
        if (lResult == ERROR_SUCCESS) {
            SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
            cout << "\033[32mОбои установлены через модификацию реестра.\033[0m" << endl;
            return true;
        }
    }
    cerr << "\033[31mНе удалось установить обои через модификацию реестра.\033[0m" << endl;
    return false;
}

static bool SetWallpaperUsingPowerShell(const wchar_t* wallpaperPath) {
    wstring command = L"powershell -Command \"";
    command += L"Set-ItemProperty -Path 'HKCU:\\Control Panel\\Desktop' -Name WallPaper -Value '";
    command += wallpaperPath;
    command += L"'; ";
    command += L"RUNDLL32.EXE user32.dll,UpdatePerUserSystemParameters; ";
    command += L"\"";
    string narrowCommand(command.begin(), command.end());
    int result = system(narrowCommand.c_str());
    if (result != 0) {
        cerr << "Ошибка при установке обоев через PowerShell: " << result << endl;
        return false;
    }
    cout << "\033[32mОбои установлены через PowerShell.\033[0m" << endl;
    return true;
}

static void SomeVisual() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(4, 6);
    int attempts = dis(gen);
    for (int i = 0; i < attempts; i++) {
        cout << "\r\033[33mПопытка обхода блокировки" << string(i + 1, '.') << "\033[0m" << flush;
        wait(500);
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "PCtoNormal v0.1\n\033[36mBy.Lonewolf239\033[0m" << endl << endl;
    cout << "\033[33mУстановка тёмной темы...\033[0m" << endl;
    const wchar_t* wallpaper4KPath = L"C:\\Windows\\Web\\4K\\Wallpaper\\Windows\\img19_1920x1200.jpg";
    const wchar_t* wallpaperPath = L"C:\\Windows\\Web\\Wallpaper\\Windows\\img19.jpg";
    string powershellCmd = "powershell -Command \"";
    powershellCmd += "Set-ItemProperty -Path 'HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize' -Name SystemUsesLightTheme -Value 0 -Type DWord -Force; ";
    powershellCmd += "Set-ItemProperty -Path 'HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize' -Name AppsUseLightTheme -Value 0 -Type DWord -Force";
    powershellCmd += "\"";
    if (system(powershellCmd.c_str()) != 0)
        cerr << "\033[31mНе удалось применить настройки темы через PowerShell.\033[0m" << endl;
    else
        cout << "\033[32mТёмная тема успешно применена.\033[0m" << endl;
    SomeVisual();
    bool wallpaperSet = false;
    const wchar_t* wallpaperToSet = nullptr;
    if (FileExists(wallpaper4KPath))
        wallpaperToSet = wallpaper4KPath;
    else if (FileExists(wallpaperPath))
        wallpaperToSet = wallpaperPath;
    else
        cerr << "\033[31mОбои отсутствуют.\033[0m" << endl;
    if (wallpaperToSet != nullptr) {
        bool firstAttempt = SetWallpaperUsingSPI(wallpaperToSet);
        bool secondAttempt = SetWallpaperUsingRegistry(wallpaperToSet);
        bool thirdAttempt = SetWallpaperUsingPowerShell(wallpaperToSet);
        wallpaperSet = firstAttempt || secondAttempt || thirdAttempt;
        if (!wallpaperSet)
            cerr << "\033[31mНе удалось установить обои с помощью доступных методов.\033[0m" << endl;
    }
    cout << "\033[1;31m[Внимание]\033[0m Если обои не изменились сейчас или после перезагрузки, к сожалению, на этом ПК не получится сменить обои." << endl;
    cout << "\033[33m[Нажмите любую клавишу для перезагрузки...]\033[0m";
    pause;
    system("shutdown /r /t 0");
    return 0;
}