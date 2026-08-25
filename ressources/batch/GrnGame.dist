@echo off
setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
set "RUNTIME_DIR=%SCRIPT_DIR%..\runtime"
set "PROJECT_DIR=%CD%"
set "DIST_DIR=%PROJECT_DIR%\dist"

if exist "%DIST_DIR%" (
    rmdir /s /q "%DIST_DIR%"
)

mkdir "%DIST_DIR%\server"
mkdir "%DIST_DIR%\linux-x86_64"
mkdir "%DIST_DIR%\linux-arm64"
mkdir "%DIST_DIR%\macos-x86_64"
mkdir "%DIST_DIR%\macos-arm64"
mkdir "%DIST_DIR%\windows-x64"
mkdir "%DIST_DIR%\windows-arm64"


copy /Y "%RUNTIME_DIR%\server.py" ^
    "%DIST_DIR%\server\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-wasm-wasm32-releaseembedded.html" ^
    "%DIST_DIR%\server\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-wasm-wasm32-releaseembedded.js" ^
    "%DIST_DIR%\server\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-wasm-wasm32-releaseembedded.wasm" ^
    "%DIST_DIR%\server\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-linux-arm64-releaseembedded" ^
    "%DIST_DIR%\linux-arm64\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-linux-x86_64-releaseembedded" ^
    "%DIST_DIR%\linux-x86_64\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-macosx-arm64-releaseembedded" ^
    "%DIST_DIR%\macos-arm64\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-macosx-x86_64-releaseembedded" ^
    "%DIST_DIR%\macos-x86_64\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-windows-arm64-releaseembedded.exe" ^
    "%DIST_DIR%\windows-arm64\" >nul

copy /Y "%RUNTIME_DIR%\Runtime-windows-x64-releaseembedded.exe" ^
    "%DIST_DIR%\windows-x64\" >nul

set "EMBEDDED_BIN="

for %%F in ("%PROJECT_DIR%\Embedded-*") do (
    if exist "%%~fF" (
        set "EMBEDDED_BIN=%%~fF"
        goto :embedded_found
    )
)

if exist "%PROJECT_DIR%\embedded.exe" (
    set "EMBEDDED_BIN=%PROJECT_DIR%\embedded.exe"
    goto :embedded_found
)

if exist "%PROJECT_DIR%\embedded" (
    set "EMBEDDED_BIN=%PROJECT_DIR%\embedded"
    goto :embedded_found
)

echo Error: Embedded executable not found in:
echo   %PROJECT_DIR%
exit /b 1

:embedded_found

"%EMBEDDED_BIN%" ^
    "%PROJECT_DIR%\Assets.pak" ^
    "%PROJECT_DIR%\assets" ^
    "%PROJECT_DIR%\scripts" ^
    "%PROJECT_DIR%\std" ^
    "%PROJECT_DIR%\data" ^
    "%PROJECT_DIR%\config"

if errorlevel 1 (
    echo Error: Embedded executable failed.
    exit /b 1
)


copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\server\" >nul
copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\linux-x86_64\" >nul
copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\linux-arm64\" >nul
copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\macos-x86_64\" >nul
copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\macos-arm64\" >nul
copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\windows-x64\" >nul
copy /Y "%PROJECT_DIR%\Assets.pak" "%DIST_DIR%\windows-arm64\" >nul

exit /b 0