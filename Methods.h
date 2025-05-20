#include <iostream>
#include <locale>
#include <conio.h>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include <codecvt>
#include <dwmapi.h>
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Dwmapi.lib")
#define wait(x) this_thread::sleep_for(chrono::milliseconds(x))
#define pause system("pause >NUL")

using namespace std;

const LPCSTR Title = "PC2Normal v0.3";
const LPCSTR ER = "\033[31m[ER]\033[0m ";
const LPCSTR OK = "\033[32m[OK]\033[0m ";

static const wchar_t* ToConstWchar_t(string str) {
    wstring wstr = wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(str);
    const wchar_t* result = wstr.c_str();
    return result;
}

static int rand(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

static bool FileExists(const wchar_t* filePath) {
    DWORD fileAttr = GetFileAttributesW(filePath);
    return (fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY));
}

static bool SetWallpaperUsingSPI(const wchar_t* wallpaperPath) {
    return SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)wallpaperPath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
}

static bool SetWallpaperUsingRegistry(const wchar_t* wallpaperPath) {
    HKEY hKey;
    LONG lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, KEY_SET_VALUE, &hKey);
    if (lResult == ERROR_SUCCESS) {
        lResult = RegSetValueExW(hKey, L"Wallpaper", 0, REG_SZ, (const BYTE*)wallpaperPath, (wcslen(wallpaperPath) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
        if (lResult == ERROR_SUCCESS) {
            SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
            return true;
        }
    }
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
    return system(narrowCommand.c_str()) == 0;
}

static bool SetWallpaperUsingGDI(const wchar_t* wallpaperPath) {
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, wallpaperPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitmap) return false;
    bool result = SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)wallpaperPath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
    DeleteObject(hBitmap);
    return result;
}

static bool SetWallpaperUsingDWM(const wchar_t* wallpaperPath) {
    HRESULT hr = DwmSetWindowAttribute(GetConsoleWindow(), DWMWA_USE_IMMERSIVE_DARK_MODE, &wallpaperPath, sizeof(wallpaperPath));
    return SUCCEEDED(hr);
}

static void StartSetup() {
    setlocale(LC_ALL, "rus");
    SetConsoleTitleA(Title);
    system("mode con cols=70 lines=24");
    HWND consoleWindow = GetConsoleWindow();
    RECT r;
    GetWindowRect(consoleWindow, &r);
    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - (r.right - r.left) / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - (r.bottom - r.top) / 2;
    SetWindowPos(consoleWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &structCursorInfo);
    HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(consoleHandle, &consoleMode);
    SetConsoleMode(consoleHandle, consoleMode & ~(ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_INSERT_MODE));
    HWND console = GetConsoleWindow();
    SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX & ~WS_MAXIMIZE);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo{};
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    wstring font_name = L"Consolas";
    wcscpy_s(fontInfo.FaceName, font_name.c_str());
    fontInfo.dwFontSize.Y = 20;
    fontInfo.dwFontSize.X = 10;
    fontInfo.FontWeight = FW_NORMAL;
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

static void SomeVisual() {
    for (int i = 1; i <= 10; i++) {
        cout << "\r\033[33m[Попытка обхода блокировки]\033[0m \033[36m" << (string(i, '=') + string(10 - i, '_')) << "\033[0m" << flush;
        wait(rand(50, 1000));
    }
    cout << endl;
}