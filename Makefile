# ============================================================================
# JavaScript Compiler - Makefile
# 优化版构建脚本，支持模块化编译和增量构建
# ============================================================================

# 编译器配置
CC = gcc
RE2C = re2c
BISON = bison

# 目录配置
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
LEXER_DIR = $(SRC_DIR)/lexer
PARSER_DIR = $(SRC_DIR)/parser
AST_DIR = $(SRC_DIR)/ast
UTILS_DIR = $(SRC_DIR)/utils
TEST_DIR = tests

# 编译选项
CFLAGS = -Wall -Wextra -I$(INC_DIR) -std=c99 -O2
DEBUG_FLAGS = -g -DDEBUG
LDFLAGS =

# 生成文件
LEXER_GEN = $(BUILD_DIR)/lexer.c
PARSER_GEN_C = $(BUILD_DIR)/parser.c
PARSER_GEN_H = $(BUILD_DIR)/parser.h

# 源文件
LEXER_RE = $(LEXER_DIR)/lexer.re
PARSER_Y = $(PARSER_DIR)/parser.y
TOKEN_C = $(LEXER_DIR)/token.c
PARSER_ADAPTER_C = $(PARSER_DIR)/parser_adapter.c
AST_C = $(AST_DIR)/ast.c
UTILS_C = $(UTILS_DIR)/utils.c

# 目标文件
LEXER_OBJS = $(BUILD_DIR)/lexer.o $(BUILD_DIR)/token.o $(BUILD_DIR)/utils.o
PARSER_OBJS = $(BUILD_DIR)/parser.o $(BUILD_DIR)/parser_adapter.o \
              $(BUILD_DIR)/ast.o $(BUILD_DIR)/token.o $(BUILD_DIR)/utils.o

# 可执行文件
LEXER_EXE = js_lexer.exe
PARSER_EXE = js_parser.exe

# 测试文件
TEST_FILES = $(wildcard $(TEST_DIR)/test_*.js)

# ============================================================================
# 主目标
# ============================================================================

.PHONY: all clean lexer parser test-lexer test-parser help

all: parser

# 词法分析器
lexer: $(LEXER_EXE)

# 语法分析器（包含词法分析器）
parser: $(PARSER_EXE)

# ============================================================================
# 构建规则
# ============================================================================

# 创建构建目录
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# 生成词法分析器源码
$(LEXER_GEN): $(LEXER_RE) | $(BUILD_DIR)
	@echo "[RE2C] Generating lexer from $(LEXER_RE)..."
	$(RE2C) -o $@ $(LEXER_RE)

# 生成语法分析器源码
$(PARSER_GEN_C) $(PARSER_GEN_H): $(PARSER_Y) | $(BUILD_DIR)
	@echo "[BISON] Generating parser from $(PARSER_Y)..."
	$(BISON) -d -o $(PARSER_GEN_C) $(PARSER_Y)

# 编译词法分析器目标文件
$(BUILD_DIR)/lexer.o: $(LEXER_GEN) $(INC_DIR)/token.h
	@echo "[CC] Compiling lexer..."
	$(CC) $(CFLAGS) -c $(LEXER_GEN) -o $@

# 编译 Token 实现
$(BUILD_DIR)/token.o: $(TOKEN_C) $(INC_DIR)/token.h
	@echo "[CC] Compiling token..."
	$(CC) $(CFLAGS) -c $(TOKEN_C) -o $@

# 编译工具函数
$(BUILD_DIR)/utils.o: $(UTILS_C) $(INC_DIR)/utils.h
	@echo "[CC] Compiling utils..."
	$(CC) $(CFLAGS) -c $(UTILS_C) -o $@

# 编译语法分析器目标文件
$(BUILD_DIR)/parser.o: $(PARSER_GEN_C) $(PARSER_GEN_H) $(INC_DIR)/ast.h
	@echo "[CC] Compiling parser..."
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -c $(PARSER_GEN_C) -o $@

# 编译适配层
$(BUILD_DIR)/parser_adapter.o: $(PARSER_ADAPTER_C) $(PARSER_GEN_H) \
                                 $(INC_DIR)/token.h $(INC_DIR)/parser_adapter.h
	@echo "[CC] Compiling parser adapter..."
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -c $(PARSER_ADAPTER_C) -o $@

# 编译 AST 实现
$(BUILD_DIR)/ast.o: $(AST_C) $(INC_DIR)/ast.h
	@echo "[CC] Compiling AST..."
	$(CC) $(CFLAGS) -c $(AST_C) -o $@

# 链接词法分析器可执行文件
$(LEXER_EXE): main.c $(LEXER_OBJS)
	@echo "[LD] Linking lexer executable..."
	$(CC) $(CFLAGS) main.c $(LEXER_OBJS) -o $@ $(LDFLAGS)
	@echo "✓ Lexer built successfully: $(LEXER_EXE)"

# 链接语法分析器可执行文件
$(PARSER_EXE): parser_main.c $(PARSER_OBJS)
	@echo "[LD] Linking parser executable..."
	$(CC) $(CFLAGS) -I$(BUILD_DIR) parser_main.c $(PARSER_OBJS) -o $@ $(LDFLAGS)
	@echo "✓ Parser built successfully: $(PARSER_EXE)"

# ============================================================================
# 测试目标
# ============================================================================

# 测试词法分析器
test-lexer: $(LEXER_EXE)
	@echo "\n========== Testing Lexer =========="
	@for test in $(TEST_FILES); do \
		echo "\n--- Testing $$test ---"; \
		./$(LEXER_EXE) $$test || exit 1; \
	done
	@echo "\n✓ All lexer tests passed!"

# 测试语法分析器
test-parser: $(PARSER_EXE)
	@echo "\n========== Testing Parser =========="
	@passed=0; failed=0; \
	for test in $(TEST_FILES); do \
		echo -n "Testing $$test ... "; \
		if ./$(PARSER_EXE) $$test > /dev/null 2>&1; then \
			echo "✓ PASS"; \
			passed=$$((passed + 1)); \
		else \
			echo "✗ FAIL"; \
			failed=$$((failed + 1)); \
		fi; \
	done; \
	echo "\n========== Test Summary =========="; \
	echo "Passed: $$passed"; \
	echo "Failed: $$failed"; \
	if [ $$failed -eq 0 ]; then \
		echo "✓ All parser tests passed!"; \
	else \
		exit 1; \
	fi

# 详细测试（显示所有输出）
test-verbose: $(PARSER_EXE)
	@echo "\n========== Testing Parser (Verbose) =========="
	@for test in $(TEST_FILES); do \
		echo "\n========== Testing $$test =========="; \
		./$(PARSER_EXE) $$test; \
	done

# 测试 AST 输出
test-ast: $(PARSER_EXE)
	@echo "\n========== Testing AST Dump =========="
	@for test in $(TEST_FILES); do \
		echo "\n========== AST for $$test =========="; \
		./$(PARSER_EXE) --dump-ast $$test; \
	done

# ============================================================================
# 调试目标
# ============================================================================

# 调试模式构建
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all
	@echo "✓ Debug build complete"

# 只编译不链接（快速语法检查）
syntax-check:
	@echo "Checking syntax..."
	$(CC) $(CFLAGS) -fsyntax-only main.c parser_main.c

# ============================================================================
# 清理目标
# ============================================================================

# 清理所有生成文件
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(LEXER_EXE) $(PARSER_EXE)
	@rm -f *.o lexer.c parser.c parser.h
	@echo "✓ Clean complete"

# 只清理目标文件（保留可执行文件）
clean-obj:
	@echo "Cleaning object files..."
	@rm -rf $(BUILD_DIR)/*.o
	@echo "✓ Object files cleaned"

# ============================================================================
# 信息目标
# ============================================================================

# 显示帮助信息
help:
	@echo "JavaScript Compiler - Build System"
	@echo "===================================="
	@echo ""
	@echo "Targets:"
	@echo "  all          - Build parser (default)"
	@echo "  lexer        - Build lexer only"
	@echo "  parser       - Build parser (includes lexer)"
	@echo "  test-lexer   - Run lexer tests"
	@echo "  test-parser  - Run parser tests"
	@echo "  test-verbose - Run tests with full output"
	@echo "  test-ast     - Test AST generation"
	@echo "  debug        - Build with debug symbols"
	@echo "  clean        - Remove all generated files"
	@echo "  clean-obj    - Remove object files only"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make              # Build parser"
	@echo "  make test-parser  # Run all tests"
	@echo "  make clean all    # Clean rebuild"
	@echo ""

# 显示配置信息
info:
	@echo "Build Configuration:"
	@echo "===================="
	@echo "CC:       $(CC)"
	@echo "RE2C:     $(RE2C)"
	@echo "BISON:    $(BISON)"
	@echo "CFLAGS:   $(CFLAGS)"
	@echo "SRC_DIR:  $(SRC_DIR)"
	@echo "BUILD_DIR: $(BUILD_DIR)"
	@echo ""

# ============================================================================
# 依赖关系
# ============================================================================

# 自动生成依赖关系
-include $(BUILD_DIR)/*.d

# ============================================================================
# 特殊目标
# ============================================================================

.SUFFIXES:
.DELETE_ON_ERROR:
