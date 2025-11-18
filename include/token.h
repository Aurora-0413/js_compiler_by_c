/**
 * @file token.h
 * @brief JavaScript 词法单元定义
 * @author JS Compiler Team
 * @date 2025
 */

#ifndef JS_COMPILER_TOKEN_H
#define JS_COMPILER_TOKEN_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Token 类型枚举
 * 按照 ECMAScript 5.1 规范定义
 */
typedef enum
{
    /* 关键字 (Keywords) */
    TOK_VAR = 256,  /* var */
    TOK_LET,        /* let */
    TOK_CONST,      /* const */
    TOK_FUNCTION,   /* function */
    TOK_IF,         /* if */
    TOK_ELSE,       /* else */
    TOK_FOR,        /* for */
    TOK_WHILE,      /* while */
    TOK_DO,         /* do */
    TOK_RETURN,     /* return */
    TOK_BREAK,      /* break */
    TOK_CONTINUE,   /* continue */
    TOK_SWITCH,     /* switch */
    TOK_CASE,       /* case */
    TOK_DEFAULT,    /* default */
    TOK_TRY,        /* try */
    TOK_CATCH,      /* catch */
    TOK_FINALLY,    /* finally */
    TOK_THROW,      /* throw */
    TOK_NEW,        /* new */
    TOK_THIS,       /* this */
    TOK_TYPEOF,     /* typeof */
    TOK_DELETE,     /* delete */
    TOK_IN,         /* in */
    TOK_INSTANCEOF, /* instanceof */
    TOK_VOID,       /* void */
    TOK_WITH,       /* with */
    TOK_DEBUGGER,   /* debugger */

    /* 字面量 (Literals) */
    TOK_TRUE,       /* true */
    TOK_FALSE,      /* false */
    TOK_NULL,       /* null */
    TOK_UNDEFINED,  /* undefined */
    TOK_NUMBER,     /* 数字字面量 */
    TOK_STRING,     /* 字符串字面量 */
    TOK_REGEX,      /* 正则表达式字面量 */
    TOK_IDENTIFIER, /* 标识符 */

    /* 算术运算符 (Arithmetic Operators) */
    TOK_PLUS,        /* + */
    TOK_MINUS,       /* - */
    TOK_STAR,        /* * */
    TOK_SLASH,       /* / */
    TOK_PERCENT,     /* % */
    TOK_PLUS_PLUS,   /* ++ */
    TOK_MINUS_MINUS, /* -- */

    /* 赋值运算符 (Assignment Operators) */
    TOK_ASSIGN,         /* = */
    TOK_PLUS_ASSIGN,    /* += */
    TOK_MINUS_ASSIGN,   /* -= */
    TOK_STAR_ASSIGN,    /* *= */
    TOK_SLASH_ASSIGN,   /* /= */
    TOK_PERCENT_ASSIGN, /* %= */

    /* 比较运算符 (Comparison Operators) */
    TOK_EQ,        /* == */
    TOK_NE,        /* != */
    TOK_EQ_STRICT, /* === */
    TOK_NE_STRICT, /* !== */
    TOK_LT,        /* < */
    TOK_LE,        /* <= */
    TOK_GT,        /* > */
    TOK_GE,        /* >= */

    /* 逻辑运算符 (Logical Operators) */
    TOK_AND, /* && */
    TOK_OR,  /* || */
    TOK_NOT, /* ! */

    /* 位运算符 (Bitwise Operators) */
    TOK_BIT_AND, /* & */
    TOK_BIT_OR,  /* | */
    TOK_BIT_XOR, /* ^ */
    TOK_BIT_NOT, /* ~ */
    TOK_LSHIFT,  /* << */
    TOK_RSHIFT,  /* >> */
    TOK_URSHIFT, /* >>> */

    /* 位赋值运算符 (Bitwise Assignment Operators) */
    TOK_AND_ASSIGN,     /* &= */
    TOK_OR_ASSIGN,      /* |= */
    TOK_XOR_ASSIGN,     /* ^= */
    TOK_LSHIFT_ASSIGN,  /* <<= */
    TOK_RSHIFT_ASSIGN,  /* >>= */
    TOK_URSHIFT_ASSIGN, /* >>>= */

    /* 其他运算符 (Other Operators) */
    TOK_QUESTION, /* ? */
    TOK_COLON,    /* : */

    /* 分隔符 (Delimiters) */
    TOK_LPAREN,    /* ( */
    TOK_RPAREN,    /* ) */
    TOK_LBRACE,    /* { */
    TOK_RBRACE,    /* } */
    TOK_LBRACKET,  /* [ */
    TOK_RBRACKET,  /* ] */
    TOK_SEMICOLON, /* ; */
    TOK_COMMA,     /* , */
    TOK_DOT,       /* . */

    /* 特殊 Token (Special Tokens) */
    TOK_EOF,    /* 文件结束 */
    TOK_ERROR,  /* 错误 Token */
    TOK_NEWLINE /* 换行（用于 ASI） */
} TokenType;

/**
 * @brief Token 上下文状态
 * 用于确定是否可以跟随正则表达式
 */
typedef enum
{
    TOKEN_CONTEXT_ALLOW_REGEX, /* 允许正则表达式 */
    TOKEN_CONTEXT_NO_REGEX,    /* 不允许正则表达式 */

    /* 向后兼容旧命名 */
    PREV_TOK_CAN_REGEX = TOKEN_CONTEXT_ALLOW_REGEX,
    PREV_TOK_NO_REGEX = TOKEN_CONTEXT_NO_REGEX
} TokenContext;

/**
 * @brief Token 结构体
 */
typedef struct
{
    TokenType type; /* Token 类型 */
    char *value;    /* Token 值（仅用于标识符、数字、字符串） */
    size_t length;  /* Token 长度 */
    int line;       /* 行号 */
    int column;     /* 列号 */
} Token;

/**
 * @brief 词法分析器状态
 */
typedef struct
{
    const char *input;     /* 输入源代码 */
    const char *cursor;    /* 当前位置指针 */
    const char *marker;    /* re2c 内部标记 */
    const char *ctxmarker; /* re2c 上下文标记 */

    int line;   /* 当前行号 */
    int column; /* 当前列号 */

    bool has_newline; /* 自上次 Token 以来是否有换行 */

    /* 主字段 */
    TokenContext context; /* Token 上下文 */

/* 向后兼容别名（与 context 共享同一内存位置） */
#define prev_tok_state context

    Token *pending_token; /* 待处理的 Token（用于 ASI） */
} Lexer;

/* ==================== 公共接口 ==================== */

/**
 * @brief 初始化词法分析器
 * @param lexer 词法分析器指针
 * @param input 输入源代码字符串
 */
void lexer_init(Lexer *lexer, const char *input);

/**
 * @brief 获取下一个 Token
 * @param lexer 词法分析器指针
 * @return 下一个 Token
 */
Token lexer_next_token(Lexer *lexer);

/**
 * @brief 释放 Token 资源
 * @param token Token 指针
 */
void token_free(Token *token);

/**
 * @brief 将 Token 类型转换为字符串
 * @param type Token 类型
 * @return Token 类型的字符串表示
 */
const char *token_type_to_string(TokenType type);

/**
 * @brief 复制 Token
 * @param token 源 Token
 * @return 新的 Token 副本
 */
Token token_copy(const Token *token);

/**
 * @brief 判断 Token 是否为关键字
 * @param type Token 类型
 * @return 如果是关键字返回 true，否则返回 false
 */
bool token_is_keyword(TokenType type);

/**
 * @brief 判断 Token 是否为字面量
 * @param type Token 类型
 * @return 如果是字面量返回 true，否则返回 false
 */
bool token_is_literal(TokenType type);

/**
 * @brief 判断 Token 是否为运算符
 * @param type Token 类型
 * @return 如果是运算符返回 true，否则返回 false
 */
bool token_is_operator(TokenType type);

#endif /* JS_COMPILER_TOKEN_H */
