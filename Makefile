# JavaScript 词法分析器 Makefile
# 适用于 Windows + MinGW 环境

CC = gcc
CFLAGS = -Wall -g -std=c99
RE2C = re2c

# 目标可执行文件
TARGET = js_lexer.exe

# 源文件
SOURCES = main.c lexer.c
OBJECTS = $(SOURCES:.c=.o)

# 默认目标
all: $(TARGET)

# 生成词法分析器 C 代码
lexer.c: lexer.re
	@echo "Generating lexer.c from lexer.re..."
	$(RE2C) -o lexer.c lexer.re

# 编译目标程序
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete: $(TARGET)"

# 编译 .c 文件为 .o 文件
%.o: %.c token.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	@echo "Cleaning up..."
	@if exist lexer.c del /Q lexer.c
	@if exist *.o del /Q *.o
	@if exist $(TARGET) del /Q $(TARGET)
	@echo "Clean complete"

# 创建测试目录和示例文件
test-setup:
	@if not exist tests mkdir tests
	@echo var x = 10; > tests\test_basic.js
	@echo let y = 20; >> tests\test_basic.js
	@echo const z = 30; >> tests\test_basic.js
	@echo function add(a, b) { return a + b; } >> tests\test_basic.js
	@echo console.log("Test complete"); >> tests\test_basic.js
	@echo Test files created in tests\ directory

# 运行测试
test: $(TARGET) test-setup
	@echo.
	@echo === Running Test: tests\test_basic.js ===
	@.\$(TARGET) tests\test_basic.js
	@echo.

# 帮助信息
help:
	@echo JavaScript Lexer - Makefile Commands:
	@echo   make          - Build the lexer
	@echo   make all      - Same as 'make'
	@echo   make clean    - Remove generated files
	@echo   make test     - Build and run tests
	@echo   make help     - Show this help message
	@echo.
	@echo Usage:
	@echo   .\js_lexer.exe filename.js

.PHONY: all clean test test-setup help
