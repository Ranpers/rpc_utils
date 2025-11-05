#pragma once

#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include "rpc/server.h"

namespace rpc_utils {

/**
 * @brief RPC服务器封装类
 * 
 * 提供简化的RPC服务器接口，包括服务器生命周期管理、函数绑定和异步运行
 */
class RPCServerWrapper {
public:
    /**
     * @brief 构造函数（监听所有接口）
     * @param port 监听端口
     */
    explicit RPCServerWrapper(uint16_t port);

    /**
     * @brief 构造函数（监听指定地址）
     * @param address 监听地址
     * @param port 监听端口
     */
    RPCServerWrapper(const std::string& address, uint16_t port);

    /**
     * @brief 析构函数
     */
    ~RPCServerWrapper();

    // 禁用拷贝
    RPCServerWrapper(const RPCServerWrapper&) = delete;
    RPCServerWrapper& operator=(const RPCServerWrapper&) = delete;

    /**
     * @brief 绑定函数到RPC服务
     * @tparam F 函数类型
     * @param name 函数名称
     * @param func 要绑定的函数
     */
    template<typename F>
    void bind(const std::string& name, F&& func);

    /**
     * @brief 同步运行服务器（阻塞调用）
     */
    void run();

    /**
     * @brief 异步运行服务器（非阻塞）
     * @param worker_threads 工作线程数，默认为1
     */
    void async_run(size_t worker_threads = 1);

    /**
     * @brief 停止服务器
     */
    void stop();

    /**
     * @brief 设置异常抑制模式
     * @param suppress true表示捕获异常并返回错误给客户端，false表示异常会崩溃服务器
     */
    void suppress_exceptions(bool suppress);

    /**
     * @brief 获取监听端口
     * @return 端口号
     */
    uint16_t port() const;

    /**
     * @brief 关闭所有会话
     */
    void close_all_sessions();

    /**
     * @brief 检查服务器是否正在运行
     * @return true if running
     */
    bool is_running() const;

private:
    std::unique_ptr<rpc::server> server_;
    std::atomic<bool> is_running_;
    std::string address_;
    uint16_t port_;
};

// 模板实现
template<typename F>
void RPCServerWrapper::bind(const std::string& name, F&& func) {
    server_->bind(name, std::forward<F>(func));
}

} // namespace rpc_utils
