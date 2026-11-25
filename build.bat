@echo off
echo ============================================================
echo   Compiling Transformer Efficiency Analyzer (100% C++)
echo ============================================================

g++ -std=c++17 -O2 -Iinclude src\main.cpp src\transformer_core.cpp src\svg_plotter.cpp src\ui_templates.cpp -lws2_32 -o transformer_analyzer.exe

if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Compilation complete: transformer_analyzer.exe
    echo.
    echo Starting server...
    transformer_analyzer.exe
) else (
    echo.
    echo [ERROR] Compilation failed. Please check the error messages above.
    pause
)
