#include <iostream>
#include <string>
#include "rpc_client_wrapper.h"
#include "rpc_utils.h"

void print_separator() {
    std::cout << "----------------------------------------" << std::endl;
}

int main(int argc, char* argv[]) {
    // 设置日志级别
    rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::INFO);

    try {
        // 解析命令行参数
        std::string host = "localhost";
        uint16_t port = 8080;
        
        if (argc > 1) {
            host = argv[1];
        }
        if (argc > 2) {
            port = static_cast<uint16_t>(std::stoi(argv[2]));
        }

        rpc_utils::Logger::info("Connecting to RPC server at " + host + ":" + 
                                std::to_string(port));

        // 创建客户端
        rpc_utils::RPCClientWrapper client(host, port, 5000);

        // 等待一小段时间确保连接建立
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (client.is_connected()) {
            rpc_utils::Logger::info("Connected successfully");
        }

        print_separator();

        // 测试算术运算
        rpc_utils::Logger::info("Testing arithmetic operations:");
        
        rpc_utils::Timer timer;
        double result = client.call<double>("add", 10.5, 20.3);
        rpc_utils::Logger::info("add(10.5, 20.3) = " + std::to_string(result) + 
                                " (took " + std::to_string(timer.elapsed_ms()) + " ms)");

        timer.reset();
        result = client.call<double>("subtract", 50.0, 20.0);
        rpc_utils::Logger::info("subtract(50.0, 20.0) = " + std::to_string(result) + 
                                " (took " + std::to_string(timer.elapsed_ms()) + " ms)");

        timer.reset();
        result = client.call<double>("multiply", 5.0, 6.0);
        rpc_utils::Logger::info("multiply(5.0, 6.0) = " + std::to_string(result) + 
                                " (took " + std::to_string(timer.elapsed_ms()) + " ms)");

        timer.reset();
        result = client.call<double>("square", 7.0);
        rpc_utils::Logger::info("square(7.0) = " + std::to_string(result) + 
                                " (took " + std::to_string(timer.elapsed_ms()) + " ms)");

        print_separator();

        // 测试字符串操作
        rpc_utils::Logger::info("Testing string operations:");
        
        timer.reset();
        std::string greeting = client.call<std::string>("greet", "World");
        rpc_utils::Logger::info("greet(\"World\") = \"" + greeting + "\"" + 
                                " (took " + std::to_string(timer.elapsed_ms()) + " ms)");

        print_separator();

        // 测试异步调用
        rpc_utils::Logger::info("Testing async calls:");
        
        timer.reset();
        auto future1 = client.async_call("add", 100.0, 200.0);
        auto future2 = client.async_call("multiply", 10.0, 20.0);
        auto future3 = client.async_call("square", 15.0);

        double async_result1 = future1.get().as<double>();
        double async_result2 = future2.get().as<double>();
        double async_result3 = future3.get().as<double>();

        rpc_utils::Logger::info("async_call results:");
        rpc_utils::Logger::info("  add(100.0, 200.0) = " + std::to_string(async_result1));
        rpc_utils::Logger::info("  multiply(10.0, 20.0) = " + std::to_string(async_result2));
        rpc_utils::Logger::info("  square(15.0) = " + std::to_string(async_result3));
        rpc_utils::Logger::info("  All async calls took " + 
                                std::to_string(timer.elapsed_ms()) + " ms total");

        print_separator();

        // 测试通知（无返回值）
        rpc_utils::Logger::info("Testing notifications:");
        client.send_notification("log_message", "This is a test notification");
        rpc_utils::Logger::info("Notification sent");

        print_separator();

        // 测试错误处理
        rpc_utils::Logger::info("Testing error handling:");
        try {
            result = client.call<double>("divide", 10.0, 0.0);
            rpc_utils::Logger::error("Should have thrown an exception!");
        } catch (const std::exception& e) {
            rpc_utils::Logger::info("Caught expected exception: " + std::string(e.what()));
        }

        print_separator();

        // 测试超时设置
        rpc_utils::Logger::info("Testing timeout:");
        client.set_timeout(1000);  // 1 second timeout
        rpc_utils::Logger::info("Timeout set to 1000ms");
        
        print_separator();

        rpc_utils::Logger::info("All tests completed successfully!");

        // 等待所有异步响应完成
        client.wait_all_responses();

    } catch (const std::exception& e) {
        rpc_utils::Logger::error("Client error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}
