#pragma once

#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace rpc_utils {

/**
 * @brief 日志级别
 */
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

/**
 * @brief 简单的日志工具类
 */
class Logger {
public:
    /**
     * @brief 设置全局日志级别
     * @param level 日志级别
     */
    static void set_log_level(LogLevel level);

    /**
     * @brief 记录调试信息
     * @param message 日志消息
     */
    static void debug(const std::string& message);

    /**
     * @brief 记录普通信息
     * @param message 日志消息
     */
    static void info(const std::string& message);

    /**
     * @brief 记录警告信息
     * @param message 日志消息
     */
    static void warning(const std::string& message);

    /**
     * @brief 记录错误信息
     * @param message 日志消息
     */
    static void error(const std::string& message);

private:
    static LogLevel current_level_;
    static void log(LogLevel level, const std::string& message);
    static std::string get_timestamp();
    static std::string level_to_string(LogLevel level);
};

/**
 * @brief 性能计时器
 */
class Timer {
public:
    /**
     * @brief 构造函数，开始计时
     */
    Timer();

    /**
     * @brief 重置计时器
     */
    void reset();

    /**
     * @brief 获取经过的时间（毫秒）
     * @return 经过的时间（毫秒）
     */
    double elapsed_ms() const;

    /**
     * @brief 获取经过的时间（秒）
     * @return 经过的时间（秒）
     */
    double elapsed_sec() const;

private:
    std::chrono::high_resolution_clock::time_point start_time_;
};

/**
 * @brief RPC工具函数
 */
class RPCUtils {
public:
    /**
     * @brief 格式化错误消息
     * @param func_name 函数名
     * @param error_msg 错误消息
     * @return 格式化后的错误消息
     */
    static std::string format_error(const std::string& func_name, const std::string& error_msg);

    /**
     * @brief 检查主机地址是否有效
     * @param host 主机地址
     * @return true if valid
     */
    static bool is_valid_host(const std::string& host);

    /**
     * @brief 检查端口是否有效
     * @param port 端口号
     * @return true if valid
     */
    static bool is_valid_port(uint16_t port);

    /**
     * @brief 生成默认的RPC端口
     * @return 默认端口号
     */
    static uint16_t default_port();
};

} // namespace rpc_utils
