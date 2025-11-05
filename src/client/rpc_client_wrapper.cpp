#include "rpc_client_wrapper.h"
#include <stdexcept>

namespace rpc_utils {

RPCClientWrapper::RPCClientWrapper(const std::string& host, uint16_t port, int64_t timeout_ms)
    : host_(host), port_(port) {
    try {
        client_ = std::make_unique<rpc::client>(host, port);
        if (timeout_ms > 0) {
            client_->set_timeout(timeout_ms);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create RPC client: " + std::string(e.what()));
    }
}

RPCClientWrapper::~RPCClientWrapper() {
    // 客户端析构时会自动断开连接
}

void RPCClientWrapper::set_timeout(int64_t timeout_ms) {
    client_->set_timeout(timeout_ms);
}

void RPCClientWrapper::clear_timeout() {
    client_->clear_timeout();
}

rpc::client::connection_state RPCClientWrapper::get_connection_state() const {
    return client_->get_connection_state();
}

void RPCClientWrapper::wait_all_responses() {
    client_->wait_all_responses();
}

bool RPCClientWrapper::is_connected() const {
    return get_connection_state() == rpc::client::connection_state::connected;
}

} // namespace rpc_utils
