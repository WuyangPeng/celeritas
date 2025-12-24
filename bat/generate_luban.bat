@echo off
setlocal enabledelayedexpansion

:: Set path variables
set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
set WORKSPACE=%SCRIPT_DIR%\

set LUBAN_EXE="%WORKSPACE%\..\deps\luban\bin\Luban.exe"
set CONF_FILE="%WORKSPACE%\..\data\config\luban\luban.conf"
set OUTPUT_CODE_DIR="%WORKSPACE%\..\src\config\luban\generated"
set OUTPUT_DATA_DIR="%WORKSPACE%\..\config\bin"

:: Create output directories
if not exist "%OUTPUT_CODE_DIR%" mkdir "%OUTPUT_CODE_DIR%"
if not exist "%OUTPUT_DATA_DIR%" mkdir "%OUTPUT_DATA_DIR%"

echo [Luban] Starting code and data generation...

:: Execute generation command
:: -t server: Server
:: -c cpp-bin: Generate C++ binary parsing code
:: -d bin: Generate binary data files
"%LUBAN_EXE%" ^
    -t server ^
    -c cpp-sharedptr-bin ^
    -d bin ^
    --conf "%CONF_FILE%" ^
    -x outputCodeDir="%OUTPUT_CODE_DIR%" ^
    -x outputDataDir="%OUTPUT_DATA_DIR%"

if %ERRORLEVEL% NEQ 0 (
    echo [Error] Luban generation failed! Please check Excel configuration or dotnet environment.
    pause
    exit /b %ERRORLEVEL%
)

echo [Success] Luban generation task completed!
pause