@echo off
echo ============================================================
echo   Compiling Transformer Efficiency Analyzer
echo ============================================================

g++ -std=c++17 -O2 -Iinclude src\main.cpp src\transformer_core.cpp src\svg_plotter.cpp src\ui_templates.cpp -lws2_32 -o transformer_analyzer.exe

if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Build complete: transformer_analyzer.exe
    echo.
    echo Launching analyzer...
    transformer_analyzer.exe
) else (
    echo.
    echo [ERROR] Build failed. Please inspect the errors above.
    pause
)
