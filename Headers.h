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

static void SetDarkTheme();

static void SetWallpaper();

static void SetVSSettings();

static const wchar_t* ToConstWchar_t(string str);

static int rand(int min, int max);

static bool FileExists(const wchar_t* filePath);

static bool SetWallpaperUsingSPI(const wchar_t* wallpaperPath);

static bool SetWallpaperUsingRegistry(const wchar_t* wallpaperPath);

static bool SetWallpaperUsingPowerShell(const wchar_t* wallpaperPath);

static bool SetWallpaperUsingGDI(const wchar_t* wallpaperPath);

static bool SetWallpaperUsingDWM(const wchar_t* wallpaperPath);

static void StartSetup();

static void SomeVisual();

static string GetExeDir();