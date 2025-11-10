# JavaScript 编译器 - 构建指南

本项目包含两个可执行程序：

- **js_lexer.exe** - 词法分析器（Token 流输出，用于调试）
- **js_parser.exe** - 语法分析器（语法验证，报告成功/失败）

## 构建项目

### Windows (使用 build.bat)

```bash
# 构建词法分析器 (js_lexer.exe)
.\build.bat

# 构建语法分析器 (js_parser.exe)
.\build.bat parser

# 构建词法分析器并运行测试
.\build.bat test

# 构建语法分析器并运行测试
.\build.bat test-parse

# 清理所有生成文件
.\build.bat clean

# 显示帮助信息
.\build.bat help
```

### 使用 Makefile (MSYS2 或 Linux)

```bash
# 构建词法分析器 (js_lexer.exe)
make

# 构建语法分析器 (js_parser.exe)
make parser

# 运行词法分析器测试
make test-lex

# 运行语法分析器测试
make test-parse

# 清理所有生成文件
make clean

# 显示帮助信息
make help
```

## 运行程序

### 词法分析器 (js_lexer.exe)

输出 Token 流，用于调试词法分析过程。

```bash
# 分析 JavaScript 文件
.\js_lexer.exe tests\test_basic.js

# 或使用任何 JS 文件
.\js_lexer.exe your_script.js
```

**示例输出：**

```text
=== Lexical Analysis of 'tests\test_basic.js' ===

[  1] Line   2, Col   1: VAR             = 'var'
[  2] Line   2, Col   5: IDENTIFIER      = 'x'
[  3] Line   2, Col   7: =
[  4] Line   2, Col   9: NUMBER          = '10'
[  5] Line   2, Col  11: ;
[  6] Line   3, Col   1: LET             = 'let'
...

=== Analysis Complete ===
Total tokens: 106
```

### 语法分析器 (js_parser.exe)

验证 JavaScript 语法，报告成功或详细错误信息。

```bash
# 分析 JavaScript 文件
.\js_parser.exe tests\test_basic.js

# 测试语法正确的文件
.\js_parser.exe tests\test_simple.js

# 测试语法错误的文件
.\js_parser.exe tests\test_error_object.js
```

**成功示例：**

```text
Parsing successful! Input file: tests\test_basic.js
```

**错误示例：**

```text
Parsing failed. Input file: tests\test_error_object.js
Syntax error: syntax error, unexpected STRING, expecting ':'
```

## 已实现的功能

### ✅ 词法分析 (Lexer)

**关键字识别：**

- 变量声明: `var`, `let`, `const`
- 函数: `function`, `return`
- 控制流: `if`, `else`, `for`, `while`, `do`, `switch`, `case`, `default`
- 异常处理: `try`, `catch`, `finally`, `throw`
- 其他: `break`, `continue`, `new`, `this`, `typeof`, `delete`, `in`, `instanceof`, `void`, `with`, `debugger`

**字面量识别：**

- 数字: 整数、浮点数、科学计数法、十六进制
- 字符串: 单引号和双引号字符串，支持转义字符
- 布尔值: `true`, `false`
- 特殊值: `null`, `undefined`

**运算符：**

- 算术: `+`, `-`, `*`, `/`, `%`, `++`, `--`
- 比较: `==`, `!=`, `===`, `!==`, `<`, `>`, `<=`, `>=`
- 逻辑: `&&`, `||`, `!`
- 位运算: `&`, `|`, `^`, `~`, `<<`, `>>`, `>>>`
- 赋值: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`, `>>>=`
- 三元: `?`, `:`

**分隔符：**

- 括号: `(`, `)`, `{`, `}`, `[`, `]`
- 其他: `;`, `,`, `.`

**其他功能：**

- 单行注释 (`//`) 和多行注释 (`/* */`)
- 行号和列号精确跟踪（用于错误报告）
- 换行标记 `has_newline`（为 ASI 机制预留）

### ✅ 语法分析 (Parser)

**支持的语句：**

- 变量声明（`var`, `let`, `const`）
- 表达式语句
- 块语句 `{ ... }`
- if-else 条件语句
- for 循环（经典三段式）
- 函数声明
- return 语句
- 空语句 `;`

**支持的表达式：**

- 字面量（数字、字符串、布尔值）
- 标识符
- 一元运算（`+`, `-`, `!`, `~`）
- 二元运算（算术、比较、逻辑）
- 赋值运算 `=`
- 成员访问 `obj.prop`
- 函数调用 `func(a, b)`
- 数组字面量 `[1, 2, 3]`
- 对象字面量 `{ key: value }`
- 括号分组 `(expr)`

**错误检测能力：**

- 详细的语法错误信息（包含期待的 token 和实际遇到的 token）
- 准确的错误位置报告
- Bison 详细错误输出（`%define parse.error verbose`）

**歧义消解：**

- 正确区分块 `{...}` 和对象字面量 `{...}`
- 使用 `expr_no_obj` 变体避免表达式语句歧义

## 编译警告说明

构建过程中会出现以下警告，这些警告**不影响功能**：

### 1. re2c sentinel 警告

```text
lexer.re:82:20: warning: sentinel symbol 0 occurs in the middle of the rule
```

- **影响：** 无影响，仅是 re2c 的优化建议
- **原因：** re2c 检测到字符串结束符在规则中间
- **可忽略**

### 2. Bison 冲突警告

```text
parser.y: 警告: 2 项偏移/归约冲突 [-Wconflicts-sr]
```

- **影响：** 无影响，Bison 使用默认规则解决冲突
- **原因：** 表达式语法的自然歧义（如 if-else 的悬挂 else）
- **可忽略**（或使用 `%expect 2` 指令消除警告）

### 3. 未使用变量警告

```text
lexer.re:89:25: warning: unused variable 'comment_start'
```

- **影响：** 无影响
- **建议：** 可以删除未使用的变量

## 下一步开发计划

### 优先级 P1 - ASI 机制

- [ ] 实现自动分号插入（Automatic Semicolon Insertion）
- [ ] 利用已有的 `has_newline` 标记
- [ ] 支持受限产生式（`return`/`break`/`continue`/`throw` 后的换行）

### 优先级 P2 - AST 构建

- [ ] 在 parser.y 的语义动作中创建 AST 节点
- [ ] 实现 AST 节点的打印函数
- [ ] 实现 AST 节点的内存管理

### 优先级 P3 - 扩展语句覆盖

- [ ] while 循环
- [ ] do-while 循环
- [ ] switch-case 语句
- [ ] try-catch-finally 异常处理

### 优先级 P4 - 完整运算符支持

- [ ] 三元运算符 `? :`
- [ ] 完整的位运算支持
- [ ] 复合赋值运算符

### 优先级 P5 - 高级特性

- [ ] 正则表达式字面量（需要上下文感知）
- [ ] ES6+ 特性（箭头函数、模板字符串等）

## 测试文件

项目包含以下测试用例：

- `tests/test_basic.js` - 综合基本语法测试 ✅
- `tests/test_simple.js` - 简单功能测试 ✅
- `tests/test_error_missing_semicolon.js` - 缺少分号错误测试 ✅
- `tests/test_error_object.js` - 对象字面量错误测试 ✅
- `tests/test_error_cases.js` - 错误用例集合（需逐个激活测试）

## 技术细节

- **词法分析器生成器**: re2c 3.0+
- **语法分析器生成器**: Bison 3.x
- **编译器**: GCC (MinGW)
- **C 标准**: C99
- **构建系统**: build.bat (Windows) / Makefile (MSYS2/Linux)

## 项目目录结构

```text
js_compiler_by_c/
├── token.h                   # Token 类型定义和结构体
├── lexer.re                  # re2c 词法分析器源文件（374 行）
├── lexer.c                   # re2c 生成的 C 代码（自动生成，勿编辑）
├── main.c                    # 词法分析器主程序
├── parser.y                  # Bison 语法分析器源文件（317 行）
├── parser.c / parser.h       # Bison 生成的 C 代码（自动生成，勿编辑）
├── parser_main.c             # 语法分析器主程序
├── parser_lex_adapter.c      # 词法-语法适配层
├── build.bat                 # Windows 构建脚本
├── Makefile                  # Make 构建脚本
├── BUILD.md                  # 本文档
├── TEST_REPORT.md            # 测试报告
├── .github/
│   └── copilot-instructions.md  # AI 编码助手指南
└── tests/                    # 测试用例目录
    ├── test_basic.js
    ├── test_simple.js
    ├── test_error_missing_semicolon.js
    ├── test_error_object.js
    └── test_error_cases.js
```

## 常见问题

### Q: 为什么需要两个可执行程序？

**A:** 双可执行程序设计便于调试：

- `js_lexer.exe` 输出所有 Token，方便查看词法分析结果
- `js_parser.exe` 专注语法验证，提供清晰的成功/失败反馈

### Q: 构建时的警告是否需要修复？

**A:** 不是必须的。这些警告不影响程序功能：

- re2c 和 Bison 的警告是优化建议
- 未使用变量警告可以修复但不影响运行

### Q: 如何添加新的测试用例？

**A:** 在 `tests/` 目录创建新的 `.js` 文件，然后运行：

```bash
.\js_parser.exe tests\your_test.js
```

### Q: 为什么使用 re2c 而不是 Flex？

**A:** JavaScript 标识符基于 Unicode 定义（ECMAScript 规范），re2c 原生支持 Unicode 字符类，而 Flex 不支持。

### Q: 当前是否支持 ES6+ 语法？

**A:** 不支持。当前仅支持 ES5 基本语法：

- ❌ 不支持：箭头函数、类、async/await、模板字符串、解构等
- ✅ 支持：var/let/const、函数、if-else、for 循环、对象/数组字面量

## 参考资源

- **ECMAScript 5.1 规范**: https://262.ecma-international.org/5.1/
- **ASI 规则详解**: ECMAScript 规范 11.9 节
- **re2c 手册**: https://re2c.org/manual/manual_c.html
- **Bison 手册**: https://www.gnu.org/software/bison/manual/
- **参考项目**: https://github.com/sunxfancy/flex-bison-examples

---

**最后更新**: 2025 年 11 月 10 日
