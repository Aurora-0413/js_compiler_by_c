/**
 * @file utils.c
 * @brief 通用工具函数实现
 */

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static bool debug_mode = false;

/* ==================== 内存管理 ==================== */

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr && size > 0)
    {
        fatal_error("Memory allocation failed: requested %zu bytes", size);
    }
    return ptr;
}

void *safe_realloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0)
    {
        fatal_error("Memory reallocation failed: requested %zu bytes", size);
    }
    return new_ptr;
}

void *safe_calloc(size_t count, size_t size)
{
    void *ptr = calloc(count, size);
    if (!ptr && count > 0 && size > 0)
    {
        fatal_error("Memory allocation failed: requested %zu items of %zu bytes",
                    count, size);
    }
    return ptr;
}

/* ==================== 字符串操作 ==================== */

char *safe_strdup(const char *str)
{
    if (!str)
    {
        return NULL;
    }

    size_t len = strlen(str);
    char *copy = (char *)safe_malloc(len + 1);
    memcpy(copy, str, len + 1);
    return copy;
}

char *strip_quotes(char *str)
{
    if (!str)
    {
        return NULL;
    }

    size_t len = strlen(str);
    if (len >= 2 && (str[0] == '"' || str[0] == '\''))
    {
        /* 移除首尾引号 */
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }

    return str;
}

char *string_format(const char *format, ...)
{
    va_list args, args_copy;
    va_start(args, format);
    va_copy(args_copy, args);

    /* 计算需要的缓冲区大小 */
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (size < 0)
    {
        va_end(args_copy);
        return NULL;
    }

    /* 分配并格式化字符串 */
    char *result = (char *)safe_malloc(size + 1);
    vsnprintf(result, size + 1, format, args_copy);
    va_end(args_copy);

    return result;
}

/* ==================== 文件操作 ==================== */

char *read_entire_file(const char *filename, size_t *size_out)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        return NULL;
    }

    /* 获取文件大小 */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size < 0)
    {
        fclose(file);
        return NULL;
    }

    /* 分配内存并读取 */
    char *content = (char *)safe_malloc(size + 1);
    size_t bytes_read = fread(content, 1, size, file);
    content[bytes_read] = '\0';

    fclose(file);

    if (size_out)
    {
        *size_out = bytes_read;
    }

    return content;
}

bool file_exists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

/* ==================== 错误处理 ==================== */

void fatal_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "\n[FATAL ERROR] ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    fprintf(stderr, "[WARNING] ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);
}

/* ==================== 调试工具 ==================== */

void debug_print(const char *format, ...)
{
    if (!debug_mode)
    {
        return;
    }

    va_list args;
    va_start(args, format);

    fprintf(stderr, "[DEBUG] ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);
}

void enable_debug_mode(void)
{
    debug_mode = true;
}

void disable_debug_mode(void)
{
    debug_mode = false;
}
