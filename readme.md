# JavaScript 编译器 (js_compiler_by_c)

> **基于 C 语言的 ECMAScript 5.1 编译器前端实现**  
> 支持词法分析、语法分析、AST 生成与自动分号插入（ASI）

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Version](https://img.shields.io/badge/Version-2.0.0-green.svg)](https://github.com/Aurora-0413/js_compiler_by_c)

---

## 📋 目录

- [项目简介](#项目简介)
- [核心特性](#核心特性)
- [快速开始](#快速开始)
- [项目结构](#项目结构)
- [详细使用指南](#详细使用指南)
- [开发指南](#开发指南)
- [测试](#测试)
- [常见问题](#常见问题)
- [参考资料](#参考资料)

---

## 🎯 项目简介

`js_compiler_by_c` 是一个使用 C 语言实现的 JavaScript 编译器前端，严格遵循 **ECMAScript 5.1** 规范。项目提供完整的词法分析、语法分析、AST 构建和自动分号插入功能，适合用于学习编译原理、理解 JavaScript 语法或作为更大项目的基础。

### 技术栈

| 组件       | 技术         | 版本要求 |
| ---------- | ------------ | -------- |
| 词法生成器 | re2c         | ≥ 3.0    |
| 语法生成器 | GNU Bison    | ≥ 3.0    |
| 编译器     | GCC          | ≥ 7.0    |
| 构建工具   | Make / Batch | -        |

---

## ⚡ 核心特性

### 🔍 词法分析器 (`js_lexer.exe`)
- ✅ 识别 27 个 ES5 关键字
- ✅ 支持 73+ 种运算符和分隔符
- ✅ 完整的数字字面量支持（整数、浮点、科学计数法、十六进制）
- ✅ 字符串字面量（单/双引号，转义字符）
- ✅ 单行/多行注释处理
- ✅ 精确的行号/列号跟踪

### 📊 语法分析器 (`js_parser.exe`)
- ✅ 完整的语句支持
  - 控制流：`if/else`, `for`, `while`, `do-while`, `switch`
  - 异常处理：`try-catch-finally`, `throw`
  - 其他：`with`, `break`, `continue`, `return`
- ✅ 表达式解析
  - 算术、逻辑、位运算、三元、逗号表达式
  - 函数调用、成员访问
  - 对象/数组字面量
- ✅ AST 构建与可视化输出
- ✅ 自动分号插入 (ASI)
  - 严格遵循 ECMA-262 11.9 节规范
  - 支持换行触发、EOF、受限产生式

### 🌲 AST 框架
- ✅ 统一的节点构造/打印/遍历/释放接口
- ✅ 支持缩进输出调试
- ✅ 内存安全保障

---

## 🚀 快速开始

### 环境准备

#### Windows 环境

1. **安装 MSYS2**
   - 下载：https://www.msys2.org/
   - 安装后打开 "MSYS2 MinGW 64-bit" 终端

2. **安装工具链**
   ```bash
   # 更新包管理器
   pacman -Syu
   
   # 安装编译工具
   pacman -S mingw-w64-x86_64-gcc re2c bison make
   ```

3. **验证安装**
   ```bash
   gcc --version
   re2c --version
   bison --version
   ```

#### Linux / macOS 环境

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install gcc re2c bison make

# Fedora/RHEL
sudo dnf install gcc re2c bison make

# macOS (需要 Homebrew)
brew install gcc re2c bison make
```

### 克隆项目

```bash
git clone https://github.com/Aurora-0413/js_compiler_by_c.git
cd js_compiler_by_c
```

### 构建项目

#### Windows (PowerShell)

```powershell
# 构建语法分析器（包含词法分析器）
.\build.bat parser

# 构建词法分析器（单独）
.\build.bat lexer

# 运行测试
.\build.bat test-parser

# 清理生成文件
.\build.bat clean

# 查看帮助
.\build.bat help
```

#### Linux / macOS

```bash
# 构建语法分析器
make parser

# 构建词法分析器
make lexer

# 运行测试
make test-parser

# 清理
make clean
```

### 基本使用

#### 1. 词法分析

```bash
# Windows
.\js_lexer.exe tests\test_basic.js

# Linux/macOS
./js_lexer.exe tests/test_basic.js
```

**输出示例：**
```
=== Lexical Analysis of 'tests\test_basic.js' ===

[  1] Line   1, Col   1: VAR         = 'var'
[  2] Line   1, Col   5: IDENTIFIER  = 'x'
[  3] Line   1, Col   7: =
[  4] Line   1, Col   9: NUMBER      = '10'
[  5] Line   1, Col  11: ;
```

#### 2. 语法分析

```bash
# Windows
.\js_parser.exe tests\test_basic.js

# Linux/macOS
./js_parser.exe tests/test_basic.js
```

**输出示例：**
```
[PASS] tests\test_basic.js - no syntax errors detected.
```

#### 3. 查看 AST

```bash
# Windows
.\js_parser.exe --dump-ast tests\test_basic.js

# Linux/macOS
./js_parser.exe --dump-ast tests/test_basic.js
```

**输出示例：**
```
=== AST Dump ===
Program
  VariableDeclaration (var)
    Declarator
      name: "x"
      init: Literal(10)
  IfStatement
    test: BinaryExpression(>)
      left: Identifier(x)
      right: Literal(0)
    consequent: BlockStatement
      ...
```

---

## 📁 项目结构

```
js_compiler_by_c/
├── include/                 # 公共头文件
│   ├── token.h             # Token 定义
│   ├── ast.h               # AST 定义
│   ├── utils.h             # 工具函数
│   └── parser_adapter.h    # 解析器适配层
│
├── src/                     # 源代码
│   ├── lexer/              # 词法分析器模块
│   │   ├── lexer.re        # re2c 词法规则
│   │   └── token.c         # Token 实现
│   ├── parser/             # 语法分析器模块
│   │   ├── parser.y        # Bison 语法规则
│   │   └── parser_adapter.c # 适配层实现
│   ├── ast/                # AST 模块
│   │   └── ast.c           # AST 实现
│   └── utils/              # 工具模块
│       └── utils.c         # 工具函数实现
│
├── build/                   # 构建输出（自动生成）
│   ├── lexer.c             # 生成的词法分析器
│   ├── parser.c            # 生成的语法分析器
│   └── *.o                 # 目标文件
│
├── tests/                   # 测试用例
│   ├── test_basic.js       # 基础语法测试
│   ├── test_functions.js   # 函数测试
│   ├── test_for_loops.js   # 循环测试
│   ├── test_asi_*.js       # ASI 测试
│   └── test_error_*.js     # 错误检测测试
│
├── docs/                    # 项目文档
│   ├── BUILD.md            # 构建指南
│   ├── TEST_REPORT.md      # 测试报告
│   ├── asi_implementation.md # ASI 实现细节
│   └── ...
│
├── main.c                   # 词法分析器入口
├── parser_main.c            # 语法分析器入口
├── lexer.re                 # 词法规则（兼容旧版）
├── parser.y                 # 语法规则（兼容旧版）
├── parser_lex_adapter.c     # 适配层（兼容旧版）
├── Makefile                 # 构建脚本
├── build.bat                # Windows 构建脚本
└── README.md                # 本文档
```

---

## 📖 详细使用指南

### 词法分析器详解

#### 支持的 Token 类型

**关键字（27个）：**
```javascript
var, let, const, function, return, if, else, for, while, do,
switch, case, default, break, continue, try, catch, finally,
throw, with, typeof, delete, void, new, this, in, instanceof
```

**运算符（73+）：**
- 算术：`+`, `-`, `*`, `/`, `%`, `++`, `--`
- 比较：`<`, `>`, `<=`, `>=`, `==`, `!=`, `===`, `!==`
- 逻辑：`&&`, `||`, `!`
- 位运算：`&`, `|`, `^`, `~`, `<<`, `>>`, `>>>`
- 赋值：`=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`, `>>>=`
- 其他：`?`, `:`, `.`, `,`, `;`, `(`, `)`, `{`, `}`, `[`, `]`

**字面量：**
- 数字：`123`, `3.14`, `1e5`, `0xFF`
- 字符串：`"hello"`, `'world'`
- 布尔值：`true`, `false`
- 特殊值：`null`, `undefined`

#### 使用示例

```bash
# 分析单个文件
.\js_lexer.exe mycode.js

# 分析测试文件
.\js_lexer.exe tests\test_literals.js
```

### 语法分析器详解

#### 支持的语句

| 类型           | 语法示例                                        | 说明               |
| -------------- | ----------------------------------------------- | ------------------ |
| 变量声明       | `var x = 10;`                                   | 支持 var/let/const |
| 函数声明       | `function foo(a, b) { return a + b; }`          | 命名函数           |
| if-else        | `if (x > 0) { ... } else { ... }`               | 条件分支           |
| for 循环       | `for (var i = 0; i < 10; i++) { ... }`          | 标准 for 循环      |
| for-in         | `for (var key in obj) { ... }`                  | 对象遍历           |
| while          | `while (x > 0) { x--; }`                        | 前置条件循环       |
| do-while       | `do { x++; } while (x < 10);`                   | 后置条件循环       |
| switch         | `switch (x) { case 1: break; default: }`        | 多分支选择         |
| try-catch      | `try { ... } catch (e) { ... } finally { ... }` | 异常处理           |
| throw          | `throw new Error("message");`                   | 抛出异常           |
| with           | `with (obj) { ... }`                            | 作用域扩展         |
| break/continue | `break label;` / `continue;`                    | 流程控制           |
| return         | `return x + y;`                                 | 函数返回           |
| 标签语句       | `loop: for (...) { break loop; }`               | 标签跳转           |

#### 使用示例

```bash
# 语法检查
.\js_parser.exe mycode.js

# 查看 AST
.\js_parser.exe --dump-ast mycode.js

# 批量测试
.\js_parser.exe tests\test_*.js
```

### 自动分号插入 (ASI)

ASI 遵循 ECMAScript 5.1 规范的 11.9 节，在以下情况自动插入分号：

#### 1. 换行触发
```javascript
var a = 1
var b = 2  // 等价于 var a = 1; var b = 2;
```

#### 2. 文件结束
```javascript
var x = 10  // 等价于 var x = 10;
// EOF
```

#### 3. 受限产生式
```javascript
return
42  // 等价于 return; 42;

throw
new Error()  // 等价于 throw; new Error()
```

#### ASI 测试

```bash
# 测试 ASI 基础功能
.\js_parser.exe tests\test_asi_basic.js

# 测试受限产生式
.\js_parser.exe tests\test_asi_return.js

# 测试控制语句
.\js_parser.exe tests\test_asi_control.js
```

---

## 🛠️ 开发指南

### 添加新的语法特性

#### 步骤 1：更新词法分析器

编辑 `src/lexer/lexer.re` 或 `lexer.re`：

```c
// 添加新关键字
"newkeyword" { 
    lexer->column += 10;
    lexer->prev_tok_state = PREV_TOK_CAN_REGEX;
    return make_token(TOK_NEW_KEYWORD, token_start, lexer->cursor, 
                     token_line, token_column);
}
```

在 `include/token.h` 中添加 Token 类型：
```c
typedef enum {
    // ...
    TOK_NEW_KEYWORD,
    // ...
} TokenType;
```

#### 步骤 2：更新语法分析器

编辑 `src/parser/parser.y` 或 `parser.y`：

```yacc
%token NEW_KEYWORD

%%

new_statement
    : NEW_KEYWORD '(' expr ')' block_statement
        { $$ = ast_make_new_stmt($3, $5); }
    ;
```

#### 步骤 3：更新 AST

在 `include/ast.h` 中添加节点类型：
```c
typedef enum {
    // ...
    AST_NEW_STMT,
    // ...
} ASTNodeType;

ASTNode* ast_make_new_stmt(ASTNode* test, ASTNode* body);
```

在 `src/ast/ast.c` 中实现：
```c
ASTNode* ast_make_new_stmt(ASTNode* test, ASTNode* body) {
    ASTNode* node = ast_alloc(AST_NEW_STMT);
    node->data.new_stmt.test = test;
    node->data.new_stmt.body = body;
    return node;
}
```

#### 步骤 4：重新构建

```bash
# Windows
.\build.bat clean
.\build.bat parser

# Linux/macOS
make clean
make parser
```

### 调试技巧

#### 1. 查看 Token 流
```bash
.\js_lexer.exe myfile.js > tokens.txt
```

#### 2. 查看详细 AST
```bash
.\js_parser.exe --dump-ast myfile.js > ast.txt
```

#### 3. 启用 Bison 调试
编辑 `parser_main.c`：
```c
extern int yydebug;
yydebug = 1;  // 启用调试
```

#### 4. 使用调试工具
```c
// 在代码中添加调试输出
#include "utils.h"

debug_print("Current token: %s", token_type_to_string(token.type));
```

---

## 🧪 测试

### 测试覆盖

项目包含 16+ 个测试用例，覆盖：

| 类别     | 测试文件                             | 说明                 |
| -------- | ------------------------------------ | -------------------- |
| 基础语法 | `test_basic.js`                      | 综合语法测试         |
| 函数     | `test_functions.js`                  | 函数声明、调用、嵌套 |
| 循环     | `test_for_loops.js`, `test_while.js` | 各种循环结构         |
| 字面量   | `test_literals.js`                   | 对象、数组字面量     |
| ASI      | `test_asi_*.js`                      | 自动分号插入         |
| 控制流   | `test_switch.js`, `test_try.js`      | switch、try-catch    |
| 运算符   | `test_operators.js`                  | 运算符优先级         |
| 错误检测 | `test_error_*.js`                    | 语法错误检测         |

### 运行测试

#### 完整测试套件

```bash
# Windows
.\build.bat test-parser

# Linux/macOS
make test-parser
```

**输出示例：**
```
========== Testing Parser ==========
Testing tests\test_basic.js ... ✓ PASS
Testing tests\test_functions.js ... ✓ PASS
Testing tests\test_for_loops.js ... ✓ PASS
...
========== Test Summary ==========
Passed: 16
Failed: 0
✓ All parser tests passed!
```

#### 单个测试

```bash
# 测试成功案例
.\js_parser.exe tests\test_basic.js

# 测试错误检测
.\js_parser.exe tests\test_error_object.js
```

#### 详细测试

```bash
# Windows
.\build.bat test-verbose

# Linux/macOS
make test-verbose
```

### 添加新测试

1. 在 `tests/` 目录创建新文件：
```bash
# 创建 tests/test_new_feature.js
```

2. 编写测试代码：
```javascript
// 测试新功能
function testNewFeature() {
    var x = 10;
    return x;
}
```

3. 运行测试：
```bash
.\js_parser.exe tests\test_new_feature.js
```

---

## ❓ 常见问题

### Q: 为什么需要两个可执行程序？

**A:** 分层设计便于调试：
- `js_lexer.exe` - 专注词法分析，输出 Token 流
- `js_parser.exe` - 完整的语法分析和 AST 构建

### Q: 支持 ES6+ 语法吗？

**A:** 目前仅支持 **ES5** 语法。不支持：
- ❌ 箭头函数
- ❌ 类（class）
- ❌ 模板字符串
- ❌ 解构赋值
- ❌ async/await
- ❌ Promise

### Q: 构建时出现警告怎么办？

**A:** 以下警告可以忽略：
- re2c sentinel 警告 - 优化建议
- Bison 冲突警告 - 已有默认处理规则
- 未使用变量警告 - 不影响功能

### Q: 如何处理路径中的空格？

**A:** Windows 用户需要用引号包裹路径：
```batch
set "GCC=C:\Program Files\mingw64\bin\gcc.exe"
```

### Q: 编译失败提示找不到头文件？

**A:** 确保使用正确的构建命令：
```bash
# 完整构建
.\build.bat parser

# 清理后重建
.\build.bat clean
.\build.bat parser
```

### Q: 如何贡献代码？

**A:** 欢迎贡献！
1. Fork 项目
2. 创建功能分支：`git checkout -b feature/new-feature`
3. 提交更改：`git commit -am 'Add new feature'`
4. 推送分支：`git push origin feature/new-feature`
5. 创建 Pull Request

---

## 📚 参考资料

### 规范文档
- [ECMAScript 5.1 规范](https://262.ecma-international.org/5.1/)
- [ASI 规则（11.9 节）](https://262.ecma-international.org/5.1/#sec-11.9)

### 工具文档
- [re2c 官方手册](https://re2c.org/manual/manual_c.html)
- [GNU Bison 手册](https://www.gnu.org/software/bison/manual/)
- [GCC 文档](https://gcc.gnu.org/onlinedocs/)

### 参考项目
- [Esprima](https://esprima.org/) - JavaScript 解析器
- [Acorn](https://github.com/acornjs/acorn) - 轻量级 JS 解析器
- [Babel Parser](https://babeljs.io/docs/en/babel-parser) - Babel 解析器

### 学习资源
- 《编译原理》（龙书） - Alfred V. Aho 等著
- 《现代编译原理》 - Andrew W. Appel 著
- 《程序设计语言实现模式》 - Terence Parr 著

---

## 📊 项目统计

| 指标         | 数值  |
| ------------ | ----- |
| 总代码行数   | ~3000 |
| 测试文件数   | 16+   |
| 测试通过率   | 100%  |
| 支持的关键字 | 27    |
| 支持的运算符 | 73+   |
| AST 节点类型 | 30+   |

---

## 👥 贡献者

- [Stardreama](https://github.com/Stardreama)
- [Limpid-8818](https://github.com/Limpid-8818)
- [FanZDStar](https://github.com/FanZDStar)
- [Aurora-0413](https://github.com/Aurora-0413)

---

## 📄 许可证

本项目采用 [MIT 许可证](LICENSE)。

---

## 🔗 相关链接

- **项目主页**: https://github.com/Aurora-0413/js_compiler_by_c
- **Issues**: https://github.com/Aurora-0413/js_compiler_by_c/issues
- **Wiki**: https://github.com/Aurora-0413/js_compiler_by_c/wiki

---

## 📝 更新日志

### Version 2.0.0 (2025-11-18)
- ✨ 重构项目结构，模块化设计
- ✨ 优化头文件，添加完整注释
- ✨ 新增工具函数库
- ✨ 改进构建系统
- ✨ 完善文档

### Version 1.0.0 (2025-11-11)
- 🎉 初始版本发布
- ✅ 完整的词法分析器
- ✅ 完整的语法分析器
- ✅ ASI 机制实现
- ✅ AST 构建与输出

---

<div align="center">

**⭐ 如果这个项目对你有帮助，请给一个 Star！⭐**

Made with ❤️ by JS Compiler Team

</div>
