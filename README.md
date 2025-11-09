# RPC Utils

[![C++](https://img.shields.io/badge/C++-14-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-orange.svg)](LICENSE)

> 基于 rpclib 的现代 C++ RPC 工具库，提供简洁易用的客户端和服务器封装

## ✨ 特性

- 🚀 **简单易用** - 封装复杂的 RPC 细节，提供直观的 API
- 🔄 **同步/异步** - 支持同步调用和异步调用两种模式
- 📝 **内置日志** - 分级日志系统，便于调试和监控
- ⏱️ **性能计时** - 高精度计时器，方便性能分析
- 🛡️ **异常安全** - 完善的错误处理和异常封装
- 🔧 **灵活配置** - 支持多种编译器和构建选项
- 📦 **自动依赖管理** - 构建时自动拉取并编译最新 rpclib

## 🚀 快速开始

### 1. 构建项目

构建脚本会自动拉取最新的 rpclib 并使用相同的编译器参数构建：

```bash
cd /workspaces/te/confidential_computing/rpc_utils
./build.sh
```

> 💡 **注意**: 首次构建时会自动克隆 rpclib 仓库到 `external/rpclib` 目录，后续构建会更新到最新版本。

### 2. 启动服务器

```bash
cd build
./example_server
```

### 3. 运行客户端

在新终端中执行：

```bash
cd build
./example_client
```

完整教程请参见 [QUICKSTART.md](QUICKSTART.md)

## 📁 项目结构

```
rpc_utils/
├── external/                    # 外部依赖（自动管理，已加入 .gitignore）
│   └── rpclib/                 # rpclib 源码（构建时自动拉取）
├── include/                     # 头文件目录
│   ├── rpc_client_wrapper.h    # 客户端封装
│   ├── rpc_server_wrapper.h    # 服务器封装
│   └── rpc_utils.h             # 工具类（日志、计时器）
├── src/                        # 源代码目录
│   ├── client/                 # 客户端实现
│   ├── server/                 # 服务器实现
│   └── common/                 # 公共工具实现
├── examples/                   # 示例程序
│   ├── example_server.cpp
│   └── example_client.cpp
├── build.sh                    # 构建脚本（自动拉取并构建 rpclib）
├── CMakeLists.txt             # CMake 配置
├── README.md                  # 本文档
└── QUICKSTART.md              # 快速入门指南
```

## 🎯 核心组件

### RPCClientWrapper - 客户端

| 功能 | 描述 |
|------|------|
| 🔗 连接管理 | 自动管理连接生命周期 |
| 📞 同步调用 | 阻塞式远程函数调用 |
| ⚡ 异步调用 | 非阻塞式远程函数调用 |
| 📢 通知发送 | 单向消息发送（无返回值） |
| ⏰ 超时控制 | 可配置的调用超时时间 |
| 🔍 状态查询 | 实时连接状态监控 |

### RPCServerWrapper - 服务器

| 功能 | 描述 |
|------|------|
| 🎯 函数绑定 | 支持函数、lambda、成员函数 |
| 🔄 运行模式 | 同步阻塞 / 异步多线程 |
| 🛑 优雅关闭 | 安全的服务器停止机制 |
| 🛡️ 异常处理 | 可配置的异常抑制模式 |
| 👥 会话管理 | 多客户端连接管理 |

### 工具类

| 工具 | 功能 |
|------|------|
| 📝 Logger | DEBUG / INFO / WARNING / ERROR 四级日志 |
| ⏱️ Timer | 高精度毫秒/秒级计时器 |
| 🔧 RPCUtils | 地址验证、错误格式化等实用函数 |

## 🔨 构建指南

### 系统要求

- **编译器**: C++14 或更高版本
  - Occlum GCC/G++ (用于机密计算)
  - GCC/G++ 7.0+
  - Clang/Clang++ 5.0+
- **构建工具**: CMake 3.10+
- **网络工具**: Git（用于拉取 rpclib）
- **依赖库**: rpclib（构建时自动拉取）

### 方式一：使用构建脚本（推荐）

```bash
# 基本用法
./build.sh                        # 使用默认 Occlum 编译器
./build.sh clean                  # 清理后重新构建

# 指定编译器类型
./build.sh --compiler=gcc         # 使用系统 gcc/g++
./build.sh --compiler=clang       # 使用 clang/clang++
./build.sh --compiler=occlum      # 使用 Occlum 编译器（默认）

# 显式指定编译器路径
./build.sh --cc=gcc-9 --cxx=g++-9

# 组合使用
./build.sh clean --compiler=gcc

# 查看帮助
./build.sh --help
```

### 方式二：直接使用 CMake

```bash
mkdir -p build && cd build

# 使用默认编译器
cmake ..

# 指定编译器
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..

# 额外选项
cmake -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_EXAMPLES=ON \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      ..

# 编译
make -j$(nproc)
```

### 构建输出

编译成功后会在 `build/` 目录生成：

```
build/
├── librpc_utils_common.a        # 公共工具库
├── librpc_utils_client.a        # 客户端库
├── librpc_utils_server.a        # 服务器库
├── example_server                # 服务器示例
└── example_client                # 客户端示例
```

## 📖 使用示例

### 服务器端开发

#### 基础示例

```cpp
#include "rpc_server_wrapper.h"
#include "rpc_utils.h"

// 定义业务函数
double add(double a, double b) {
    return a + b;
}

int main() {
    // 配置日志
    rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::INFO);
    
    // 创建服务器（监听 8080 端口）
    rpc_utils::RPCServerWrapper server(8080);
    
    // 绑定函数
    server.bind("add", &add);
    
    // 绑定 Lambda 表达式
    server.bind("multiply", [](double a, double b) {
        return a * b;
    });
    
    // 启动服务器（阻塞模式）
    rpc_utils::Logger::info("Server starting on port 8080");
    server.run();
    
    return 0;
}
```

#### 高级用法

```cpp
#include "rpc_server_wrapper.h"

class Calculator {
public:
    double divide(double a, double b) {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    }
};

int main() {
    rpc_utils::RPCServerWrapper server("0.0.0.0", 8080);
    Calculator calc;
    
    // 绑定成员函数
    server.bind("divide", [&calc](double a, double b) {
        return calc.divide(a, b);
    });
    
    // 绑定复杂类型
    server.bind("sum_vector", [](const std::vector<int>& nums) {
        return std::accumulate(nums.begin(), nums.end(), 0);
    });
    
    // 异步运行（4个工作线程）
    server.async_run(4);
    
    // 主线程可以做其他事情
    std::this_thread::sleep_for(std::chrono::hours(24));
    
    server.stop();
    return 0;
}
```

### 客户端开发

#### 基础示例

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

## 🔧 API 参考

### RPCClientWrapper

#### 构造函数

```cpp
// 创建客户端并连接到指定服务器
RPCClientWrapper(const std::string& host, 
                 uint16_t port, 
                 int64_t timeout_ms = 5000);
```

**参数：**
- `host`: 服务器地址（IP 或域名）
- `port`: 服务器端口
- `timeout_ms`: 默认超时时间（毫秒）

#### 主要方法

```cpp
// 同步调用远程函数
template<typename R, typename... Args>
R call(const std::string& func_name, Args&&... args);

// 异步调用远程函数
template<typename... Args>
auto async_call(const std::string& func_name, Args&&... args) 
    -> std::future<RPCLIB_MSGPACK::object_handle>;

// 发送通知（单向，无返回值）
template<typename... Args>
void send_notification(const std::string& func_name, Args&&... args);

// 超时管理
void set_timeout(int64_t timeout_ms);     // 设置超时
void clear_timeout();                      // 清除超时限制

// 连接管理
bool is_connected() const;                                    // 检查连接状态
rpc::client::connection_state get_connection_state() const;  // 获取连接状态
void wait_all_responses();                                    // 等待所有异步响应
```

### RPCServerWrapper

#### 构造函数

```cpp
// 在指定端口创建服务器（监听所有地址）
explicit RPCServerWrapper(uint16_t port);

// 在指定地址和端口创建服务器
RPCServerWrapper(const std::string& address, uint16_t port);
```

#### 主要方法

```cpp
// 绑定函数到 RPC 接口
template<typename F>
void bind(const std::string& name, F&& func);

// 运行控制
void run();                              // 同步运行（阻塞）
void async_run(size_t worker_threads);   // 异步运行（指定工作线程数）
void stop();                             // 停止服务器

// 配置
void suppress_exceptions(bool suppress);  // 设置异常抑制模式
uint16_t port() const;                   // 获取监听端口

// 会话管理
void close_all_sessions();               // 关闭所有客户端连接
bool is_running() const;                 // 检查服务器是否运行中
```

### Logger

```cpp
// 设置日志级别
static void set_log_level(LogLevel level);
// 级别: DEBUG < INFO < WARNING < ERROR

// 日志输出方法
static void debug(const std::string& message);
static void info(const std::string& message);
static void warning(const std::string& message);
static void error(const std::string& message);
```

### Timer

```cpp
Timer();                      // 构造时自动开始计时
void reset();                 // 重置计时器
double elapsed_ms() const;    // 获取经过的毫秒数
double elapsed_sec() const;   // 获取经过的秒数
```

## 💡 最佳实践

### 1. 异常处理

始终使用 try-catch 包裹 RPC 调用：

```cpp
try {
    auto result = client.call<double>("divide", 10.0, 2.0);
} catch (const std::runtime_error& e) {
    rpc_utils::Logger::error("RPC failed: " + std::string(e.what()));
    // 错误处理逻辑
}
```

### 2. 超时设置

为长时间操作设置合适的超时：

```cpp
client.set_timeout(30000);  // 30 秒
auto result = client.call<std::string>("long_running_task", params);
client.clear_timeout();     // 恢复默认超时
```

### 3. 日志级别管理

根据环境选择合适的日志级别：

```cpp
#ifdef NDEBUG
    rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::INFO);    // 生产环境
#else
    rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::DEBUG);   // 开发环境
#endif
```

### 4. 异步批量操作

使用异步调用提高性能：

```cpp
std::vector<std::future<RPCLIB_MSGPACK::object_handle>> futures;

// 发起批量请求
for (const auto& item : items) {
    futures.push_back(client.async_call("process", item));
}

// 收集结果
for (auto& future : futures) {
    auto result = future.get().as<ResultType>();
    // 处理结果...
}
```

### 5. 资源管理

利用 RAII 自动清理资源：

```cpp
{
    rpc_utils::RPCServerWrapper server(8080);
    server.bind("test", &test_function);
    server.run();
}  // 析构函数自动清理资源
```

### 6. 性能监控

使用 Timer 进行性能分析：

```cpp
rpc_utils::Timer timer;
auto result = client.call<Data>("fetch_data");
rpc_utils::Logger::info("Fetch took " + 
    std::to_string(timer.elapsed_ms()) + " ms");
```

## ❓ 常见问题

### Q1: 如何自定义类型序列化？

**A:** 使用 msgpack 的 `MSGPACK_DEFINE` 宏：

```cpp
struct User {
    std::string name;
    int age;
    MSGPACK_DEFINE(name, age);
};

// 现在可以直接使用
server.bind("get_user", []() -> User {
    return {"Alice", 30};
});
```

### Q2: 客户端连接失败怎么办？

**A:** 检查以下几点：
1. 服务器是否已启动
2. 网络连接是否正常
3. 防火墙是否阻止端口
4. 地址和端口是否正确

```bash
# 检查服务器端口
netstat -tlnp | grep 8080

# 测试网络连接
telnet localhost 8080
```

### Q3: RPC 调用超时如何处理？

**A:** 增加超时时间或优化服务器性能：

```cpp
client.set_timeout(60000);  // 增加到 60 秒
```

### Q4: 如何处理服务器端的异常？

**A:** 在绑定的函数中抛出异常，客户端会接收到：

```cpp
// 服务器端
server.bind("divide", [](double a, double b) -> double {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }
    return a / b;
});

// 客户端
try {
    auto result = client.call<double>("divide", 10, 0);
} catch (const std::runtime_error& e) {
    std::cout << "Caught: " << e.what() << std::endl;
}
```

### Q5: 多线程环境下如何使用？

**A:** 每个线程创建独立的客户端实例：

```cpp
void worker_thread(int id) {
    rpc_utils::RPCClientWrapper client("localhost", 8080);
    auto result = client.call<int>("process", id);
}

std::vector<std::thread> threads;
for (int i = 0; i < 10; ++i) {
    threads.emplace_back(worker_thread, i);
}
```

### Q6: 如何选择编译器？

**A:** 根据部署环境选择：

| 编译器 | 使用场景 |
|--------|----------|
| **Occlum GCC** | 机密计算环境（SGX、SEV 等） |
| **GCC/G++** | 常规 Linux 服务器 |
| **Clang** | macOS 或需要更好的诊断信息 |

```bash
# 开发环境用 gcc
./build.sh --compiler=gcc

# 生产环境用 occlum
./build.sh --compiler=occlum
```

## 📚 相关资源

- [rpclib 官方文档](https://github.com/rpclib/rpclib)
- [MessagePack 格式说明](https://msgpack.org/)
- [快速入门指南](QUICKSTART.md)
- [示例代码](examples/)

## 📄 许可证

本项目遵循 rpclib 的许可证条款。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

### 贡献指南

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📧 联系方式

如有问题或建议，请通过以下方式联系：

- 提交 [GitHub Issue](../../issues)
- 查看 [讨论区](../../discussions)

---

<div align="center">

**[⬆ 返回顶部](#rpc-utils)**

Made with ❤️ for the community

</div>

