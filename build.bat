@echo off
REM ========================================
REM   Football Field Manager - Build Script
REM ========================================

echo.
echo ========================================
echo    BUILDING PROJECT
echo ========================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Configure with CMake
echo [1/2] Configuring CMake...
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [2/2] Building project...
cmake --build build --config Debug -- -j 4
if errorlevel 1 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo    BUILD SUCCESSFUL!
echo ========================================
echo    Executable: build\bin\FootballFieldManager.exe
echo    Data folder: D:\QT_PBL2\Data
echo ========================================
echo.
pause
