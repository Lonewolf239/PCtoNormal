#include "Methods.h"

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
    string finalMessage = "Нажмите любую клавишу для перезагрузки ПК...";
    if (onlyWallpapers) finalMessage = "Нажмите любую клавишу завершения...";
    cout << endl << "\033[33m[" << finalMessage << "]\033[0m";
    pause;
    if (!onlyWallpapers) system("shutdown /r /t 0");
    return 0;
}