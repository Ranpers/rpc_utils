#include <iostream>
#include <csignal>
#include <atomic>
#include "rpc_server_wrapper.h"
#include "rpc_utils.h"
#include "rpc/this_handler.h"

// 全局标志用于优雅关闭
std::atomic<bool> running(true);
rpc_utils::RPCServerWrapper* g_server = nullptr;

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        rpc_utils::Logger::info("Received shutdown signal, stopping server...");
        running = false;
        if (g_server) {
            g_server->stop();
        }
    }
}

// 示例：简单的算术运算函数
double add(double a, double b) {
    rpc_utils::Logger::debug("add(" + std::to_string(a) + ", " + std::to_string(b) + ")");
    return a + b;
}

double subtract(double a, double b) {
    rpc_utils::Logger::debug("subtract(" + std::to_string(a) + ", " + std::to_string(b) + ")");
    return a - b;
}

double multiply(double a, double b) {
    rpc_utils::Logger::debug("multiply(" + std::to_string(a) + ", " + std::to_string(b) + ")");
    return a * b;
}

double divide(double a, double b) {
    rpc_utils::Logger::debug("divide(" + std::to_string(a) + ", " + std::to_string(b) + ")");
    if (b == 0.0) {
        rpc::this_handler().respond_error(
            std::make_tuple(1, "Division by zero"));
    }
    return a / b;
}

// 示例：返回字符串的函数
std::string greet(const std::string& name) {
    rpc_utils::Logger::debug("greet(" + name + ")");
    return "Hello, " + name + "!";
}

// 示例：无返回值的函数
void log_message(const std::string& message) {
    rpc_utils::Logger::info("Received message: " + message);
}

int main(int argc, char* argv[]) {
    // 设置信号处理
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // 设置日志级别
    rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::DEBUG);

    try {
        // 创建服务器
        uint16_t port = 8080;
        if (argc > 1) {
            port = static_cast<uint16_t>(std::stoi(argv[1]));
        }

        rpc_utils::Logger::info("Creating RPC server on port " + std::to_string(port));
        rpc_utils::RPCServerWrapper server(port);
        g_server = &server;

        // 绑定算术运算函数
        server.bind("add", &add);
        server.bind("subtract", &subtract);
        server.bind("multiply", &multiply);
        server.bind("divide", &divide);

        // 绑定字符串处理函数
        server.bind("greet", &greet);

        // 绑定无返回值函数
        server.bind("log_message", &log_message);

        // 绑定lambda函数
        server.bind("square", [](double x) -> double {
            rpc_utils::Logger::debug("square(" + std::to_string(x) + ")");
            return x * x;
        });

        // 绑定用于停止服务器的函数
        server.bind("shutdown", [&server]() {
            rpc_utils::Logger::info("Shutdown requested via RPC");
            running = false;
            server.stop();
        });

        rpc_utils::Logger::info("Server started successfully on port " + 
                                std::to_string(server.port()));
        rpc_utils::Logger::info("Available functions:");
        rpc_utils::Logger::info("  - add(double, double) -> double");
        rpc_utils::Logger::info("  - subtract(double, double) -> double");
        rpc_utils::Logger::info("  - multiply(double, double) -> double");
        rpc_utils::Logger::info("  - divide(double, double) -> double");
        rpc_utils::Logger::info("  - greet(string) -> string");
        rpc_utils::Logger::info("  - log_message(string) -> void");
        rpc_utils::Logger::info("  - square(double) -> double");
        rpc_utils::Logger::info("  - shutdown() -> void");
        rpc_utils::Logger::info("Press Ctrl+C to stop the server");

        // 运行服务器（阻塞调用）
        server.run();

        rpc_utils::Logger::info("Server stopped");

    } catch (const std::exception& e) {
        rpc_utils::Logger::error("Server error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
