@echo off
setlocal enabledelayedexpansion

set "APP_NAME=cryptum"
set "CRYPTUM_DIR=%ProgramFiles%\%APP_NAME%"

net session >nul 2>&1
if %errorLevel% neq 0 (
    echo Administrator rights required
    pause
    exit 1
)

if exist "%CRYPTUM_DIR%" (
    rd /s /q "%CRYPTUM_DIR%"
    if %errorLevel%==1 (
        echo [ERROR]: Cannot delete directory
        pause
        exit /b 1
    )
)

endlocal

echo Unnstallation done
pause