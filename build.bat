@echo off
REM JavaScript 词法分析器构建脚本 (Windows)

set GCC=D:\mingw64\bin\gcc.exe
set RE2C=re2c
set TARGET=js_lexer.exe

echo ================================================
echo JavaScript Lexer - Build Script
echo ================================================
echo.

REM 检查参数
if "%1"=="clean" goto clean
if "%1"=="test" goto test
if "%1"=="help" goto help

:build
echo [1/3] Generating lexer.c from lexer.re...
%RE2C% -o lexer.c lexer.re
if errorlevel 1 (
    echo ERROR: re2c failed!
    exit /b 1
)
echo       Generated lexer.c successfully.
echo.

echo [2/3] Compiling source files...
%GCC% -Wall -g -std=c99 -c main.c -o main.o
if errorlevel 1 (
    echo ERROR: Failed to compile main.c
    exit /b 1
)
%GCC% -Wall -g -std=c99 -c lexer.c -o lexer.o
if errorlevel 1 (
    echo ERROR: Failed to compile lexer.c
    exit /b 1
)
echo       Compiled successfully.
echo.

echo [3/3] Linking...
%GCC% -Wall -g -std=c99 -o %TARGET% main.o lexer.o
if errorlevel 1 (
    echo ERROR: Linking failed!
    exit /b 1
)
echo       Linked successfully.
echo.

echo ================================================
echo Build complete: %TARGET%
echo ================================================
echo.
echo Usage: %TARGET% ^<javascript_file^>
echo Example: %TARGET% tests\test_basic.js
echo.
goto end

:clean
echo Cleaning up...
if exist lexer.c del /Q lexer.c
if exist *.o del /Q *.o
if exist %TARGET% del /Q %TARGET%
echo Clean complete.
goto end

:test
call :build
if errorlevel 1 goto end

echo.
echo ================================================
echo Running Test
echo ================================================
echo.
if not exist tests\test_basic.js (
    echo ERROR: Test file tests\test_basic.js not found!
    exit /b 1
)
%TARGET% tests\test_basic.js
goto end

:help
echo Usage: build.bat [command]
echo.
echo Commands:
echo   (none)  - Build the lexer
echo   clean   - Remove generated files
echo   test    - Build and run tests
echo   help    - Show this help message
echo.
goto end

:end
