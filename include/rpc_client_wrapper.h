#pragma once

#include <string>
#include <memory>
#include <functional>
#include <exception>
#include "rpc/client.h"
#include "rpc/rpc_error.h"

namespace rpc_utils {

/**
 * @brief RPC客户端封装类
 * 
 * 提供简化的RPC客户端接口，包括连接管理、调用封装和错误处理
 */
class RPCClientWrapper {
public:
    /**
     * @brief 构造函数
     * @param host 服务器地址
     * @param port 服务器端口
     * @param timeout_ms 超时时间（毫秒），默认5000ms
     */
    RPCClientWrapper(const std::string& host, uint16_t port, int64_t timeout_ms = 5000);

    /**
     * @brief 析构函数
     */
    ~RPCClientWrapper();

    // 禁用拷贝
    RPCClientWrapper(const RPCClientWrapper&) = delete;
    RPCClientWrapper& operator=(const RPCClientWrapper&) = delete;

    /**
     * @brief 同步调用RPC函数
     * @tparam R 返回值类型
     * @tparam Args 参数类型
     * @param func_name 函数名
     * @param args 函数参数
     * @return 函数返回值
     * @throws std::runtime_error 调用失败时抛出异常
     */
    template<typename R, typename... Args>
    R call(const std::string& func_name, Args&&... args);

    /**
     * @brief 异步调用RPC函数
     * @tparam Args 参数类型
     * @param func_name 函数名
     * @param args 函数参数
     * @return std::future对象，用于获取异步结果
     */
    template<typename... Args>
    auto async_call(const std::string& func_name, Args&&... args) 
        -> std::future<RPCLIB_MSGPACK::object_handle>;

    /**
     * @brief 发送通知（不等待返回值）
     * @tparam Args 参数类型
     * @param func_name 函数名
     * @param args 函数参数
     */
    template<typename... Args>
    void send_notification(const std::string& func_name, Args&&... args);

    /**
     * @brief 设置超时时间
     * @param timeout_ms 超时时间（毫秒）
     */
    void set_timeout(int64_t timeout_ms);

    /**
     * @brief 清除超时设置
     */
    void clear_timeout();

    /**
     * @brief 获取连接状态
     * @return 连接状态
     */
    rpc::client::connection_state get_connection_state() const;

    /**
     * @brief 等待所有异步响应完成
     */
    void wait_all_responses();

    /**
     * @brief 检查是否已连接
     * @return true if connected
     */
    bool is_connected() const;

private:
    std::unique_ptr<rpc::client> client_;
    std::string host_;
    uint16_t port_;
};

// 模板实现
template<typename R, typename... Args>
R RPCClientWrapper::call(const std::string& func_name, Args&&... args) {
    try {
        return client_->call(func_name, std::forward<Args>(args)...).template as<R>();
    } catch (const rpc::rpc_error& e) {
        std::string error_msg = "RPC call failed for function '" + func_name + "': " + e.what();
        throw std::runtime_error(error_msg);
    } catch (const std::exception& e) {
        std::string error_msg = "Exception in RPC call '" + func_name + "': " + e.what();
        throw std::runtime_error(error_msg);
    }
}

template<typename... Args>
auto RPCClientWrapper::async_call(const std::string& func_name, Args&&... args) 
    -> std::future<RPCLIB_MSGPACK::object_handle> {
    return client_->async_call(func_name, std::forward<Args>(args)...);
}

template<typename... Args>
void RPCClientWrapper::send_notification(const std::string& func_name, Args&&... args) {
    client_->send(func_name, std::forward<Args>(args)...);
}

} // namespace rpc_utils
