#include "Headers.h"

int main() {
    StartSetup();
    cout << "\033[1;36m" << Title << "\033[0m\n\033[36mBy.Lonewolf239\033[0m" << endl << endl;
    SomeVisual();
    bool onlyWallpapers = true;
    cout << endl << "\033[33m[Выбор алгоритма]\033[0m" << endl;
    cout << "\033[36m[1]\033[0m Смена темы + обои\n\033[36m[2]\033[0m Сменить только обои" << endl;
    int cW = 0;
    while (cW != 49 && cW != 50) cW = _getch();
    if (cW == 49) onlyWallpapers = false;
    const wchar_t* wallpaper4KPath = L"C:\\Windows\\Web\\4K\\Wallpaper\\Windows\\img19_1920x1200.jpg";
    const wchar_t* wallpaperPath = L"C:\\Windows\\Web\\Wallpaper\\Windows\\img19.jpg";
    if (!onlyWallpapers) {
        cout << endl << "\033[33m[Установка тёмной темы]\033[0m" << endl;
        wait(500);
        string powershellCmd = "powershell -Command \"";
        powershellCmd += "Set-ItemProperty -Path 'HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize' -Name SystemUsesLightTheme -Value 0 -Type DWord -Force; ";
        powershellCmd += "Set-ItemProperty -Path 'HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize' -Name AppsUseLightTheme -Value 0 -Type DWord -Force";
        powershellCmd += "\"";
        if (system(powershellCmd.c_str()) != 0) cerr << ER << "Не удалось применить настройки темы." << endl;
        else cout << OK << "Тёмная тема успешно применена." << endl;
    }
    cout << endl << "\033[33m[Выбор обоев для установки]\033[0m" << endl;
    cout << "\033[36m[1]\033[0m Стандартные обои Windows 11\n\033[36m[2]\033[0m Свои обои" << endl;
    int c = 0;
    while (c != 49 && c != 50) c = _getch();
    cout << endl << "\033[33m[Установка новых обоев]\033[0m" << endl;
    wait(500);
    const wchar_t* wallpaperToSet = nullptr;
    if (c == 50) {
        string path;
        cout << endl << "Введите путь к файлу: ";
        getline(cin, path);
        const wchar_t* wcharPath = ToConstWchar_t(path);
        if (FileExists(wcharPath)) wallpaperToSet = wcharPath;
        else {
            cerr << "\033[31mОбои отсутствуют.\033[0m" << endl;
            c = 49;
        }
    }
    if (c == 49) {
        if (FileExists(wallpaper4KPath)) wallpaperToSet = wallpaper4KPath;
        else if (FileExists(wallpaperPath)) wallpaperToSet = wallpaperPath;
        else cerr << "\033[31mОбои отсутствуют.\033[0m" << endl;
    }
    if (wallpaperToSet != nullptr) {
        if (SetWallpaperUsingSPI(wallpaperToSet)) cout << OK << "Обои установлены с помощью SPI." << endl;
        else cerr << ER << "Не удалось установить обои с помощью SPI." << endl;
        if (SetWallpaperUsingRegistry(wallpaperToSet)) cout << OK << "Обои установлены через модификацию реестра." << endl;
        else cerr << ER << "Не удалось установить обои через модификацию реестра." << endl;
        if (SetWallpaperUsingPowerShell(wallpaperToSet)) cout << OK << "Обои установлены через PowerShell." << endl;
        else cerr << ER << "Не удалось установить обои через PowerShell." << endl;
        if (SetWallpaperUsingGDI(wallpaperToSet)) cout << OK << "Обои установлены через GDI." << endl;
        else cerr << ER << "Не удалось установить обои через GDI." << endl;
        if (SetWallpaperUsingDWM(wallpaperToSet)) cout << OK << "Обои установлены через DWM." << endl;
        else cerr << ER << "Не удалось установить обои через DWM." << endl;
        cout << endl << "\033[1;31m[Внимание]\033[0m Если обои не изменились сейчас или после\nперезагрузки, к сожалению, на этом ПК не получится\nсменить обои." << endl;
    }
    cout << endl << "\033[33m[Установка настроек Visual Studio]\033[0m" << endl;
    string vsCommand = "\"" + VSPath + "\" /ResetSettings \"" + GetExeDir() + "\\normal.vssettings\"";
    if (system(vsCommand.c_str()) != 0)  cerr << ER << "Не удалось применить настройки Visual Studio." << endl;
    else cout << OK << "Настройки Visual Studio успешно применены." << endl;
    string finalMessage = "Нажмите любую клавишу для перезагрузки ПК...";
    if (onlyWallpapers) finalMessage = "Нажмите любую клавишу завершения...";
    cout << endl << "\033[33m[" << finalMessage << "]\033[0m";
    pause;
    if (!onlyWallpapers) system("shutdown /r /t 0");
    return 0;
}

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

static string GetExeDir() {
    char path[MAX_PATH] = { 0 };
    DWORD len = GetModuleFileNameA(nullptr, path, MAX_PATH);
    if (len == 0 || len == MAX_PATH) return "";
    string exePath(path);
    string::size_type pos = exePath.find_last_of("\\/");
    string exeDir = (pos == string::npos) ? string(".") : exePath.substr(0, pos);
    return exeDir;
}