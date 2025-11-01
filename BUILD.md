# JavaScript 词法分析器 - 快速开始

## 构建项目

### Windows (使用 build.bat)

```bash
# 构建词法分析器
build.bat

# 清理生成文件
build.bat clean

# 构建并运行测试
build.bat test

# 显示帮助
build.bat help
```

### 使用 Makefile (需要安装 make)

```bash
# 构建词法分析器
make

# 运行测试
make test

# 清理
make clean
```

## 运行

```bash
# 分析 JavaScript 文件
js_lexer.exe tests\test_basic.js

# 或使用任何 JS 文件
js_lexer.exe your_script.js
```

## 已实现的功能

### ✅ 关键字识别

- 变量声明: `var`, `let`, `const`
- 函数: `function`, `return`
- 控制流: `if`, `else`, `for`, `while`, `do`, `switch`, `case`, `default`
- 异常处理: `try`, `catch`, `finally`, `throw`
- 其他: `break`, `continue`, `new`, `this`, `typeof`, `delete`, `in`, `instanceof`, `void`, `with`, `debugger`

### ✅ 字面量识别

- 数字: 整数、浮点数、科学计数法、十六进制
- 字符串: 单引号和双引号字符串，支持转义字符
- 布尔值: `true`, `false`
- 特殊值: `null`, `undefined`

### ✅ 运算符

- 算术: `+`, `-`, `*`, `/`, `%`, `++`, `--`
- 比较: `==`, `!=`, `===`, `!==`, `<`, `>`, `<=`, `>=`
- 逻辑: `&&`, `||`, `!`
- 位运算: `&`, `|`, `^`, `~`, `<<`, `>>`, `>>>`
- 赋值: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`, `>>>=`
- 三元: `?`, `:`

### ✅ 分隔符

- 括号: `(`, `)`, `{`, `}`, `[`, `]`
- 其他: `;`, `,`, `.`

### ✅ 其他功能

- 单行注释 (`//`) 和多行注释 (`/* */`)
- 行号和列号跟踪（用于错误报告）
- 换行标记（用于 ASI 机制）

## 示例输出

```
=== Lexical Analysis of 'tests\test_basic.js' ===

[  1] Line   2, Col   1: VAR             = 'var'
[  2] Line   2, Col   5: IDENTIFIER      = 'x'
[  3] Line   2, Col   7: =
[  4] Line   2, Col   9: NUMBER          = '10'
[  5] Line   2, Col  11: ;
...

=== Analysis Complete ===
Total tokens: 106
```

## 下一步

- [ ] 实现语法分析器 (parser.y)
- [ ] 实现 AST 结构
- [ ] 实现 ASI（自动分号插入）机制
- [ ] 添加更多测试用例

## 技术细节

- **词法分析器**: re2c 3.0+
- **编译器**: GCC (MinGW)
- **C 标准**: C99

## 目录结构

```
js_compiler_by_c/
├── token.h         # Token 类型定义
├── lexer.re        # re2c 词法分析器源文件
├── lexer.c         # re2c 生成的 C 代码（自动生成）
├── main.c          # 主程序
├── build.bat       # Windows 构建脚本
├── Makefile        # Make 构建脚本
└── tests/          # 测试用例目录
    └── test_basic.js
```
