#include "rpc_utils.h"
#include <regex>

namespace rpc_utils {

// Logger 实现
LogLevel Logger::current_level_ = LogLevel::INFO;

void Logger::set_log_level(LogLevel level) {
    current_level_ = level;
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level >= current_level_) {
        std::cout << "[" << get_timestamp() << "] "
                  << "[" << level_to_string(level) << "] "
                  << message << std::endl;
    }
}

std::string Logger::get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

// Timer 实现
Timer::Timer() : start_time_(std::chrono::high_resolution_clock::now()) {}

void Timer::reset() {
    start_time_ = std::chrono::high_resolution_clock::now();
}

double Timer::elapsed_ms() const {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(now - start_time_).count();
}

double Timer::elapsed_sec() const {
    return elapsed_ms() / 1000.0;
}

// RPCUtils 实现
std::string RPCUtils::format_error(const std::string& func_name, const std::string& error_msg) {
    return "Error in RPC function '" + func_name + "': " + error_msg;
}

bool RPCUtils::is_valid_host(const std::string& host) {
    if (host.empty()) {
        return false;
    }
    
    // 简单的IP地址或主机名验证
    // IPv4 格式
    std::regex ipv4_pattern("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    if (std::regex_match(host, ipv4_pattern)) {
        return true;
    }
    
    // 主机名格式（简化验证）
    std::regex hostname_pattern("^[a-zA-Z0-9.-]+$");
    return std::regex_match(host, hostname_pattern);
}

bool RPCUtils::is_valid_port(uint16_t port) {
    // 0-1023 是保留端口，但技术上可用
    // 这里只检查是否为0
    return port > 0;
}

uint16_t RPCUtils::default_port() {
    return 8080;
}

} // namespace rpc_utils
