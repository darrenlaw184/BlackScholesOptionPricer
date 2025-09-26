@echo off
REM Black-Scholes Option Pricer - Run Application Script

echo Starting Black-Scholes Option Pricer...
echo ======================================

cd /d "%~dp0"

REM Check if build directory exists
if not exist "build" (
    echo Error: Build directory not found.
    echo Please run setup_and_build.bat first to build the project.
    pause
    exit /b 1
)

cd build

REM Try to find and run the executable
if exist "BlackScholesOptionPricer.exe" (
    echo Running application...
    BlackScholesOptionPricer.exe
) else if exist "Release\BlackScholesOptionPricer.exe" (
    echo Running release version...
    Release\BlackScholesOptionPricer.exe
) else if exist "Debug\BlackScholesOptionPricer.exe" (
    echo Running debug version...
    Debug\BlackScholesOptionPricer.exe
) else (
    echo Error: Could not find BlackScholesOptionPricer.exe
    echo Please make sure the project has been built successfully.
    echo Run setup_and_build.bat to build the project.
    pause
    exit /b 1
)

cd ..

REM Keep window open if application exits with error
if !errorlevel! neq 0 (
    echo.
    echo Application exited with error code: !errorlevel!
    pause
)
