# Locale-Emulator-Core

This repository holds the source codes of the core libraries of the [Locale-Emulator project](https://github.com/xupefei/Locale-Emulator).

## Build

Prerequisites:

 - Microsoft Visual Studio 2013/2015

Building steps:

 1. Extract `.7z` packages in `_Compilers`, `_Libs` and `_WDK` folders.
 2. Load .sln solution file.
 3. Compile.

## Notes

 - The `_Compilers` folder contains modified toolchain taken from Microsoft Visual Studio 2015. Please compare `link.bak` and `link.exe` for the modifications.
 - The `_WDK` folder contains [Windows Driver Kit](https://en.wikipedia.org/wiki/Windows_Driver_Kit) version 7.1 and 8.0.

## License

![enter image description here](http://www.gnu.org/graphics/lgplv3-147x51.png)

All source code files in `Loader`, `LoaderDll` and `LocaleEmulator` folders are licensed under [LGPL-3.0](https://opensource.org/licenses/LGPL-3.0).

If you want make any modification on these source codes while keeping new codes not protected by LGPL-3.0, please contact me for a sublicense instead.
