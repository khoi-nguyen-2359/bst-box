@echo off
setlocal enabledelayedexpansion

set BUILD_FLAGS=-g
set BUILD_TYPE=_debug

for %%i in (%*) do (
    if "%%i"=="release" (
        set BUILD_FLAGS=-O3
        set BUILD_TYPE=
    ) else if "%%i"=="loggable" (
        set BUILD_FLAGS=!BUILD_FLAGS! -DLOGGABLE
    )
)

set OUTPUT_FILE=build\bstbox%BUILD_TYPE%_windows.exe

if not exist build mkdir build

g++ -std=c++11 ^
    %BUILD_FLAGS% ^
    bstbox\source\*.cpp tree\source\*.cpp log\source\*.c ^
    -Itree\include -Ilog\include ^
    -o %OUTPUT_FILE%

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded! Output: %OUTPUT_FILE%
)