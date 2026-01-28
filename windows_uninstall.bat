@echo off
setlocal enabledelayedexpansion

set "APP_NAME=Cryptum"
set "CRYPTUM_DIR=%LocalAppData%\%APP_NAME%"

if exist "%CRYPTUM_DIR%" (
    rd /s /q "%CRYPTUM_DIR%"
    if errorlevel 1 (
        echo Cannot delete program directory!
        pause
        exit /b 1
    )
)

endlocal

echo Unnstallation done
pause