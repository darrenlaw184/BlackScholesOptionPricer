@echo off
REM Black-Scholes Option Pricer - Windows Setup and Build Script
REM This script sets up dependencies and builds the project on Windows

setlocal EnableDelayedExpansion

echo Black-Scholes Option Pricer - Windows Setup and Build
echo =====================================================

REM Check if PowerShell is available
where powershell >nul 2>nul
if !errorlevel! neq 0 (
    echo Error: PowerShell is required but not found.
    echo Please install PowerShell to continue.
    pause
    exit /b 1
)

echo.
echo [1/4] Setting up external dependencies...
powershell -ExecutionPolicy Bypass -File "setup_dependencies.ps1"
if !errorlevel! neq 0 (
    echo Error: Failed to set up dependencies.
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo [2/4] Creating build directory...
if not exist "build" mkdir build
cd build

echo.
echo [3/4] Configuring with CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release
if !errorlevel! neq 0 (
    echo Error: CMake configuration failed.
    echo Please make sure you have:
    echo - CMake installed and in PATH
    echo - A C++ compiler (Visual Studio, MinGW, etc.)
    echo - GLFW and OpenGL libraries available
    pause
    exit /b 1
)

echo.
echo [4/4] Building the project...
cmake --build . --config Release
if !errorlevel! neq 0 (
    echo Error: Build failed.
    echo Please check the error messages above.
    pause
    exit /b 1
)

echo.
echo ============================================
echo Build completed successfully!
echo ============================================
echo.
echo The executable has been created in the build directory.
echo.
echo To run the application:
echo   cd build
echo   BlackScholesOptionPricer.exe
echo.
echo Or simply run:
echo   run_application.bat
echo.

REM Ask if user wants to run the application
set /p choice="Would you like to run the application now? (y/n): "
if /i "!choice!"=="y" (
    echo.
    echo Starting Black-Scholes Option Pricer...
    if exist "BlackScholesOptionPricer.exe" (
        start BlackScholesOptionPricer.exe
    ) else if exist "Release\BlackScholesOptionPricer.exe" (
        start Release\BlackScholesOptionPricer.exe
    ) else if exist "Debug\BlackScholesOptionPricer.exe" (
        start Debug\BlackScholesOptionPricer.exe
    ) else (
        echo Error: Could not find the executable.
        echo Please check the build directory.
    )
) else (
    echo.
    echo Setup complete. You can run the application manually when ready.
)

cd ..
pause
