@SETLOCAL

@set BASE_PATH=%~dp0
@set BASE_PATH=%BASE_PATH:~0,-1%
@set BASE_PATH=%BASE_PATH%\..\..

@set INPUT=%1
@shift /1
@type %INPUT% | %BASE_PATH%\build\Debug\Compiler.exe %1 %2 %3 %4 %5 %6 %7 %8 %9

@echo.
@echo Additional options:
@echo   -w ^<width^>     Set SVG width (default: 1000)
@echo   -h ^<height^>    Set SVG height (default: 1000)
@echo   --width ^<w^>    Set SVG width (default: 1000)
@echo   --height ^<h^>   Set SVG height (default: 1000)
@echo.
@echo Example: start.bat myfile.dib -w 800 -h 600

@ENDLOCAL
