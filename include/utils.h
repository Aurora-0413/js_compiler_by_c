/**
 * @file utils.h
 * @brief 通用工具函数
 * @author JS Compiler Team
 * @date 2025
 */

#ifndef JS_COMPILER_UTILS_H
#define JS_COMPILER_UTILS_H

#include <stddef.h>
#include <stdbool.h>

/* ==================== 内存管理 ==================== */

/**
 * @brief 安全的内存分配
 * @param size 要分配的字节数
 * @return 分配的内存指针
 * @note 如果分配失败，程序会终止
 */
void *safe_malloc(size_t size);

/**
 * @brief 安全的内存重新分配
 * @param ptr 原指针
 * @param size 新的大小
 * @return 重新分配的内存指针
 */
void *safe_realloc(void *ptr, size_t size);

/**
 * @brief 安全的内存分配并清零
 * @param count 元素数量
 * @param size 每个元素的字节数
 * @return 分配的内存指针
 */
void *safe_calloc(size_t count, size_t size);

/* ==================== 字符串操作 ==================== */

/**
 * @brief 安全的字符串复制
 * @param str 源字符串
 * @return 新分配的字符串副本
 */
char *safe_strdup(const char *str);

/**
 * @brief 从字符串中移除引号
 * @param str 源字符串
 * @return 去除引号后的新字符串
 */
char *strip_quotes(char *str);

/**
 * @brief 字符串格式化到新分配的内存
 * @param format 格式字符串
 * @param ... 可变参数
 * @return 格式化后的新字符串
 */
char *string_format(const char *format, ...);

/* ==================== 文件操作 ==================== */

/**
 * @brief 读取整个文件到字符串
 * @param filename 文件路径
 * @param size_out 输出文件大小（可选）
 * @return 文件内容字符串，使用后需要 free
 */
char *read_entire_file(const char *filename, size_t *size_out);

/**
 * @brief 检查文件是否存在
 * @param filename 文件路径
 * @return 如果文件存在返回 true
 */
bool file_exists(const char *filename);

/* ==================== 错误处理 ==================== */

/**
 * @brief 打印错误信息并退出
 * @param format 格式字符串
 * @param ... 可变参数
 */
void fatal_error(const char *format, ...);

/**
 * @brief 打印警告信息
 * @param format 格式字符串
 * @param ... 可变参数
 */
void warning(const char *format, ...);

/* ==================== 调试工具 ==================== */

/**
 * @brief 调试信息输出（仅在调试模式下）
 * @param format 格式字符串
 * @param ... 可变参数
 */
void debug_print(const char *format, ...);

/**
 * @brief 启用调试输出
 */
void enable_debug_mode(void);

/**
 * @brief 禁用调试输出
 */
void disable_debug_mode(void);

#endif /* JS_COMPILER_UTILS_H */
