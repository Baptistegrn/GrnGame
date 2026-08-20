@echo off
setlocal enabledelayedexpansion

if exist dist rmdir /s /q dist
mkdir dist\server

set PLATFORMS=linux-x64 linux-arm64 macos-x64 macos-arm64 windows-x64 windows-arm64

for %%P in (%PLATFORMS%) do (
    mkdir "dist\%%P"
    copy "runtime\*%%P*" "dist\%%P\" >nul 2>&1
)

set EMBEDDED_BIN=
for %%F in (Embedded*.exe) do set EMBEDDED_BIN=%%F
if "!EMBEDDED_BIN!"=="" set EMBEDDED_BIN=embedded.exe

"!EMBEDDED_BIN!" Assets.pak assets scripts std data config

for %%P in (%PLATFORMS%) do (
    copy Assets.pak "dist\%%P\" >nul
)

copy server.py dist\server\ >nul
copy Assets.pak dist\server\ >nul

copy *.js dist\server\ >nul 2>&1
copy *.wasm dist\server\ >nul 2>&1
copy *.html dist\server\ >nul 2>&1

pause
