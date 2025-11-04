@echo off
setlocal enabledelayedexpansion

set "APP_NAME=cryptum"
set "CRYPTUM_DIR=%ProgramFiles%\%APP_NAME%"
set "SRC_DIR=win-build-release"

net session >nul 2>&1
if errorlevel 1 (
    echo Administrator rights required!
    pause
    exit /b 1
)

cmake -S . -B "%SRC_DIR%" -G "Visual Studio 17 2022"
if errorlevel 1 (
    echo CMake Configuration failed!
    pause
    exit /b 1
)

cmake --build "%SRC_DIR%" --config Release
if errorlevel 1 (
    echo CMake Build failed!
    pause
    exit /b 1
)

if not exist "%CRYPTUM_DIR%" mkdir "%CRYPTUM_DIR%"

copy /Y "%SRC_DIR%\Release\cryptum.exe" "%CRYPTUM_DIR%\" >nul
copy /Y "%SRC_DIR%\Release\aes.dll" "%CRYPTUM_DIR%\" >nul
copy /Y "%SRC_DIR%\Release\des.dll" "%CRYPTUM_DIR%\" >nul
copy /Y "%SRC_DIR%\Release\rava.dll" "%CRYPTUM_DIR%\" >nul

set "UserPath="
for /f "usebackq tokens=2*" %%A in (`reg query "HKCU\Environment" /v Path 2^>nul ^| findstr /R /C:"Path"`) do (
    set "UserPath=%%B"
)
if not defined UserPath set "UserPath="

echo !UserPath! | find /i "%CRYPTUM_DIR%" >nul
if errorlevel 1 (
    if "!UserPath!"=="" (
        set "NewPath=%CRYPTUM_DIR%"
    ) else (
        set "NewPath=%UserPath%;%CRYPTUM_DIR%"
    )
    setx Path "!NewPath!"
    echo Restart shell to update PATH
)

endlocal

echo Installation done
pause