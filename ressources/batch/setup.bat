@echo off

setlocal

set "SCRIPTS_DIR=%~dp0\scripts"

echo Adding to PATH:
echo   %SCRIPTS_DIR%

echo.

powershell -NoProfile -ExecutionPolicy Bypass -Command ^
    "$scripts = [Environment]::GetEnvironmentVariable('Path', 'User');" ^
    "if (($scripts -split ';') -notcontains '%SCRIPTS_DIR%') {" ^
    "    [Environment]::SetEnvironmentVariable('Path', ($scripts.TrimEnd(';') + ';%SCRIPTS_DIR%'), 'User')" ^
    "}"

echo.
echo Done.
echo.
echo Available commands:

for %%F in ("%SCRIPTS_DIR%\*.bat") do (
    echo   %%~nF
)

endlocal