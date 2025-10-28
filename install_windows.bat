@echo off
setlocal enabledelayedexpansion

set "CRYPTUM_DIR=C:\Program Files\Cryptum"

net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [ERROR]: Administrator rights required
    pause
    exit /b 1
)

if not exist "%CRYPTUM_DIR%" (
    mkdir "%CRYPTUM_DIR%"
    if %errorLevel% neq 0 (
        echo [ERROR]: Cannot create directory
        pause
        exit /b 1
    )
)

set "SOURCE_DIR=cmake-build-release"

copy /Y "%SOURCE_DIR%\cryptum.exe" "%CRYPTUM_DIR%\" >nul
copy /Y "%SOURCE_DIR%\aes.dll" "%CRYPTUM_DIR%\" >nul
copy /Y "%SOURCE_DIR%\des.dll" "%CRYPTUM_DIR%\" >nul
copy /Y "%SOURCE_DIR%\rava.dll" "%CRYPTUM_DIR%\" >nul

if %errorLevel% neq 0 (
    echo [ERROR]: Cannot copy files in directory
    pause
    exit /b 1
)

set "UserPath="
for /f "usebackq tokens=2*" %%A in (`reg query "HKCU\Environment" /v Path 2^>nul ^| findstr /R /C:"Path"`) do (
    set "UserPath=%%B"
)
if not defined UserPath set "UserPath="

echo !UserPath! | find /i "%CRYPTUM_DIR%" >nul
if %errorlevel%==1 (
    if "!UserPath!"=="" (
        set "NewPath=%CRYPTUM_DIR%"
    ) else (
        set "NewPath=%UserPath%;%CRYPTUM_DIR%"
    )
    setx Path "!NewPath!"
)

endlocal

echo Restart shell to update PATH
echo Installation done
pause