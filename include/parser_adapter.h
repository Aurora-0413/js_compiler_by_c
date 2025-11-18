/**
 * @file parser_adapter.h
 * @brief 词法分析器与语法分析器适配层
 * @author JS Compiler Team
 * @date 2025
 *
 * 该模块负责:
 * 1. 将词法 Token 映射到 Bison 终结符
 * 2. 实现自动分号插入 (ASI) 机制
 * 3. 管理解析器状态和上下文
 */

#ifndef JS_COMPILER_PARSER_ADAPTER_H
#define JS_COMPILER_PARSER_ADAPTER_H

#include "token.h"
#include "ast.h"
#include <stdbool.h>

/* ==================== ASI (自动分号插入) ==================== */

/**
 * @brief 控制语句类型
 */
typedef enum
{
    CONTROL_IF,
    CONTROL_FOR,
    CONTROL_WHILE,
    CONTROL_WITH,
    CONTROL_SWITCH
} ControlType;

/**
 * @brief 括号类型
 */
typedef enum
{
    BRACE_BLOCK, /* 块语句的大括号 */
    BRACE_OBJECT /* 对象字面量的大括号 */
} BraceType;

/* ==================== 解析器状态管理 ==================== */

/**
 * @brief 设置解析器输入
 * @param input 源代码字符串
 */
void parser_set_input(const char *input);

/**
 * @brief 重置解析器状态
 */
void parser_reset_state(void);

/**
 * @brief 获取当前 AST
 * @return AST 根节点
 */
ASTNode *parser_take_ast(void);

/**
 * @brief 设置 AST 根节点
 * @param root AST 根节点
 */
void parser_set_ast(ASTNode *root);

/**
 * @brief 获取错误计数
 * @return 错误数量
 */
int parser_error_count(void);

/**
 * @brief 重置错误计数
 */
void parser_reset_error_count(void);

/**
 * @brief 增加错误计数
 */
void parser_increment_error_count(void);

/* ==================== Bison 接口 ==================== */

/**
 * @brief Bison 词法分析器接口
 * @return Bison Token 类型
 */
int yylex(void);

/**
 * @brief Bison 错误处理函数
 * @param msg 错误消息
 */
void yyerror(const char *msg);

/**
 * @brief 获取当前语义值
 * @return 语义值指针
 */
void *yylval_ptr(void);

#endif /* JS_COMPILER_PARSER_ADAPTER_H */
