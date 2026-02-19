# PCtoNormal

A utility for forcing desktop settings on locked systems.

## Description

PCtoNormal is a small C++ utility that bypasses administrative restrictions and allows you to:
- Apply a dark Windows theme
- Set a custom or default wallpaper
- Configure Visual Studio (dark theme, fonts, settings)

Designed for use on university and corporate computers where standard configuration tools are disabled by administrators.

## Features

- Force the Windows dark theme (via registry and system calls)
- Set wallpaper without Group Policy
- Quick Visual Studio setup
- Minimal dependencies—WinAPI only
- Fast execution and small executable size

## Requirements

- Windows 10/11
- C++ compiler with C++17 support
- Write permissions to the current user's registry (no administrator rights required)

## Important

The utility runs in the context of the current user and does not require administrator rights. Changes are applied only to your account and do not affect the system globally.

## License

MIT License—use as you wish.