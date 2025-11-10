#include "ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ASTNode *ast_alloc(ASTNodeType type) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Out of memory while constructing AST\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    return node;
}

static char *strip_quotes(char *text) {
    if (!text) {
        return NULL;
    }
    size_t len = strlen(text);
    if (len >= 2 && (text[0] == '"' || text[0] == '\'')) {
        memmove(text, text + 1, len - 2);
        text[len - 2] = '\0';
    }
    return text;
}

ASTList *ast_list_append(ASTList *list, ASTNode *node) {
    if (!node) {
        return list;
    }
    ASTList *item = (ASTList *)calloc(1, sizeof(ASTList));
    if (!item) {
        fprintf(stderr, "Out of memory while constructing AST list\n");
        exit(EXIT_FAILURE);
    }
    item->node = node;
    if (!list) {
        return item;
    }
    ASTList *tail = list;
    while (tail->next) {
        tail = tail->next;
    }
    tail->next = item;
    return list;
}

ASTList *ast_list_concat(ASTList *head, ASTList *tail) {
    if (!head) {
        return tail;
    }
    ASTList *iter = head;
    while (iter->next) {
        iter = iter->next;
    }
    iter->next = tail;
    return head;
}

void ast_list_free(ASTList *list) {
    while (list) {
        ASTList *next = list->next;
        if (list->node) {
            ast_free(list->node);
        }
        free(list);
        list = next;
    }
}

ASTNode *ast_make_program(ASTList *body) {
    ASTNode *node = ast_alloc(AST_PROGRAM);
    node->data.program.body = body;
    return node;
}

ASTNode *ast_make_block(ASTList *body) {
    ASTNode *node = ast_alloc(AST_BLOCK);
    node->data.block.body = body;
    return node;
}

ASTNode *ast_make_var_decl(ASTVarKind kind, char *name, ASTNode *init) {
    ASTNode *node = ast_alloc(AST_VAR_DECL);
    node->data.var_decl.kind = kind;
    node->data.var_decl.name = name;
    node->data.var_decl.init = init;
    return node;
}

ASTNode *ast_make_function_decl(char *name, ASTList *params, ASTNode *body) {
    ASTNode *node = ast_alloc(AST_FUNCTION_DECL);
    node->data.function_decl.name = name;
    node->data.function_decl.params = params;
    node->data.function_decl.body = body;
    return node;
}

ASTNode *ast_make_return(ASTNode *argument) {
    ASTNode *node = ast_alloc(AST_RETURN_STMT);
    node->data.return_stmt.argument = argument;
    return node;
}

ASTNode *ast_make_if(ASTNode *test, ASTNode *consequent, ASTNode *alternate) {
    ASTNode *node = ast_alloc(AST_IF_STMT);
    node->data.if_stmt.test = test;
    node->data.if_stmt.consequent = consequent;
    node->data.if_stmt.alternate = alternate;
    return node;
}

ASTNode *ast_make_for(ASTNode *init, ASTNode *test, ASTNode *update, ASTNode *body) {
    ASTNode *node = ast_alloc(AST_FOR_STMT);
    node->data.for_stmt.init = init;
    node->data.for_stmt.test = test;
    node->data.for_stmt.update = update;
    node->data.for_stmt.body = body;
    return node;
}

ASTNode *ast_make_expression_stmt(ASTNode *expression) {
    ASTNode *node = ast_alloc(AST_EXPR_STMT);
    node->data.expr_stmt.expression = expression;
    return node;
}

ASTNode *ast_make_empty_statement(void) {
    return ast_alloc(AST_EMPTY_STMT);
}

ASTNode *ast_make_identifier(char *name) {
    ASTNode *node = ast_alloc(AST_IDENTIFIER);
    node->data.identifier.name = name;
    return node;
}

ASTNode *ast_make_number_literal(char *raw) {
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_NUMBER;
    if (raw) {
        node->data.literal.value.number = strtod(raw, NULL);
        free(raw);
    } else {
        node->data.literal.value.number = 0.0;
    }
    return node;
}

ASTNode *ast_make_string_literal(char *raw) {
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_STRING;
    node->data.literal.value.string = strip_quotes(raw);
    return node;
}

ASTNode *ast_make_boolean_literal(bool value) {
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_BOOLEAN;
    node->data.literal.value.boolean = value;
    return node;
}

ASTNode *ast_make_null_literal(void) {
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_NULL;
    return node;
}

ASTNode *ast_make_undefined_literal(void) {
    ASTNode *node = ast_alloc(AST_LITERAL);
    node->data.literal.literal_type = AST_LITERAL_UNDEFINED;
    return node;
}

ASTNode *ast_make_assignment(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = ast_alloc(AST_ASSIGN_EXPR);
    node->data.assign.op = op;
    node->data.assign.left = left;
    node->data.assign.right = right;
    return node;
}

ASTNode *ast_make_binary(const char *op, ASTNode *left, ASTNode *right) {
    ASTNode *node = ast_alloc(AST_BINARY_EXPR);
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ASTNode *ast_make_unary(const char *op, ASTNode *argument) {
    ASTNode *node = ast_alloc(AST_UNARY_EXPR);
    node->data.unary.op = op;
    node->data.unary.argument = argument;
    return node;
}

ASTNode *ast_make_update(const char *op, ASTNode *argument, bool prefix) {
    ASTNode *node = ast_alloc(AST_UPDATE_EXPR);
    node->data.update.op = op;
    node->data.update.argument = argument;
    node->data.update.prefix = prefix;
    return node;
}

ASTNode *ast_make_call(ASTNode *callee, ASTList *arguments) {
    ASTNode *node = ast_alloc(AST_CALL_EXPR);
    node->data.call_expr.callee = callee;
    node->data.call_expr.arguments = arguments;
    return node;
}

ASTNode *ast_make_member(ASTNode *object, char *property, bool computed) {
    ASTNode *node = ast_alloc(AST_MEMBER_EXPR);
    node->data.member_expr.object = object;
    node->data.member_expr.property = property;
    node->data.member_expr.computed = computed;
    return node;
}

ASTNode *ast_make_array_literal(ASTList *elements) {
    ASTNode *node = ast_alloc(AST_ARRAY_LITERAL);
    node->data.array_literal.elements = elements;
    return node;
}

ASTNode *ast_make_object_literal(ASTList *properties) {
    ASTNode *node = ast_alloc(AST_OBJECT_LITERAL);
    node->data.object_literal.properties = properties;
    return node;
}

ASTNode *ast_make_property(char *key, bool is_identifier, ASTNode *value) {
    ASTNode *node = ast_alloc(AST_PROPERTY);
    node->data.property.key.name = is_identifier ? key : strip_quotes(key);
    node->data.property.key.is_identifier = is_identifier;
    node->data.property.value = value;
    return node;
}

static void ast_traverse_list(ASTList *list, ASTVisitFn visitor, void *userdata) {
    for (ASTList *iter = list; iter; iter = iter->next) {
        if (iter->node) {
            ast_traverse(iter->node, visitor, userdata);
        }
    }
}

void ast_traverse(ASTNode *node, ASTVisitFn visitor, void *userdata) {
    if (!node || !visitor) {
        return;
    }
    visitor(node, userdata);
    switch (node->type) {
        case AST_PROGRAM:
            ast_traverse_list(node->data.program.body, visitor, userdata);
            break;
        case AST_BLOCK:
            ast_traverse_list(node->data.block.body, visitor, userdata);
            break;
        case AST_VAR_DECL:
            ast_traverse(node->data.var_decl.init, visitor, userdata);
            break;
        case AST_FUNCTION_DECL:
            ast_traverse_list(node->data.function_decl.params, visitor, userdata);
            ast_traverse(node->data.function_decl.body, visitor, userdata);
            break;
        case AST_RETURN_STMT:
            ast_traverse(node->data.return_stmt.argument, visitor, userdata);
            break;
        case AST_IF_STMT:
            ast_traverse(node->data.if_stmt.test, visitor, userdata);
            ast_traverse(node->data.if_stmt.consequent, visitor, userdata);
            ast_traverse(node->data.if_stmt.alternate, visitor, userdata);
            break;
        case AST_FOR_STMT:
            ast_traverse(node->data.for_stmt.init, visitor, userdata);
            ast_traverse(node->data.for_stmt.test, visitor, userdata);
            ast_traverse(node->data.for_stmt.update, visitor, userdata);
            ast_traverse(node->data.for_stmt.body, visitor, userdata);
            break;
        case AST_EXPR_STMT:
            ast_traverse(node->data.expr_stmt.expression, visitor, userdata);
            break;
        case AST_ASSIGN_EXPR:
            ast_traverse(node->data.assign.left, visitor, userdata);
            ast_traverse(node->data.assign.right, visitor, userdata);
            break;
        case AST_BINARY_EXPR:
            ast_traverse(node->data.binary.left, visitor, userdata);
            ast_traverse(node->data.binary.right, visitor, userdata);
            break;
        case AST_UNARY_EXPR:
            ast_traverse(node->data.unary.argument, visitor, userdata);
            break;
        case AST_UPDATE_EXPR:
            ast_traverse(node->data.update.argument, visitor, userdata);
            break;
        case AST_CALL_EXPR:
            ast_traverse(node->data.call_expr.callee, visitor, userdata);
            ast_traverse_list(node->data.call_expr.arguments, visitor, userdata);
            break;
        case AST_MEMBER_EXPR:
            ast_traverse(node->data.member_expr.object, visitor, userdata);
            break;
        case AST_ARRAY_LITERAL:
            ast_traverse_list(node->data.array_literal.elements, visitor, userdata);
            break;
        case AST_OBJECT_LITERAL:
            ast_traverse_list(node->data.object_literal.properties, visitor, userdata);
            break;
        case AST_PROPERTY:
            ast_traverse(node->data.property.value, visitor, userdata);
            break;
        case AST_EMPTY_STMT:
        case AST_IDENTIFIER:
        case AST_LITERAL:
            break;
    }
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; ++i) {
        putchar(' ');
    }
}

static const char *var_kind_to_string(ASTVarKind kind) {
    switch (kind) {
        case AST_VAR_KIND_VAR:   return "var";
        case AST_VAR_KIND_LET:   return "let";
        case AST_VAR_KIND_CONST: return "const";
        default:                 return "unknown";
    }
}

static void ast_print_internal(const ASTNode *node, int indent);

static void ast_print_list(const ASTList *list, int indent) {
    for (const ASTList *iter = list; iter; iter = iter->next) {
        ast_print_internal(iter->node, indent);
    }
}

static void ast_print_internal(const ASTNode *node, int indent) {
    if (!node) {
        return;
    }
    switch (node->type) {
        case AST_PROGRAM:
            print_indent(indent);
            printf("Program\n");
            ast_print_list(node->data.program.body, indent + 2);
            break;
        case AST_BLOCK:
            print_indent(indent);
            printf("BlockStatement\n");
            ast_print_list(node->data.block.body, indent + 2);
            break;
        case AST_VAR_DECL:
            print_indent(indent);
            printf("VariableDeclaration kind=%s name=%s\n",
                   var_kind_to_string(node->data.var_decl.kind),
                   node->data.var_decl.name ? node->data.var_decl.name : "<anonymous>");
            ast_print_internal(node->data.var_decl.init, indent + 2);
            break;
        case AST_FUNCTION_DECL:
            print_indent(indent);
            printf("FunctionDeclaration name=%s\n",
                   node->data.function_decl.name ? node->data.function_decl.name : "<anonymous>");
            if (node->data.function_decl.params) {
                print_indent(indent + 2);
                printf("Params\n");
                ast_print_list(node->data.function_decl.params, indent + 4);
            }
            print_indent(indent + 2);
            printf("Body\n");
            ast_print_internal(node->data.function_decl.body, indent + 4);
            break;
        case AST_RETURN_STMT:
            print_indent(indent);
            printf("ReturnStatement\n");
            ast_print_internal(node->data.return_stmt.argument, indent + 2);
            break;
        case AST_IF_STMT:
            print_indent(indent);
            printf("IfStatement\n");
            print_indent(indent + 2);
            printf("Test\n");
            ast_print_internal(node->data.if_stmt.test, indent + 4);
            print_indent(indent + 2);
            printf("Consequent\n");
            ast_print_internal(node->data.if_stmt.consequent, indent + 4);
            if (node->data.if_stmt.alternate) {
                print_indent(indent + 2);
                printf("Alternate\n");
                ast_print_internal(node->data.if_stmt.alternate, indent + 4);
            }
            break;
        case AST_FOR_STMT:
            print_indent(indent);
            printf("ForStatement\n");
            print_indent(indent + 2);
            printf("Init\n");
            ast_print_internal(node->data.for_stmt.init, indent + 4);
            print_indent(indent + 2);
            printf("Test\n");
            ast_print_internal(node->data.for_stmt.test, indent + 4);
            print_indent(indent + 2);
            printf("Update\n");
            ast_print_internal(node->data.for_stmt.update, indent + 4);
            print_indent(indent + 2);
            printf("Body\n");
            ast_print_internal(node->data.for_stmt.body, indent + 4);
            break;
        case AST_EXPR_STMT:
            print_indent(indent);
            printf("ExpressionStatement\n");
            ast_print_internal(node->data.expr_stmt.expression, indent + 2);
            break;
        case AST_EMPTY_STMT:
            print_indent(indent);
            printf("EmptyStatement\n");
            break;
        case AST_IDENTIFIER:
            print_indent(indent);
            printf("Identifier name=%s\n", node->data.identifier.name ? node->data.identifier.name : "<unnamed>");
            break;
        case AST_LITERAL:
            print_indent(indent);
            switch (node->data.literal.literal_type) {
                case AST_LITERAL_NUMBER:
                    printf("NumericLiteral value=%g\n", node->data.literal.value.number);
                    break;
                case AST_LITERAL_STRING:
                    printf("StringLiteral value=\"%s\"\n", node->data.literal.value.string ? node->data.literal.value.string : "");
                    break;
                case AST_LITERAL_BOOLEAN:
                    printf("BooleanLiteral value=%s\n", node->data.literal.value.boolean ? "true" : "false");
                    break;
                case AST_LITERAL_NULL:
                    printf("NullLiteral\n");
                    break;
                case AST_LITERAL_UNDEFINED:
                    printf("UndefinedLiteral\n");
                    break;
            }
            break;
        case AST_ASSIGN_EXPR:
            print_indent(indent);
            printf("AssignmentExpression op=%s\n", node->data.assign.op ? node->data.assign.op : "=");
            print_indent(indent + 2);
            printf("Left\n");
            ast_print_internal(node->data.assign.left, indent + 4);
            print_indent(indent + 2);
            printf("Right\n");
            ast_print_internal(node->data.assign.right, indent + 4);
            break;
        case AST_BINARY_EXPR:
            print_indent(indent);
            printf("BinaryExpression op=%s\n", node->data.binary.op ? node->data.binary.op : "");
            print_indent(indent + 2);
            printf("Left\n");
            ast_print_internal(node->data.binary.left, indent + 4);
            print_indent(indent + 2);
            printf("Right\n");
            ast_print_internal(node->data.binary.right, indent + 4);
            break;
        case AST_UNARY_EXPR:
            print_indent(indent);
            printf("UnaryExpression op=%s\n", node->data.unary.op ? node->data.unary.op : "");
            ast_print_internal(node->data.unary.argument, indent + 2);
            break;
        case AST_UPDATE_EXPR:
            print_indent(indent);
            printf("UpdateExpression op=%s %s\n",
                   node->data.update.op ? node->data.update.op : "",
                   node->data.update.prefix ? "(prefix)" : "(postfix)");
            ast_print_internal(node->data.update.argument, indent + 2);
            break;
        case AST_CALL_EXPR:
            print_indent(indent);
            printf("CallExpression\n");
            print_indent(indent + 2);
            printf("Callee\n");
            ast_print_internal(node->data.call_expr.callee, indent + 4);
            if (node->data.call_expr.arguments) {
                print_indent(indent + 2);
                printf("Arguments\n");
                ast_print_list(node->data.call_expr.arguments, indent + 4);
            }
            break;
        case AST_MEMBER_EXPR:
            print_indent(indent);
            printf("MemberExpression property=%s\n",
                   node->data.member_expr.property ? node->data.member_expr.property : "<computed>");
            print_indent(indent + 2);
            printf("Object\n");
            ast_print_internal(node->data.member_expr.object, indent + 4);
            break;
        case AST_ARRAY_LITERAL:
            print_indent(indent);
            printf("ArrayLiteral\n");
            if (node->data.array_literal.elements) {
                print_indent(indent + 2);
                printf("Elements\n");
                ast_print_list(node->data.array_literal.elements, indent + 4);
            }
            break;
        case AST_OBJECT_LITERAL:
            print_indent(indent);
            printf("ObjectLiteral\n");
            if (node->data.object_literal.properties) {
                print_indent(indent + 2);
                printf("Properties\n");
                ast_print_list(node->data.object_literal.properties, indent + 4);
            }
            break;
        case AST_PROPERTY:
            print_indent(indent);
            printf("Property key=%s%s\n",
                   node->data.property.key.name ? node->data.property.key.name : "<unknown>",
                   node->data.property.key.is_identifier ? " (identifier)" : "");
            ast_print_internal(node->data.property.value, indent + 2);
            break;
    }
}

void ast_print(const ASTNode *node) {
    ast_print_internal(node, 0);
}

void ast_free(ASTNode *node) {
    if (!node) {
        return;
    }
    switch (node->type) {
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
        case AST_EXPR_STMT:
            ast_free(node->data.expr_stmt.expression);
            break;
        case AST_EMPTY_STMT:
            break;
        case AST_IDENTIFIER:
            free(node->data.identifier.name);
            break;
        case AST_LITERAL:
            if (node->data.literal.literal_type == AST_LITERAL_STRING) {
                free(node->data.literal.value.string);
            }
            break;
        case AST_ASSIGN_EXPR:
            ast_free(node->data.assign.left);
            ast_free(node->data.assign.right);
            break;
        case AST_BINARY_EXPR:
            ast_free(node->data.binary.left);
            ast_free(node->data.binary.right);
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
    }
    free(node);
}
