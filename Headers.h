#include <iostream>
#include <locale>
#include <conio.h>
#include <windows.h>
#include <fstream>
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

const LPCSTR Title = "PC2Normal v0.4";
const LPCSTR ER = "\033[31m[ER]\033[0m ";
const LPCSTR OK = "\033[32m[OK]\033[0m ";
const string VSPath = R"(C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe)";
const wchar_t* Wallpaper4KPath = L"C:\\Windows\\Web\\4K\\Wallpaper\\Windows\\img19_1920x1200.jpg";
const wchar_t* WallpaperPath = L"C:\\Windows\\Web\\Wallpaper\\Windows\\img19.jpg";

static void SetDarkTheme();

static void SetWallpaper();

static void SetVSSettings();

static void SomeVisual();