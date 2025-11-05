# RPC Utils - RPC工具类库

这是一个基于rpclib的C++ RPC工具类库，提供了简化和易用的RPC客户端和服务器封装。

## 项目结构

```
rpc_utils/
├── include/                    # 头文件
│   ├── rpc/                   # rpclib头文件（已集成）
│   ├── rpc_client_wrapper.h   # RPC客户端封装类
│   ├── rpc_server_wrapper.h   # RPC服务器封装类
│   └── rpc_utils.h            # 公共工具类（日志、计时器等）
├── lib/                       # 库文件
│   └── librpc.a              # rpclib静态库
├── src/                       # 源文件
│   ├── client/                # 客户端实现
│   │   └── rpc_client_wrapper.cpp
│   ├── server/                # 服务器实现
│   │   └── rpc_server_wrapper.cpp
│   └── common/                # 公共工具实现
│       └── rpc_utils.cpp
├── examples/                  # 示例程序
│   ├── example_server.cpp     # 服务器示例
│   └── example_client.cpp     # 客户端示例
├── CMakeLists.txt            # CMake构建配置
└── README.md                 # 本文件
```

## 功能特性

### RPCClientWrapper - 客户端封装

- ✅ 简化的连接管理
- ✅ 同步和异步函数调用
- ✅ 通知发送（无需返回值的调用）
- ✅ 超时配置
- ✅ 连接状态查询
- ✅ 异常处理和错误封装

### RPCServerWrapper - 服务器封装

- ✅ 简化的服务器初始化
- ✅ 灵活的函数绑定（支持函数指针、lambda、成员函数等）
- ✅ 同步和异步运行模式
- ✅ 优雅关闭
- ✅ 异常抑制模式
- ✅ 会话管理

### 公共工具

- ✅ **Logger**: 分级日志系统（DEBUG, INFO, WARNING, ERROR）
- ✅ **Timer**: 高精度性能计时器
- ✅ **RPCUtils**: 地址验证、错误格式化等工具函数

## 构建说明

### 前置条件

1. C++14或更高版本的编译器（推荐使用Occlum工具链）
2. CMake 3.10或更高版本
3. rpclib库（已包含在本项目的include/rpc和lib/目录中）

### 构建步骤

```bash
# 1. 进入rpc_utils目录
cd /workspaces/te/confidential_computing/rpc_utils

# 2. 创建构建目录
mkdir -p build
cd build

# 3. 配置CMake
cmake ..

# 4. 编译
make

# 5. 编译后会生成以下文件：
#    - librpc_utils_common.a    # 公共工具库
#    - librpc_utils_client.a    # 客户端库
#    - librpc_utils_server.a    # 服务器库
#    - example_server            # 服务器示例程序
#    - example_client            # 客户端示例程序
```

### 编译选项

```bash
# 方式1: 使用标准编译器
cmake ..

# 方式2: 显式使用Occlum编译器（推荐）
cmake -DCMAKE_C_COMPILER=occlum-gcc -DCMAKE_CXX_COMPILER=occlum-g++ ..

# 不编译示例程序
cmake -DBUILD_EXAMPLES=OFF ..

# 指定安装路径
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

## 使用示例

### 服务器端示例

```cpp
#include "rpc_server_wrapper.h"
#include "rpc_utils.h"

// 定义RPC函数
double add(double a, double b) {
    return a + b;
}

int main() {
    // 设置日志级别
    rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::INFO);
    
    // 创建服务器
    rpc_utils::RPCServerWrapper server(8080);
    
    // 绑定函数
    server.bind("add", &add);
    
    // 绑定lambda
    server.bind("multiply", [](double a, double b) {
        return a * b;
    });
    
    // 运行服务器（阻塞）
    rpc_utils::Logger::info("Server starting on port 8080");
    server.run();
    
    return 0;
}
```

### 客户端示例

```cpp
#include "rpc_client_wrapper.h"
#include "rpc_utils.h"

int main() {
    // 创建客户端，连接到localhost:8080，超时5秒
    rpc_utils::RPCClientWrapper client("localhost", 8080, 5000);
    
    // 同步调用
    double result = client.call<double>("add", 10.0, 20.0);
    rpc_utils::Logger::info("add(10, 20) = " + std::to_string(result));
    
    // 异步调用
    auto future = client.async_call("multiply", 5.0, 6.0);
    // 做其他事情...
    double async_result = future.get().as<double>();
    rpc_utils::Logger::info("multiply(5, 6) = " + std::to_string(async_result));
    
    // 发送通知（不等待返回）
    client.send_notification("log", "Hello from client");
    
    return 0;
}
```

## 运行示例程序

### 1. 启动服务器

```bash
# 使用默认端口8080
./example_server

# 或指定端口
./example_server 9000
```

服务器启动后会显示：
```
[2025-11-05 12:00:00.000] [INFO] Server started successfully on port 8080
[2025-11-05 12:00:00.000] [INFO] Available functions:
[2025-11-05 12:00:00.000] [INFO]   - add(double, double) -> double
[2025-11-05 12:00:00.000] [INFO]   - subtract(double, double) -> double
...
```

### 2. 运行客户端

在另一个终端中：

```bash
# 连接到localhost:8080
./example_client

# 或指定服务器地址和端口
./example_client 127.0.0.1 9000
```

客户端会执行一系列测试并显示结果：
```
[2025-11-05 12:01:00.000] [INFO] Connected successfully
----------------------------------------
[2025-11-05 12:01:00.000] [INFO] Testing arithmetic operations:
[2025-11-05 12:01:00.100] [INFO] add(10.5, 20.3) = 30.800000 (took 2.5 ms)
...
```

## API文档

### RPCClientWrapper

```cpp
// 构造函数
RPCClientWrapper(const std::string& host, uint16_t port, int64_t timeout_ms = 5000);

// 同步调用
template<typename R, typename... Args>
R call(const std::string& func_name, Args&&... args);

// 异步调用
template<typename... Args>
auto async_call(const std::string& func_name, Args&&... args) 
    -> std::future<RPCLIB_MSGPACK::object_handle>;

// 发送通知
template<typename... Args>
void send_notification(const std::string& func_name, Args&&... args);

// 超时管理
void set_timeout(int64_t timeout_ms);
void clear_timeout();

// 状态查询
bool is_connected() const;
rpc::client::connection_state get_connection_state() const;
void wait_all_responses();
```

### RPCServerWrapper

```cpp
// 构造函数
explicit RPCServerWrapper(uint16_t port);
RPCServerWrapper(const std::string& address, uint16_t port);

// 绑定函数
template<typename F>
void bind(const std::string& name, F&& func);

// 运行控制
void run();                              // 同步运行（阻塞）
void async_run(size_t worker_threads);   // 异步运行
void stop();                             // 停止服务器

// 配置
void suppress_exceptions(bool suppress);
uint16_t port() const;

// 会话管理
void close_all_sessions();
bool is_running() const;
```

### Logger

```cpp
// 设置日志级别
static void set_log_level(LogLevel level);

// 日志记录
static void debug(const std::string& message);
static void info(const std::string& message);
static void warning(const std::string& message);
static void error(const std::string& message);
```

### Timer

```cpp
Timer();                      // 构造时开始计时
void reset();                 // 重置计时器
double elapsed_ms() const;    // 获取经过的毫秒数
double elapsed_sec() const;   // 获取经过的秒数
```

## 错误处理

所有RPC调用失败都会抛出`std::runtime_error`异常，包含详细的错误信息：

```cpp
try {
    double result = client.call<double>("divide", 10.0, 0.0);
} catch (const std::runtime_error& e) {
    // 错误信息格式：
    // "RPC call failed for function 'divide': Division by zero"
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## 最佳实践

1. **异常处理**: 总是使用try-catch包裹RPC调用
2. **超时设置**: 为长时间运行的操作设置合适的超时时间
3. **日志级别**: 在生产环境使用INFO或WARNING级别，开发时使用DEBUG级别
4. **异步调用**: 对于批量操作，使用async_call可以提高性能
5. **资源清理**: 使用RAII，让对象析构时自动清理资源

## 注意事项

- 所有通过RPC传递的类型必须支持msgpack序列化
- 自定义类型需要使用`MSGPACK_DEFINE`宏定义序列化
- 客户端和服务器必须对函数签名有一致的理解
- 默认端口8080可能与其他服务冲突，请根据需要修改

## 许可证

本项目遵循rpclib的许可证。

## 贡献

欢迎提交Issue和Pull Request！

## 联系方式

如有问题，请通过Issue联系。
