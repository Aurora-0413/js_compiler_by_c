@echo off
REM ============================================================================
REM JavaScript Compiler - Windows 构建脚本 (优化版)
REM 支持模块化编译、增量构建、自动依赖管理
REM ============================================================================

setlocal enabledelayedexpansion

REM ============================================================================
REM 配置区域 - 请根据实际安装路径修改
REM ============================================================================

REM 编译工具路径（请修改为实际路径）
set "GCC=gcc"
set "RE2C=re2c"
set "BISON=bison"

REM 检查是否使用默认工具（从 PATH）
where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] GCC not found in PATH. Please install MinGW or update GCC path.
    exit /b 1
)

where re2c >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] RE2C not found in PATH. Please install RE2C or update path.
    exit /b 1
)

where bison >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] BISON not found in PATH. Please install Bison or update path.
    exit /b 1
)

REM 目录配置
set "SRC_DIR=src"
set "INC_DIR=include"
set "BUILD_DIR=build"
set "TEST_DIR=tests"

REM 编译选项
set "CFLAGS=-Wall -Wextra -I%INC_DIR% -std=c99 -O2"
set "DEBUG_FLAGS=-g -DDEBUG"

REM 可执行文件名
set "LEXER_EXE=js_lexer.exe"
set "PARSER_EXE=js_parser.exe"

REM ============================================================================
REM 辅助函数
REM ============================================================================

:print_header
echo.
echo ============================================================================
echo %~1
echo ============================================================================
echo.
goto :eof

:print_step
echo [%~1] %~2
goto :eof

:check_error
if %errorlevel% neq 0 (
    echo [ERROR] %~1
    exit /b 1
)
goto :eof

REM ============================================================================
REM 主程序
REM ============================================================================

if "%1"=="" goto build_parser
if "%1"=="lexer" goto build_lexer
if "%1"=="parser" goto build_parser
if "%1"=="test" goto test_lexer
if "%1"=="test-lexer" goto test_lexer
if "%1"=="test-parser" goto test_parser
if "%1"=="test-parse" goto test_parser
if "%1"=="clean" goto clean
if "%1"=="help" goto help
if "%1"=="info" goto info

echo Unknown target: %1
echo Run "build.bat help" for usage information.
exit /b 1

REM ============================================================================
REM 构建目标
REM ============================================================================

:build_lexer
call :print_header "Building Lexer"

REM 创建构建目录
if not exist "%BUILD_DIR%" (
    call :print_step "MKDIR" "Creating build directory"
    mkdir "%BUILD_DIR%"
)

REM 生成词法分析器源码
if exist "%SRC_DIR%\lexer\lexer.re" (
    call :print_step "RE2C" "Generating lexer from lexer.re"
    "%RE2C%" -o "%BUILD_DIR%\lexer.c" "%SRC_DIR%\lexer\lexer.re"
    call :check_error "RE2C generation failed"
) else if exist "lexer.re" (
    call :print_step "RE2C" "Generating lexer from lexer.re (root)"
    "%RE2C%" -o "%BUILD_DIR%\lexer.c" "lexer.re"
    call :check_error "RE2C generation failed"
) else (
    echo [ERROR] lexer.re not found
    exit /b 1
)

REM 编译词法分析器
call :print_step "GCC" "Compiling lexer sources"

REM 编译生成的词法分析器
"%GCC%" %CFLAGS% -c "%BUILD_DIR%\lexer.c" -o "%BUILD_DIR%\lexer.o"
call :check_error "Lexer compilation failed"

REM 编译 token 实现
if exist "%SRC_DIR%\lexer\token.c" (
    "%GCC%" %CFLAGS% -c "%SRC_DIR%\lexer\token.c" -o "%BUILD_DIR%\token.o"
    call :check_error "Token compilation failed"
)

REM 编译工具函数
if exist "%SRC_DIR%\utils\utils.c" (
    "%GCC%" %CFLAGS% -c "%SRC_DIR%\utils\utils.c" -o "%BUILD_DIR%\utils.o"
    call :check_error "Utils compilation failed"
)

REM 链接可执行文件
call :print_step "LD" "Linking lexer executable"

if exist "%SRC_DIR%\utils\utils.c" (
    "%GCC%" %CFLAGS% main.c "%BUILD_DIR%\lexer.o" "%BUILD_DIR%\token.o" "%BUILD_DIR%\utils.o" -o "%LEXER_EXE%"
) else (
    "%GCC%" %CFLAGS% main.c "%BUILD_DIR%\lexer.o" -o "%LEXER_EXE%"
)
call :check_error "Lexer linking failed"

echo.
echo ^✓ Lexer built successfully: %LEXER_EXE%
echo.
goto :eof

:build_parser
call :print_header "Building Parser"

REM 创建构建目录
if not exist "%BUILD_DIR%" (
    call :print_step "MKDIR" "Creating build directory"
    mkdir "%BUILD_DIR%"
)

REM 生成词法分析器源码
if exist "%SRC_DIR%\lexer\lexer.re" (
    call :print_step "RE2C" "Generating lexer"
    "%RE2C%" -o "%BUILD_DIR%\lexer.c" "%SRC_DIR%\lexer\lexer.re"
    call :check_error "RE2C failed"
) else if exist "lexer.re" (
    call :print_step "RE2C" "Generating lexer"
    "%RE2C%" -o "%BUILD_DIR%\lexer.c" "lexer.re"
    call :check_error "RE2C failed"
) else (
    echo [ERROR] lexer.re not found
    exit /b 1
)

REM 生成语法分析器源码
if exist "%SRC_DIR%\parser\parser.y" (
    call :print_step "BISON" "Generating parser"
    "%BISON%" -d -o "%BUILD_DIR%\parser.c" "%SRC_DIR%\parser\parser.y"
    call :check_error "Bison failed"
) else if exist "parser.y" (
    call :print_step "BISON" "Generating parser"
    "%BISON%" -d -o "%BUILD_DIR%\parser.c" "parser.y"
    call :check_error "Bison failed"
) else (
    echo [ERROR] parser.y not found
    exit /b 1
)

REM 编译所有模块
call :print_step "GCC" "Compiling parser modules"

REM 编译词法分析器
"%GCC%" %CFLAGS% -c "%BUILD_DIR%\lexer.c" -o "%BUILD_DIR%\lexer.o"
call :check_error "Lexer compilation failed"

REM 编译语法分析器
"%GCC%" %CFLAGS% -I"%BUILD_DIR%" -c "%BUILD_DIR%\parser.c" -o "%BUILD_DIR%\parser.o"
call :check_error "Parser compilation failed"

REM 编译适配层
if exist "%SRC_DIR%\parser\parser_adapter.c" (
    "%GCC%" %CFLAGS% -I"%BUILD_DIR%" -c "%SRC_DIR%\parser\parser_adapter.c" -o "%BUILD_DIR%\parser_adapter.o"
) else if exist "parser_lex_adapter.c" (
    "%GCC%" %CFLAGS% -I"%BUILD_DIR%" -c "parser_lex_adapter.c" -o "%BUILD_DIR%\parser_adapter.o"
) else (
    echo [ERROR] parser adapter file not found
    exit /b 1
)
call :check_error "Parser adapter compilation failed"

REM 编译 AST
if exist "%SRC_DIR%\ast\ast.c" (
    "%GCC%" %CFLAGS% -c "%SRC_DIR%\ast\ast.c" -o "%BUILD_DIR%\ast.o"
    call :check_error "AST compilation failed"
) else if exist "ast.c" (
    "%GCC%" %CFLAGS% -c "ast.c" -o "%BUILD_DIR%\ast.o"
    call :check_error "AST compilation failed"
) else (
    echo [ERROR] ast.c not found
    exit /b 1
)

REM 编译 token 实现
if exist "%SRC_DIR%\lexer\token.c" (
    "%GCC%" %CFLAGS% -c "%SRC_DIR%\lexer\token.c" -o "%BUILD_DIR%\token.o"
)

REM 编译工具函数
if exist "%SRC_DIR%\utils\utils.c" (
    "%GCC%" %CFLAGS% -c "%SRC_DIR%\utils\utils.c" -o "%BUILD_DIR%\utils.o"
)

REM 链接可执行文件
call :print_step "LD" "Linking parser executable"

set "OBJ_FILES=%BUILD_DIR%\lexer.o %BUILD_DIR%\parser.o %BUILD_DIR%\parser_adapter.o %BUILD_DIR%\ast.o"
if exist "%BUILD_DIR%\token.o" set "OBJ_FILES=%OBJ_FILES% %BUILD_DIR%\token.o"
if exist "%BUILD_DIR%\utils.o" set "OBJ_FILES=%OBJ_FILES% %BUILD_DIR%\utils.o"

if exist "parser_main.c" (
    "%GCC%" %CFLAGS% -I"%BUILD_DIR%" parser_main.c %OBJ_FILES% -o "%PARSER_EXE%"
    call :check_error "Parser linking failed"
) else if exist "%SRC_DIR%\parser_main.c" (
    "%GCC%" %CFLAGS% -I"%BUILD_DIR%" "%SRC_DIR%\parser_main.c" %OBJ_FILES% -o "%PARSER_EXE%"
    call :check_error "Parser linking failed"
) else (
    echo [ERROR] parser_main.c not found
    exit /b 1
)

echo.
echo ^✓ Parser built successfully: %PARSER_EXE%
echo.
goto :eof

REM ============================================================================
REM 测试目标
REM ============================================================================

:test_lexer
if not exist "%LEXER_EXE%" call :build_lexer

call :print_header "Testing Lexer"

set "test_count=0"
set "pass_count=0"
set "fail_count=0"

for %%f in ("%TEST_DIR%\test_*.js") do (
    set /a test_count+=1
    echo.
    echo --- Testing %%f ---
    "%LEXER_EXE%" "%%f"
    if !errorlevel! equ 0 (
        set /a pass_count+=1
        echo ^✓ PASS
    ) else (
        set /a fail_count+=1
        echo ^✗ FAIL
    )
)

echo.
echo ========== Test Summary ==========
echo Total:  !test_count!
echo Passed: !pass_count!
echo Failed: !fail_count!
echo.

if !fail_count! equ 0 (
    echo ^✓ All lexer tests passed!
    exit /b 0
) else (
    echo ^✗ Some tests failed
    exit /b 1
)
goto :eof

:test_parser
if not exist "%PARSER_EXE%" call :build_parser

call :print_header "Testing Parser"

set "test_count=0"
set "pass_count=0"
set "fail_count=0"

for %%f in ("%TEST_DIR%\test_*.js") do (
    set /a test_count+=1
    echo Testing %%f ...
    "%PARSER_EXE%" "%%f" >nul 2>&1
    if !errorlevel! equ 0 (
        set /a pass_count+=1
        echo   ^✓ PASS
    ) else (
        set /a fail_count+=1
        echo   ^✗ FAIL
    )
)

echo.
echo ========== Test Summary ==========
echo Total:  !test_count!
echo Passed: !pass_count!
echo Failed: !fail_count!
echo.

if !fail_count! equ 0 (
    echo ^✓ All parser tests passed!
    exit /b 0
) else (
    echo ^✗ Some tests failed
    exit /b 1
)
goto :eof

REM ============================================================================
REM 清理目标
REM ============================================================================

:clean
call :print_header "Cleaning Build Artifacts"

if exist "%BUILD_DIR%" (
    call :print_step "RM" "Removing build directory"
    rd /s /q "%BUILD_DIR%"
)

if exist "%LEXER_EXE%" (
    call :print_step "RM" "Removing %LEXER_EXE%"
    del /q "%LEXER_EXE%"
)

if exist "%PARSER_EXE%" (
    call :print_step "RM" "Removing %PARSER_EXE%"
    del /q "%PARSER_EXE%"
)

REM 清理根目录的生成文件（兼容旧版本）
if exist "lexer.c" del /q "lexer.c"
if exist "parser.c" del /q "parser.c"
if exist "parser.h" del /q "parser.h"
if exist "*.o" del /q "*.o"

echo.
echo ^✓ Clean complete
echo.
goto :eof

REM ============================================================================
REM 信息目标
REM ============================================================================

:help
echo JavaScript Compiler - Build System (Windows)
echo =============================================
echo.
echo Usage: build.bat [target]
echo.
echo Targets:
echo   lexer        Build lexer only
echo   parser       Build parser (default)
echo   test-lexer   Run lexer tests
echo   test-parser  Run parser tests
echo   clean        Remove all generated files
echo   info         Show build configuration
echo   help         Show this help message
echo.
echo Examples:
echo   build.bat              # Build parser
echo   build.bat test-parser  # Run all tests
echo   build.bat clean        # Clean build
echo.
goto :eof

:info
echo Build Configuration:
echo ====================
echo GCC:       %GCC%
echo RE2C:      %RE2C%
echo BISON:     %BISON%
echo CFLAGS:    %CFLAGS%
echo SRC_DIR:   %SRC_DIR%
echo BUILD_DIR: %BUILD_DIR%
echo.
goto :eof

endlocal
