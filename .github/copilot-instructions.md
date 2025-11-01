# JavaScript 编译器项目 - AI 编码助手指南

## 项目概述

这是一个基于 C 语言开发的 JavaScript 语法解析器，使用 **re2c**（词法分析）和 **Bison**（语法分析）来构建完整的 JS 解析器。

**当前状态**: 第一阶段 - 词法分析器已完成 ✅

### ✅ 已完成的组件

1. **词法分析器** (`lexer.re`, 300+ 行)
   - 支持 27 个 ES5 关键字 (var, let, const, function, if, else, for, while, return, break, continue, switch, case, default, try, catch, finally, throw, new, this, typeof, delete, in, instanceof, void, with, debugger)
   - 支持 73+ 种运算符和分隔符
   - 数字字面量：整数、浮点数、科学计数法、十六进制
   - 字符串字面量：单引号、双引号，支持转义字符
   - 注释处理：单行 `//` 和多行 `/* */`
   - 行号列号精确跟踪（为错误报告和 ASI 机制准备）
   - 换行标记 `has_newline` 字段（为 ASI 机制准备）

2. **Token 定义** (`token.h`)
   - 完整的 TokenType 枚举（80+ 种类型）
   - Token 结构体（类型、值、行号、列号）
   - Lexer 状态结构体

3. **测试程序** (`main.c`)
   - 文件读取功能
   - Token 流输出（格式化显示）
   - 错误报告
   - 已测试：成功解析 test_basic.js（106 个 tokens）

4. **构建系统**
   - `build.bat`: Windows 批处理脚本（推荐使用）
   - `Makefile`: Make 构建脚本（可选）
   - `BUILD.md`: 构建和使用文档

### 🚧 待开发的组件（TODOs）

#### 优先级 1: 语法分析器基础
- [ ] 创建 `parser.y` - Bison 语法分析器定义
  - [ ] 实现表达式语法规则（字面量、标识符、二元运算、一元运算）
  - [ ] 实现语句语法规则（变量声明、赋值、块语句）
  - [ ] 实现函数声明和调用
  - [ ] 实现控制流语句（if/else, for, while, do-while, switch）

#### 优先级 2: AST 结构
- [ ] 创建 `ast.h` - AST 节点类型定义
  - [ ] 定义节点类型枚举（表达式节点、语句节点等）
  - [ ] 定义各类型节点的结构体
- [ ] 创建 `ast.c` - AST 构建和操作函数
  - [ ] 实现节点创建函数
  - [ ] 实现 AST 打印/可视化函数（用于调试）
  - [ ] 实现节点释放函数

#### 优先级 3: ASI 机制实现
- [ ] 在 parser.y 中实现 ASI 规则
  - [ ] 换行符触发的分号插入
  - [ ] 文件结束触发的分号插入
  - [ ] 受限产生式的处理（return/break/continue/throw 后的换行）

#### 优先级 4: 测试用例扩展
- [ ] 创建 `tests/test_asi.js` - ASI 机制测试
- [ ] 创建 `tests/test_complex.js` - 复杂语法测试
- [ ] 创建 `tests/test_errors.js` - 错误处理测试

#### 优先级 5: 增强功能
- [ ] 创建 `utils.h/c` - 工具函数（字符串处理、内存管理等）
- [ ] 改进错误报告（更详细的错误信息和恢复策略）
- [ ] 添加详细的调试模式（显示解析过程）

## 核心架构

### 当前项目结构

```
js_compiler_by_c/
├── .github/
│   └── copilot-instructions.md  # AI 编码助手指南 ✅
├── .gitignore                   # Git 忽略文件配置 ✅
├── token.h                      # Token 类型定义 ✅
├── lexer.re                     # re2c 词法分析器源码 ✅
├── lexer.c                      # re2c 自动生成（不提交到 Git）
├── main.c                       # 主测试程序 ✅
├── build.bat                    # Windows 构建脚本 ✅
├── Makefile                     # Make 构建脚本 ✅
├── BUILD.md                     # 构建使用文档 ✅
├── readme.md                    # 项目说明文档 ✅
├── tests/                       # 测试用例目录 ✅
│   └── test_basic.js            # 基本语法测试 ✅
│
├── parser.y                     # Bison 语法分析器（待创建）
├── parser.c/.h                  # Bison 自动生成（待生成）
├── ast.h                        # AST 节点定义（待创建）
├── ast.c                        # AST 实现（待创建）
└── utils.h/utils.c              # 工具函数（待创建）
```

### 构建产物（不提交到 Git）

- `lexer.c` - 由 re2c 从 lexer.re 生成
- `parser.c`, `parser.h` - 由 Bison 从 parser.y 生成
- `*.o` - 目标文件
- `*.exe` - 可执行文件
- `js_lexer.exe` - 当前词法分析器可执行文件

### 技术栈选择的关键原因

- **re2c 而非 Flex**: JavaScript 需要 Unicode 支持（标识符、符号），re2c 原生支持 Unicode 定义
- **目标**: 实现 ECMAScript 语法验证和 **ASI（自动分号插入）** 机制

## 开发工作流

### 构建环境（Windows + MinGW）

**当前构建方式**（词法分析器已实现）：

```batch
# 方式1: 使用 build.bat（推荐）
build.bat        # 构建
build.bat test   # 构建并运行测试
build.bat clean  # 清理

# 方式2: 手动构建
re2c -o lexer.c lexer.re
D:\mingw64\bin\gcc.exe -Wall -g -std=c99 -c main.c -o main.o
D:\mingw64\bin\gcc.exe -Wall -g -std=c99 -c lexer.c -o lexer.o
D:\mingw64\bin\gcc.exe -Wall -g -std=c99 -o js_lexer.exe main.o lexer.o

# 运行词法分析器
js_lexer.exe tests\test_basic.js
```

**未来完整构建流程**（当语法分析器实现后）：

```bash
re2c -o lexer.c lexer.re
bison -d -o parser.c parser.y
gcc -Wall -g -o js_parser lexer.c parser.c main.c ast.c utils.c
```

### 测试策略

按照 readme.md 规划，测试应覆盖：

1. **基本语法** (`test_basic.js`): 变量声明、函数、控制流
2. **ASI 机制** (`test_asi.js`): `return`/`break`/`continue` 后换行的分号插入
3. **复杂结构** (`test_complex.js`): 对象字面量、数组、嵌套控制流

## 项目特定约定

### ASI 实现要点（核心难点）

根据 ECMAScript 11.9 节，ASI 触发条件：

1. **换行符 + 语法错误**: token 间有换行符且当前无法形成合法语法
2. **文件结束**: 输入流结束但程序不完整
3. **受限产生式**: `return`/`break`/`continue`/`throw` 后遇到换行

**实现策略**:

```c
// 在 lexer.re 中跟踪换行状态
bool hasNewline = false;

// 在 parser.y 错误恢复规则中检查 ASI 条件
// 示例：return 语句后的换行应插入分号
```

### 错误报告格式（预期）

```
Syntax Error at line 5, column 12:
Unexpected token '}'
Expected: ';' or expression
```

必须在词法分析器中追踪行号和列号。

## 参考资源

- **ECMAScript 规范**: https://tc39.es/ecma262/ （语法定义的权威来源）
- **re2c 手册**: https://re2c.org/manual/manual_c.html
- **Bison 手册**: https://www.gnu.org/software/bison/manual/
- **参考实现**: https://github.com/sunxfancy/flex-bison-examples

## 开发优先级

1. ✅ **词法分析器** (`lexer.re`): 已完成，实现所有基本 token（关键字、标识符、字面量、运算符）
2. 🚧 **语法分析器** (`parser.y`): **【下一步】** 从简单表达式到语句结构逐步扩展
3. 🚧 **AST 结构** (`ast.h/c`): 定义节点类型和构建函数
4. 🚧 **ASI 机制**: 在语法分析器中集成分号插入逻辑
5. 🚧 **测试用例**: 边开发边验证每个语法特性

## 已知限制

- 当前仅计划支持 ES5 基本语法
- 不包含语义分析或代码生成
- ES6+ 特性（箭头函数、类、async/await）不在初始范围内
