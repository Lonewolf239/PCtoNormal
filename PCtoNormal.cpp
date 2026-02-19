#include "Headers.h"
#include "API.h"

int main() {
    API::StartSetup();
    cout << "\033[1;36m" << Title << "\033[0m\n\033[36mBy.Lonewolf239\033[0m" << endl << endl;
    SomeVisual();
    int mode = 0;
    cout << endl << "\033[33m[Выбор алгоритма]\033[0m" << endl;
    cout << "\033[36m[1]\033[0m Полная настройка\n\033[36m[2]\033[0m Сменить только обои\n\033[36m[3]\033[0m Настроить VS" << endl;
    int cW = 0;
    while (cW < 49 || cW > 51) cW = _getch();
    if (cW == 50) mode = 1;
    else if (cW == 51) mode = 2;
    if (mode == 0) SetDarkTheme();
    if (mode < 2) SetWallpaper();
    if (mode != 1) SetVSSettings();
    string finalMessage = "Нажмите любую клавишу для перезагрузки ПК...";
    if (mode != 0) finalMessage = "Нажмите любую клавишу завершения...";
    cout << endl << "\033[33m[" << finalMessage << "]\033[0m";
    pause;
    if (mode == 0) system("shutdown /r /t 0");
    return 0;
}

static void SetDarkTheme() {
    cout << endl << "\033[33m[Установка тёмной темы]\033[0m" << endl;
    wait(500);
    string powershellCmd = "powershell -Command \"";
    powershellCmd += "Set-ItemProperty -Path 'HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize' -Name SystemUsesLightTheme -Value 0 -Type DWord -Force; ";
    powershellCmd += "Set-ItemProperty -Path 'HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize' -Name AppsUseLightTheme -Value 0 -Type DWord -Force";
    powershellCmd += "\"";
    if (system(powershellCmd.c_str()) != 0) cerr << ER << "Не удалось применить настройки темы." << endl;
    else cout << OK << "Тёмная тема успешно применена." << endl;
}

static void SetWallpaper() {
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
        const wchar_t* wcharPath = API::ToConstWchar_t(path);
        if (API::FileExists(wcharPath)) wallpaperToSet = wcharPath;
        else {
            cerr << "\033[31mОбои отсутствуют.\033[0m" << endl;
            c = 49;
        }
    }
    if (c == 49) {
        if (API::FileExists(Wallpaper4KPath)) wallpaperToSet = Wallpaper4KPath;
        else if (API::FileExists(WallpaperPath)) wallpaperToSet = WallpaperPath;
        else cerr << "\033[31mОбои отсутствуют.\033[0m" << endl;
    }
    if (wallpaperToSet != nullptr) {
        if (API::SetWallpaperUsingSPI(wallpaperToSet)) cout << OK << "Обои установлены с помощью SPI." << endl;
        else cerr << ER << "Не удалось установить обои с помощью SPI." << endl;
        if (API::SetWallpaperUsingRegistry(wallpaperToSet)) cout << OK << "Обои установлены через модификацию реестра." << endl;
        else cerr << ER << "Не удалось установить обои через модификацию реестра." << endl;
        if (API::SetWallpaperUsingPowerShell(wallpaperToSet)) cout << OK << "Обои установлены через PowerShell." << endl;
        else cerr << ER << "Не удалось установить обои через PowerShell." << endl;
        if (API::SetWallpaperUsingGDI(wallpaperToSet)) cout << OK << "Обои установлены через GDI." << endl;
        else cerr << ER << "Не удалось установить обои через GDI." << endl;
        if (API::SetWallpaperUsingDWM(wallpaperToSet)) cout << OK << "Обои установлены через DWM." << endl;
        else cerr << ER << "Не удалось установить обои через DWM." << endl;
        cout << endl << "\033[1;31m[Внимание]\033[0m Если обои не изменились сейчас или после\nперезагрузки, к сожалению, на этом ПК не получится\nсменить обои." << endl;
    }
}

static void SetVSSettings() {
    cout << endl << "\033[33m[Установка настроек Visual Studio]\033[0m" << endl;
    string vsCommand = "\"\"" + VSPath + "\" /ResetSettings \"" + API::GetExeDir() + "\\normal.vssettings\"\"";
    if (system(vsCommand.c_str()) != 0)  cerr << ER << "Не удалось применить настройки Visual Studio." << endl;
    else cout << OK << "Настройки Visual Studio успешно применены." << endl;
}

static void SomeVisual() {
    for (int i = 1; i <= 10; i++) {
        cout << "\r\033[33m[Попытка обхода блокировки]\033[0m \033[36m" << (string(i, '#') + string(10 - i, '-')) << "\033[0m" << flush;
        wait(API::rand(50, 666));
    }
    cout << endl;
}
