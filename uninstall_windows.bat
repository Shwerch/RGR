@echo off
setlocal enabledelayedexpansion

set "CRYPTUM_DIR=C:\Program Files\Cryptum"

net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [ERROR]: Administrator rights required
    pause
    exit /b 1
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

echo Restart shell to update Path
echo Installation done
pause