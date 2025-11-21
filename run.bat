@echo off
REM ========================================
REM   Football Field Manager - Run Script
REM ========================================

echo.
echo ========================================
echo    RUNNING APPLICATION
echo ========================================
echo.

REM Check if executable exists
if not exist "build\bin\FootballFieldManager.exe" (
    echo ERROR: Application not found!
    echo Please run build.bat first.
    echo.
    pause
    exit /b 1
)

REM Check if Data folder exists
if not exist "D:\PBL2-\Data" (
    echo WARNING: Data folder not found!
    echo Creating Data directory...
    mkdir "D:\PBL2-\Data"
)

REM Run the application
echo Starting Football Field Manager...
echo.
start "" "build\bin\FootballFieldManager.exe"

echo Application launched!
echo Data location: D:\PBL2-\Data
echo.
