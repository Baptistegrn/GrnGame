@echo off
setlocal enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"

if "%~1"=="" (
    echo Usage: %~nx0 ^<name^> ^<destination^>
    exit /b 1
)
if "%~2"=="" (
    echo Usage: %~nx0 ^<name^> ^<destination^>
    exit /b 1
)

robocopy "%SCRIPT_DIR%..\project_model" "%~2\project_model" /E >nul
if errorlevel 8 (
    echo Error: copy failed.
    exit /b 1
)

ren "%~2\project_model" "%~1"