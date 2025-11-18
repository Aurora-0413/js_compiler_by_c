/**
 * @file ast.c
 * @brief 抽象语法树（AST）实现
 * @author JS Compiler Team
 * @date 2025
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== 内存分配辅助函数 ==================== */

/**
 * @brief 分配 AST 节点
 */
static ASTNode *ast_alloc(ASTNodeType type)
{
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node)
    {
        fprintf(stderr, "[FATAL] Out of memory allocating AST node\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    return node;
}

/**
 * @brief 安全地复制字符串
 */
static char *ast_strdup(const char *s)
{
    if (!s)
        return NULL;
    char *copy = strdup(s);
    if (!copy)
    {
        fprintf(stderr, "[FATAL] Out of memory duplicating string\n");
        exit(EXIT_FAILURE);
    }
    return copy;
}

/* ==================== 链表操作 ==================== */

ASTList *ast_list_append(ASTList *list, ASTNode *node)
{
    ASTList *new_item = (ASTList *)malloc(sizeof(ASTList));
    if (!new_item)
    {
        fprintf(stderr, "[FATAL] Out of memory creating list node\n");
        exit(EXIT_FAILURE);
    }
    new_item->node = node;
    new_item->next = NULL;

    if (!list)
        return new_item;

    ASTList *tail = list;
    while (tail->next)
        tail = tail->next;
    tail->next = new_item;
    return list;
}

ASTList *ast_list_concat(ASTList *head, ASTList *tail)
{
    if (!head)
        return tail;
    if (!tail)
        return head;

    ASTList *last = head;
    while (last->next)
        last = last->next;
    last->next = tail;
    return head;
}

void ast_list_free(ASTList *list)
{
    while (list)
    {
        ASTList *next = list->next;
        if (list->node)
            ast_free(list->node);
        free(list);
        list = next;
    }
}

/* ==================== 节点构造函数 ==================== */

/* --- 程序结构 --- */

ASTNode *ast_make_program(ASTList *body)
{
    ASTNode *node = ast_alloc(AST_PROGRAM);
    node->data.program.body = body;
    return node;
}

ASTNode *ast_make_block(ASTList *body)
{
    ASTNode *node = ast_alloc(AST_BLOCK);
    node->data.block.body = body;
    return node;
}

/* --- 声明 --- */

ASTNode *ast_make_var_decl(ASTVarKind kind, char *name, ASTNode *init)
{
    ASTNode *node = ast_alloc(AST_VAR_DECL);
    node->data.var_decl.kind = kind;
    node->data.var_decl.name = ast_strdup(name);
    node->data.var_decl.init = init;
    return node;
}

ASTNode *ast_make_function_decl(char *name, ASTList *params, ASTNode *body)
{
    ASTNode *node = ast_alloc(AST_FUNCTION_DECL);
    node->data.function_decl.name = ast_strdup(name);
    node->data.function_decl.params = params;
    node->data.function_decl.body = body;
    return node;
}

/* --- 语句 --- */

ASTNode *ast_make_return(ASTNode *argument)
{
    ASTNode *node = ast_alloc(AST_RETURN_STMT);
    node->data.return_stmt.argument = argument;
    return node;
}

ASTNode *ast_make_if(ASTNode *test, ASTNode *consequent, ASTNode *alternate)
{
    ASTNode *node = ast_alloc(AST_IF_STMT);
    node->data.if_stmt.test = test;
    node->data.if_stmt.consequent = consequent;
    node->data.if_stmt.alternate = alternate;
    return node;
}

ASTNode *ast_make_for(ASTNode *init, ASTNode *test, ASTNode *update, ASTNode *body)
{
    ASTNode *node = ast_alloc(AST_FOR_STMT);
    node->data.for_stmt.init = init;
    node->data.for_stmt.test = test;
    node->data.for_stmt.update = update;
    node->data.for_stmt.body = body;
    return node;
}

ASTNode *ast_make_while(ASTNode *test, ASTNode *body)
{
    ASTNode *node = ast_alloc(AST_WHILE_STMT);
    node->data.while_stmt.test = test;
    node->data.while_stmt.body = body;
    return node;
}

ASTNode *ast_make_do_while(ASTNode *body, ASTNode *test)
{
    ASTNode *node = ast_alloc(AST_DO_WHILE_STMT);
    node->data.do_while_stmt.body = body;
    node->data.do_while_stmt.test = test;
    return node;
}

ASTNode *ast_make_switch(ASTNode *discriminant, ASTList *cases)
{
    ASTNode *node = ast_alloc(AST_SWITCH_STMT);
    node->data.switch_stmt.discriminant = discriminant;
    node->data.switch_stmt.cases = cases;
    return node;
}

ASTNode *ast_make_try(ASTNode *block, ASTNode *handler, ASTNode *finalizer)
{
    ASTNode *node = ast_alloc(AST_TRY_STMT);
    node->data.try_stmt.block = block;
    node->data.try_stmt.handler = handler;
    node->data.try_stmt.finalizer = finalizer;
    return node;
}

ASTNode *ast_make_with(ASTNode *object, ASTNode *body)
{
    ASTNode *node = ast_alloc(AST_WITH_STMT);
    node->data.with_stmt.object = object;
    node->data.with_stmt.body = body;
    return node;
}

ASTNode *ast_make_labeled(char *label, ASTNode *body)
{
    ASTNode *node = ast_alloc(AST_LABELED_STMT);
    node->data.labeled_stmt.label = ast_strdup(label);
    node->data.labeled_stmt.body = body;
    return node;
}

ASTNode *ast_make_break(char *label)
{
    ASTNode *node = ast_alloc(AST_BREAK_STMT);
    node->data.break_stmt.label = label ? ast_strdup(label) : NULL;
    return node;
}

ASTNode *ast_make_continue(char *label)
{
    ASTNode *node = ast_alloc(AST_CONTINUE_STMT);
    node->data.continue_stmt.label = label ? ast_strdup(label) : NULL;
    return node;
}

ASTNode *ast_make_throw(ASTNode *argument)
{
    ASTNode *node = ast_alloc(AST_THROW_STMT);
    node->data.throw_stmt.argument = argument;
    return node;
}

ASTNode *ast_make_expression_stmt(ASTNode *expression)
{
    ASTNode *node = ast_alloc(AST_EXPR_STMT);
    node->data.expr_stmt.expression = expression;
    return node;
}

ASTNode *ast_make_empty_statement(void)
{
    return ast_alloc(AST_EMPTY_STMT);
}

/* --- 表达式 --- */

ASTNode *ast_make_identifier(char *name)
{
    ASTNode *node = ast_alloc(AST_IDENTIFIER);
    node->data.identifier.name = ast_strdup(name);
    return node;
}

ASTNode *ast_make_number_literal(char *raw)
{
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_NUMBER;
    node->data.literal.value.number = atof(raw);
    return node;
}

ASTNode *ast_make_string_literal(char *raw)
{
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_STRING;
    // 去掉首尾引号
    size_t len = strlen(raw);
    if (len >= 2 && (raw[0] == '"' || raw[0] == '\''))
    {
        char *str = (char *)malloc(len - 1);
        if (str)
        {
            strncpy(str, raw + 1, len - 2);
            str[len - 2] = '\0';
            node->data.literal.value.string = str;
        }
    }
    else
    {
        node->data.literal.value.string = ast_strdup(raw);
    }
    return node;
}

ASTNode *ast_make_boolean_literal(bool value)
{
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_BOOLEAN;
    node->data.literal.value.boolean = value;
    return node;
}

ASTNode *ast_make_null_literal(void)
{
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_NULL;
    return node;
}

ASTNode *ast_make_undefined_literal(void)
{
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_UNDEFINED;
    return node;
}

ASTNode *ast_make_assignment(const char *op, ASTNode *left, ASTNode *right)
{
    ASTNode *node = ast_alloc(AST_ASSIGN_EXPR);
    node->data.assign.op = op;
    node->data.assign.left = left;
    node->data.assign.right = right;
    return node;
}

ASTNode *ast_make_binary(const char *op, ASTNode *left, ASTNode *right)
{
    ASTNode *node = ast_alloc(AST_BINARY_EXPR);
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ASTNode *ast_make_conditional(ASTNode *test, ASTNode *consequent, ASTNode *alternate)
{
    ASTNode *node = ast_alloc(AST_CONDITIONAL_EXPR);
    node->data.conditional.test = test;
    node->data.conditional.consequent = consequent;
    node->data.conditional.alternate = alternate;
    return node;
}

ASTNode *ast_make_sequence(ASTNode *left, ASTNode *right)
{
    ASTNode *node = ast_alloc(AST_SEQUENCE_EXPR);
    // 将两个表达式添加到序列链表
    node->data.sequence.elements = NULL;
    node->data.sequence.elements = ast_list_append(node->data.sequence.elements, left);
    node->data.sequence.elements = ast_list_append(node->data.sequence.elements, right);
    return node;
}

ASTNode *ast_make_unary(const char *op, ASTNode *argument)
{
    ASTNode *node = ast_alloc(AST_UNARY_EXPR);
    node->data.unary.op = op;
    node->data.unary.argument = argument;
    return node;
}

ASTNode *ast_make_update(const char *op, ASTNode *argument, bool prefix)
{
    ASTNode *node = ast_alloc(AST_UPDATE_EXPR);
    node->data.update.op = op;
    node->data.update.argument = argument;
    node->data.update.prefix = prefix;
    return node;
}

ASTNode *ast_make_call(ASTNode *callee, ASTList *arguments)
{
    ASTNode *node = ast_alloc(AST_CALL_EXPR);
    node->data.call_expr.callee = callee;
    node->data.call_expr.arguments = arguments;
    return node;
}

ASTNode *ast_make_member(ASTNode *object, char *property, bool computed)
{
    ASTNode *node = ast_alloc(AST_MEMBER_EXPR);
    node->data.member_expr.object = object;
    node->data.member_expr.property = ast_strdup(property);
    node->data.member_expr.computed = computed;
    return node;
}

ASTNode *ast_make_array_literal(ASTList *elements)
{
    ASTNode *node = ast_alloc(AST_ARRAY_LITERAL);
    node->data.array_literal.elements = elements;
    return node;
}

ASTNode *ast_make_object_literal(ASTList *properties)
{
    ASTNode *node = ast_alloc(AST_OBJECT_LITERAL);
    node->data.object_literal.properties = properties;
    return node;
}

ASTNode *ast_make_property(char *key, bool is_identifier, ASTNode *value)
{
    ASTNode *node = ast_alloc(AST_PROPERTY);
    node->data.property.key.name = ast_strdup(key);
    node->data.property.key.is_identifier = is_identifier;
    node->data.property.value = value;
    return node;
}

/* --- 辅助节点 --- */

ASTNode *ast_make_switch_case(ASTNode *test, ASTList *consequent)
{
    ASTNode *node = ast_alloc(AST_SWITCH_CASE);
    node->data.switch_case.test = test;
    node->data.switch_case.consequent = consequent;
    node->data.switch_case.is_default = false;
    return node;
}

ASTNode *ast_make_switch_default(ASTList *consequent)
{
    ASTNode *node = ast_alloc(AST_SWITCH_CASE);
    node->data.switch_case.test = NULL;
    node->data.switch_case.consequent = consequent;
    node->data.switch_case.is_default = true;
    return node;
}

ASTNode *ast_make_catch(char *param, ASTNode *body)
{
    ASTNode *node = ast_alloc(AST_CATCH_CLAUSE);
    node->data.catch_clause.param = ast_strdup(param);
    node->data.catch_clause.body = body;
    return node;
}

/* ==================== 节点类型字符串 ==================== */

const char *ast_node_type_to_string(ASTNodeType type)
{
    switch (type)
    {
    case AST_PROGRAM:
        return "Program";
    case AST_BLOCK:
        return "BlockStatement";
    case AST_VAR_DECL:
        return "VariableDeclaration";
    case AST_FUNCTION_DECL:
        return "FunctionDeclaration";
    case AST_RETURN_STMT:
        return "ReturnStatement";
    case AST_IF_STMT:
        return "IfStatement";
    case AST_FOR_STMT:
        return "ForStatement";
    case AST_WHILE_STMT:
        return "WhileStatement";
    case AST_DO_WHILE_STMT:
        return "DoWhileStatement";
    case AST_SWITCH_STMT:
        return "SwitchStatement";
    case AST_TRY_STMT:
        return "TryStatement";
    case AST_WITH_STMT:
        return "WithStatement";
    case AST_LABELED_STMT:
        return "LabeledStatement";
    case AST_BREAK_STMT:
        return "BreakStatement";
    case AST_CONTINUE_STMT:
        return "ContinueStatement";
    case AST_THROW_STMT:
        return "ThrowStatement";
    case AST_EXPR_STMT:
        return "ExpressionStatement";
    case AST_EMPTY_STMT:
        return "EmptyStatement";
    case AST_IDENTIFIER:
        return "Identifier";
    case AST_LITERAL:
        return "Literal";
    case AST_ASSIGN_EXPR:
        return "AssignmentExpression";
    case AST_BINARY_EXPR:
        return "BinaryExpression";
    case AST_CONDITIONAL_EXPR:
        return "ConditionalExpression";
    case AST_SEQUENCE_EXPR:
        return "SequenceExpression";
    case AST_UNARY_EXPR:
        return "UnaryExpression";
    case AST_UPDATE_EXPR:
        return "UpdateExpression";
    case AST_CALL_EXPR:
        return "CallExpression";
    case AST_MEMBER_EXPR:
        return "MemberExpression";
    case AST_ARRAY_LITERAL:
        return "ArrayExpression";
    case AST_OBJECT_LITERAL:
        return "ObjectExpression";
    case AST_PROPERTY:
        return "Property";
    case AST_SWITCH_CASE:
        return "SwitchCase";
    case AST_CATCH_CLAUSE:
        return "CatchClause";
    default:
        return "Unknown";
    }
}

/* ==================== 打印 AST ==================== */

static void ast_print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

static void ast_print_node(ASTNode *node, int depth);

static void ast_print_list(ASTList *list, int depth)
{
    while (list)
    {
        ast_print_node(list->node, depth);
        list = list->next;
    }
}

static void ast_print_node(ASTNode *node, int depth)
{
    if (!node)
    {
        ast_print_indent(depth);
        printf("(null)\n");
        return;
    }

    ast_print_indent(depth);
    printf("%s", ast_node_type_to_string(node->type));

    switch (node->type)
    {
    case AST_PROGRAM:
        printf("\n");
        ast_print_list(node->data.program.body, depth + 1);
        break;

    case AST_BLOCK:
        printf("\n");
        ast_print_list(node->data.block.body, depth + 1);
        break;

    case AST_VAR_DECL:
        printf(" (%s)\n", node->data.var_decl.kind == AST_VAR_KIND_VAR   ? "var"
                          : node->data.var_decl.kind == AST_VAR_KIND_LET ? "let"
                                                                         : "const");
        ast_print_indent(depth + 1);
        printf("name: \"%s\"\n", node->data.var_decl.name);
        if (node->data.var_decl.init)
        {
            ast_print_indent(depth + 1);
            printf("init:\n");
            ast_print_node(node->data.var_decl.init, depth + 2);
        }
        break;

    case AST_FUNCTION_DECL:
        printf(" %s\n", node->data.function_decl.name);
        if (node->data.function_decl.params)
        {
            ast_print_indent(depth + 1);
            printf("params:\n");
            ast_print_list(node->data.function_decl.params, depth + 2);
        }
        ast_print_indent(depth + 1);
        printf("body:\n");
        ast_print_node(node->data.function_decl.body, depth + 2);
        break;

    case AST_IDENTIFIER:
        printf("(%s)\n", node->data.identifier.name);
        break;

    case AST_LITERAL:
        switch (node->data.literal.literal_type)
        {
        case AST_LITERAL_NUMBER:
            printf("(%g)\n", node->data.literal.value.number);
            break;
        case AST_LITERAL_STRING:
            printf("(\"%s\")\n", node->data.literal.value.string);
            break;
        case AST_LITERAL_BOOLEAN:
            printf("(%s)\n", node->data.literal.value.boolean ? "true" : "false");
            break;
        case AST_LITERAL_NULL:
            printf("(null)\n");
            break;
        case AST_LITERAL_UNDEFINED:
            printf("(undefined)\n");
            break;
        }
        break;

    case AST_BINARY_EXPR:
        printf("(%s)\n", node->data.binary.op);
        ast_print_indent(depth + 1);
        printf("left:\n");
        ast_print_node(node->data.binary.left, depth + 2);
        ast_print_indent(depth + 1);
        printf("right:\n");
        ast_print_node(node->data.binary.right, depth + 2);
        break;

    case AST_IF_STMT:
        printf("\n");
        ast_print_indent(depth + 1);
        printf("test:\n");
        ast_print_node(node->data.if_stmt.test, depth + 2);
        ast_print_indent(depth + 1);
        printf("consequent:\n");
        ast_print_node(node->data.if_stmt.consequent, depth + 2);
        if (node->data.if_stmt.alternate)
        {
            ast_print_indent(depth + 1);
            printf("alternate:\n");
            ast_print_node(node->data.if_stmt.alternate, depth + 2);
        }
        break;

    case AST_RETURN_STMT:
        printf("\n");
        if (node->data.return_stmt.argument)
        {
            ast_print_indent(depth + 1);
            printf("argument:\n");
            ast_print_node(node->data.return_stmt.argument, depth + 2);
        }
        break;

    case AST_EXPR_STMT:
        printf("\n");
        ast_print_node(node->data.expr_stmt.expression, depth + 1);
        break;

    case AST_EMPTY_STMT:
        printf("\n");
        break;

    default:
        printf(" (details omitted)\n");
        break;
    }
}

void ast_print(ASTNode *node)
{
    printf("=== AST Dump ===\n");
    ast_print_node(node, 0);
}

/* ==================== 释放 AST ==================== */

void ast_free(ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_PROGRAM:
        ast_list_free(node->data.program.body);
        break;

    case AST_BLOCK:
        ast_list_free(node->data.block.body);
        break;

    case AST_VAR_DECL:
        free(node->data.var_decl.name);
        ast_free(node->data.var_decl.init);
        break;

    case AST_FUNCTION_DECL:
        free(node->data.function_decl.name);
        ast_list_free(node->data.function_decl.params);
        ast_free(node->data.function_decl.body);
        break;

    case AST_RETURN_STMT:
        ast_free(node->data.return_stmt.argument);
        break;

    case AST_IF_STMT:
        ast_free(node->data.if_stmt.test);
        ast_free(node->data.if_stmt.consequent);
        ast_free(node->data.if_stmt.alternate);
        break;

    case AST_FOR_STMT:
        ast_free(node->data.for_stmt.init);
        ast_free(node->data.for_stmt.test);
        ast_free(node->data.for_stmt.update);
        ast_free(node->data.for_stmt.body);
        break;

    case AST_WHILE_STMT:
        ast_free(node->data.while_stmt.test);
        ast_free(node->data.while_stmt.body);
        break;

    case AST_DO_WHILE_STMT:
        ast_free(node->data.do_while_stmt.body);
        ast_free(node->data.do_while_stmt.test);
        break;

    case AST_SWITCH_STMT:
        ast_free(node->data.switch_stmt.discriminant);
        ast_list_free(node->data.switch_stmt.cases);
        break;

    case AST_TRY_STMT:
        ast_free(node->data.try_stmt.block);
        ast_free(node->data.try_stmt.handler);
        ast_free(node->data.try_stmt.finalizer);
        break;

    case AST_WITH_STMT:
        ast_free(node->data.with_stmt.object);
        ast_free(node->data.with_stmt.body);
        break;

    case AST_LABELED_STMT:
        free(node->data.labeled_stmt.label);
        ast_free(node->data.labeled_stmt.body);
        break;

    case AST_BREAK_STMT:
        free(node->data.break_stmt.label);
        break;

    case AST_CONTINUE_STMT:
        free(node->data.continue_stmt.label);
        break;

    case AST_THROW_STMT:
        ast_free(node->data.throw_stmt.argument);
        break;

    case AST_EXPR_STMT:
        ast_free(node->data.expr_stmt.expression);
        break;

    case AST_IDENTIFIER:
        free(node->data.identifier.name);
        break;

    case AST_LITERAL:
        if (node->data.literal.literal_type == AST_LITERAL_STRING)
            free(node->data.literal.value.string);
        break;

    case AST_ASSIGN_EXPR:
        ast_free(node->data.assign.left);
        ast_free(node->data.assign.right);
        break;

    case AST_BINARY_EXPR:
        ast_free(node->data.binary.left);
        ast_free(node->data.binary.right);
        break;

    case AST_CONDITIONAL_EXPR:
        ast_free(node->data.conditional.test);
        ast_free(node->data.conditional.consequent);
        ast_free(node->data.conditional.alternate);
        break;

    case AST_SEQUENCE_EXPR:
        ast_list_free(node->data.sequence.elements);
        break;

    case AST_UNARY_EXPR:
        ast_free(node->data.unary.argument);
        break;

    case AST_UPDATE_EXPR:
        ast_free(node->data.update.argument);
        break;

    case AST_CALL_EXPR:
        ast_free(node->data.call_expr.callee);
        ast_list_free(node->data.call_expr.arguments);
        break;

    case AST_MEMBER_EXPR:
        ast_free(node->data.member_expr.object);
        free(node->data.member_expr.property);
        break;

    case AST_ARRAY_LITERAL:
        ast_list_free(node->data.array_literal.elements);
        break;

    case AST_OBJECT_LITERAL:
        ast_list_free(node->data.object_literal.properties);
        break;

    case AST_PROPERTY:
        free(node->data.property.key.name);
        ast_free(node->data.property.value);
        break;

    case AST_SWITCH_CASE:
        ast_free(node->data.switch_case.test);
        ast_list_free(node->data.switch_case.consequent);
        break;

    case AST_CATCH_CLAUSE:
        free(node->data.catch_clause.param);
        ast_free(node->data.catch_clause.body);
        break;

    case AST_EMPTY_STMT:
        break;
    }

    free(node);
}

/* ==================== 遍历 AST ==================== */

void ast_traverse(ASTNode *node, ASTVisitor visitor, void *userdata)
{
    if (!node || !visitor)
        return;

    visitor(node, userdata);

    // 递归遍历子节点（简化实现）
    switch (node->type)
    {
    case AST_PROGRAM:
    {
        ASTList *list = node->data.program.body;
        while (list)
        {
            ast_traverse(list->node, visitor, userdata);
            list = list->next;
        }
        break;
    }
    case AST_IF_STMT:
        ast_traverse(node->data.if_stmt.test, visitor, userdata);
        ast_traverse(node->data.if_stmt.consequent, visitor, userdata);
        ast_traverse(node->data.if_stmt.alternate, visitor, userdata);
        break;
        // 其他节点类型的遍历...
    default:
        break;
    }
}
