#include "rpc_server_wrapper.h"
#include <stdexcept>

namespace rpc_utils {

RPCServerWrapper::RPCServerWrapper(uint16_t port)
    : port_(port), is_running_(false) {
    try {
        server_ = std::make_unique<rpc::server>(port);
        // 默认启用异常抑制，这样服务器不会因为处理函数的异常而崩溃
        server_->suppress_exceptions(true);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create RPC server: " + std::string(e.what()));
    }
}

RPCServerWrapper::RPCServerWrapper(const std::string& address, uint16_t port)
    : address_(address), port_(port), is_running_(false) {
    try {
        server_ = std::make_unique<rpc::server>(address, port);
        // 默认启用异常抑制
        server_->suppress_exceptions(true);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create RPC server: " + std::string(e.what()));
    }
}

RPCServerWrapper::~RPCServerWrapper() {
    if (is_running_) {
        stop();
    }
}

void RPCServerWrapper::run() {
    is_running_ = true;
    server_->run();
    is_running_ = false;
}

void RPCServerWrapper::async_run(size_t worker_threads) {
    is_running_ = true;
    server_->async_run(worker_threads);
}

void RPCServerWrapper::stop() {
    if (is_running_) {
        server_->stop();
        is_running_ = false;
    }
}

void RPCServerWrapper::suppress_exceptions(bool suppress) {
    server_->suppress_exceptions(suppress);
}

uint16_t RPCServerWrapper::port() const {
    return server_->port();
}

void RPCServerWrapper::close_all_sessions() {
    server_->close_sessions();
}

bool RPCServerWrapper::is_running() const {
    return is_running_;
}

} // namespace rpc_utils
