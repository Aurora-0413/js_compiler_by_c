/**
 * @file ast.h
 * @brief 抽象语法树（AST）定义与接口
 * @author JS Compiler Team
 * @date 2025
 */

#ifndef JS_COMPILER_AST_H
#define JS_COMPILER_AST_H

#include <stdbool.h>
#include <stddef.h>

/* ==================== AST 节点类型 ==================== */

/**
 * @brief AST 节点类型枚举
 */
typedef enum
{
    /* 程序结构 */
    AST_PROGRAM, /* 程序根节点 */
    AST_BLOCK,   /* 代码块 */

    /* 声明 */
    AST_VAR_DECL,      /* 变量声明 */
    AST_FUNCTION_DECL, /* 函数声明 */

    /* 语句 */
    AST_RETURN_STMT,   /* return 语句 */
    AST_IF_STMT,       /* if 语句 */
    AST_FOR_STMT,      /* for 循环 */
    AST_WHILE_STMT,    /* while 循环 */
    AST_DO_WHILE_STMT, /* do-while 循环 */
    AST_SWITCH_STMT,   /* switch 语句 */
    AST_TRY_STMT,      /* try-catch-finally 语句 */
    AST_WITH_STMT,     /* with 语句 */
    AST_LABELED_STMT,  /* 标签语句 */
    AST_BREAK_STMT,    /* break 语句 */
    AST_CONTINUE_STMT, /* continue 语句 */
    AST_THROW_STMT,    /* throw 语句 */
    AST_EXPR_STMT,     /* 表达式语句 */
    AST_EMPTY_STMT,    /* 空语句 */

    /* 表达式 */
    AST_IDENTIFIER,       /* 标识符 */
    AST_LITERAL,          /* 字面量 */
    AST_ASSIGN_EXPR,      /* 赋值表达式 */
    AST_BINARY_EXPR,      /* 二元表达式 */
    AST_CONDITIONAL_EXPR, /* 条件表达式 */
    AST_SEQUENCE_EXPR,    /* 逗号表达式 */
    AST_UNARY_EXPR,       /* 一元表达式 */
    AST_UPDATE_EXPR,      /* 更新表达式（++ --） */
    AST_CALL_EXPR,        /* 函数调用 */
    AST_MEMBER_EXPR,      /* 成员访问 */
    AST_ARRAY_LITERAL,    /* 数组字面量 */
    AST_OBJECT_LITERAL,   /* 对象字面量 */

    /* 辅助节点 */
    AST_PROPERTY,    /* 对象属性 */
    AST_SWITCH_CASE, /* switch case */
    AST_CATCH_CLAUSE /* catch 子句 */
} ASTNodeType;

/**
 * @brief 变量声明类型
 */
typedef enum
{
    AST_VAR_KIND_VAR,  /* var */
    AST_VAR_KIND_LET,  /* let */
    AST_VAR_KIND_CONST /* const */
} ASTVarKind;

/**
 * @brief 字面量类型
 */
typedef enum
{
    AST_LITERAL_NUMBER,   /* 数字 */
    AST_LITERAL_STRING,   /* 字符串 */
    AST_LITERAL_BOOLEAN,  /* 布尔值 */
    AST_LITERAL_NULL,     /* null */
    AST_LITERAL_UNDEFINED /* undefined */
} ASTLiteralType;

/* ==================== 前向声明 ==================== */

typedef struct ASTNode ASTNode;
typedef struct ASTList ASTList;

/* ==================== AST 链表 ==================== */

/**
 * @brief AST 节点链表
 */
struct ASTList
{
    ASTNode *node; /* 节点指针 */
    ASTList *next; /* 下一个节点 */
};

/* ==================== 属性键 ==================== */

/**
 * @brief 对象属性键
 */
typedef struct
{
    char *name;         /* 属性名 */
    bool is_identifier; /* 是否为标识符 */
} ASTPropertyKey;

/* ==================== AST 节点 ==================== */

/**
 * @brief AST 节点结构
 */
struct ASTNode
{
    ASTNodeType type; /* 节点类型 */

    union
    {
        /* 程序 */
        struct
        {
            ASTList *body;
        } program;

        /* 代码块 */
        struct
        {
            ASTList *body;
        } block;

        /* 变量声明 */
        struct
        {
            ASTVarKind kind;
            char *name;
            ASTNode *init;
        } var_decl;

        /* 函数声明 */
        struct
        {
            char *name;
            ASTList *params;
            ASTNode *body;
        } function_decl;

        /* return 语句 */
        struct
        {
            ASTNode *argument;
        } return_stmt;

        /* if 语句 */
        struct
        {
            ASTNode *test;
            ASTNode *consequent;
            ASTNode *alternate;
        } if_stmt;

        /* for 循环 */
        struct
        {
            ASTNode *init;
            ASTNode *test;
            ASTNode *update;
            ASTNode *body;
        } for_stmt;

        /* while 循环 */
        struct
        {
            ASTNode *test;
            ASTNode *body;
        } while_stmt;

        /* do-while 循环 */
        struct
        {
            ASTNode *body;
            ASTNode *test;
        } do_while_stmt;

        /* switch 语句 */
        struct
        {
            ASTNode *discriminant;
            ASTList *cases;
        } switch_stmt;

        /* try 语句 */
        struct
        {
            ASTNode *block;
            ASTNode *handler;
            ASTNode *finalizer;
        } try_stmt;

        /* with 语句 */
        struct
        {
            ASTNode *object;
            ASTNode *body;
        } with_stmt;

        /* 标签语句 */
        struct
        {
            char *label;
            ASTNode *body;
        } labeled_stmt;

        /* break 语句 */
        struct
        {
            char *label;
        } break_stmt;

        /* continue 语句 */
        struct
        {
            char *label;
        } continue_stmt;

        /* throw 语句 */
        struct
        {
            ASTNode *argument;
        } throw_stmt;

        /* 表达式语句 */
        struct
        {
            ASTNode *expression;
        } expr_stmt;

        /* 标识符 */
        struct
        {
            char *name;
        } identifier;

        /* 字面量 */
        struct
        {
            ASTLiteralType literal_type;
            union
            {
                double number;
                bool boolean;
                char *string;
            } value;
        } literal;

        /* 二元表达式 */
        struct
        {
            const char *op;
            ASTNode *left;
            ASTNode *right;
        } binary;

        /* 条件表达式 */
        struct
        {
            ASTNode *test;
            ASTNode *consequent;
            ASTNode *alternate;
        } conditional;

        /* 逗号表达式 */
        struct
        {
            ASTList *elements;
        } sequence;

        /* 赋值表达式 */
        struct
        {
            const char *op;
            ASTNode *left;
            ASTNode *right;
        } assign;

        /* 一元表达式 */
        struct
        {
            const char *op;
            ASTNode *argument;
        } unary;

        /* 更新表达式 */
        struct
        {
            const char *op;
            ASTNode *argument;
            bool prefix;
        } update;

        /* 函数调用 */
        struct
        {
            ASTNode *callee;
            ASTList *arguments;
        } call_expr;

        /* 成员访问 */
        struct
        {
            ASTNode *object;
            char *property;
            bool computed;
        } member_expr;

        /* 数组字面量 */
        struct
        {
            ASTList *elements;
        } array_literal;

        /* 对象字面量 */
        struct
        {
            ASTList *properties;
        } object_literal;

        /* 对象属性 */
        struct
        {
            ASTPropertyKey key;
            ASTNode *value;
        } property;

        /* switch case */
        struct
        {
            ASTNode *test;
            ASTList *consequent;
            bool is_default;
        } switch_case;

        /* catch 子句 */
        struct
        {
            char *param;
            ASTNode *body;
        } catch_clause;
    } data;
};

/* ==================== 公共接口 ==================== */

/* --- 链表操作 --- */

/**
 * @brief 在链表末尾追加节点
 * @param list 链表头
 * @param node 要追加的节点
 * @return 更新后的链表头
 */
ASTList *ast_list_append(ASTList *list, ASTNode *node);

/**
 * @brief 连接两个链表
 * @param head 第一个链表
 * @param tail 第二个链表
 * @return 连接后的链表头
 */
ASTList *ast_list_concat(ASTList *head, ASTList *tail);

/**
 * @brief 释放链表及其包含的所有节点
 * @param list 链表头
 */
void ast_list_free(ASTList *list);

/* --- 节点构造 --- */

/* 程序结构 */
ASTNode *ast_make_program(ASTList *body);
ASTNode *ast_make_block(ASTList *body);

/* 声明 */
ASTNode *ast_make_var_decl(ASTVarKind kind, char *name, ASTNode *init);
ASTNode *ast_make_function_decl(char *name, ASTList *params, ASTNode *body);

/* 语句 */
ASTNode *ast_make_return(ASTNode *argument);
ASTNode *ast_make_if(ASTNode *test, ASTNode *consequent, ASTNode *alternate);
ASTNode *ast_make_for(ASTNode *init, ASTNode *test, ASTNode *update, ASTNode *body);
ASTNode *ast_make_while(ASTNode *test, ASTNode *body);
ASTNode *ast_make_do_while(ASTNode *body, ASTNode *test);
ASTNode *ast_make_switch(ASTNode *discriminant, ASTList *cases);
ASTNode *ast_make_try(ASTNode *block, ASTNode *handler, ASTNode *finalizer);
ASTNode *ast_make_with(ASTNode *object, ASTNode *body);
ASTNode *ast_make_labeled(char *label, ASTNode *body);
ASTNode *ast_make_break(char *label);
ASTNode *ast_make_continue(char *label);
ASTNode *ast_make_throw(ASTNode *argument);
ASTNode *ast_make_expression_stmt(ASTNode *expression);
ASTNode *ast_make_empty_statement(void);

/* 表达式 */
ASTNode *ast_make_identifier(char *name);
ASTNode *ast_make_number_literal(char *raw);
ASTNode *ast_make_string_literal(char *raw);
ASTNode *ast_make_boolean_literal(bool value);
ASTNode *ast_make_null_literal(void);
ASTNode *ast_make_undefined_literal(void);
ASTNode *ast_make_assignment(const char *op, ASTNode *left, ASTNode *right);
ASTNode *ast_make_binary(const char *op, ASTNode *left, ASTNode *right);
ASTNode *ast_make_conditional(ASTNode *test, ASTNode *consequent, ASTNode *alternate);
ASTNode *ast_make_sequence(ASTNode *left, ASTNode *right);
ASTNode *ast_make_unary(const char *op, ASTNode *argument);
ASTNode *ast_make_update(const char *op, ASTNode *argument, bool prefix);
ASTNode *ast_make_call(ASTNode *callee, ASTList *arguments);
ASTNode *ast_make_member(ASTNode *object, char *property, bool computed);
ASTNode *ast_make_array_literal(ASTList *elements);
ASTNode *ast_make_object_literal(ASTList *properties);
ASTNode *ast_make_property(char *key, bool is_identifier, ASTNode *value);

/* 辅助节点 */
ASTNode *ast_make_switch_case(ASTNode *test, ASTList *consequent);
ASTNode *ast_make_switch_default(ASTList *consequent);
ASTNode *ast_make_catch(char *param, ASTNode *body);

/* --- 工具函数 --- */

/**
 * @brief 打印 AST（用于调试）
 * @param node AST 节点
 */
void ast_print(ASTNode *node);

/**
 * @brief 释放 AST 节点及其子节点
 * @param node AST 节点
 */
void ast_free(ASTNode *node);

/**
 * @brief 访问者模式遍历 AST
 * @param node AST 节点
 * @param visitor 访问函数
 * @param userdata 用户数据
 */
typedef void (*ASTVisitor)(ASTNode *node, void *userdata);
void ast_traverse(ASTNode *node, ASTVisitor visitor, void *userdata);

/**
 * @brief 获取节点类型的字符串表示
 * @param type 节点类型
 * @return 节点类型的字符串
 */
const char *ast_node_type_to_string(ASTNodeType type);

#endif /* JS_COMPILER_AST_H */
