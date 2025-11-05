# 快速入门指南

## 5分钟快速上手

### 步骤1: 构建项目 (1分钟)

```bash
cd /workspaces/te/confidential_computing/rpc_utils
./build.sh
```

等待编译完成，应该看到：
```
[INFO] Build completed successfully!
```

### 步骤2: 运行示例 (2分钟)

**终端1 - 启动服务器**:
```bash
cd build
./example_server
```

应该看到：
```
[INFO] Server started successfully on port 8080
[INFO] Available functions:
  - add(double, double) -> double
  - subtract(double, double) -> double
  ...
```

**终端2 - 运行客户端**:
```bash
cd build
./example_client
```

应该看到测试结果：
```
[INFO] Connected successfully
----------------------------------------
[INFO] Testing arithmetic operations:
[INFO] add(10.5, 20.3) = 30.800000 (took 2.5 ms)
...
[INFO] All tests completed successfully!
```

### 步骤3: 编写第一个程序 (2分钟)

**my_server.cpp**:
```cpp
#include "rpc_server_wrapper.h"

int main() {
    rpc_utils::RPCServerWrapper server(8080);
    
    // 绑定一个简单的函数
    server.bind("hello", []() -> std::string {
        return "Hello, RPC!";
    });
    
    server.run();
}
```

**my_client.cpp**:
```cpp
#include "rpc_client_wrapper.h"
#include <iostream>

int main() {
    rpc_utils::RPCClientWrapper client("localhost", 8080);
    
    std::string msg = client.call<std::string>("hello");
    std::cout << msg << std::endl;  // 输出: Hello, RPC!
}
```

**编译**:
```bash
# 服务器
occlum-g++ -std=c++14 my_server.cpp \
    -I../include \
    -L. -lrpc_utils_server -lrpc_utils_common \
    -L../lib -lrpc \
    -lpthread -o my_server

# 客户端
occlum-g++ -std=c++14 my_client.cpp \
    -I../include \
    -L. -lrpc_utils_client -lrpc_utils_common \
    -L../lib -lrpc \
    -lpthread -o my_client
```

## 常见用例

### 用例1: 远程计算服务

```cpp
// 服务器
server.bind("compute", [](const std::vector<double>& data) {
    double sum = 0;
    for (auto x : data) sum += x;
    return sum / data.size();
});

// 客户端
std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
double avg = client.call<double>("compute", data);
```

### 用例2: 异步批量请求

```cpp
// 发起多个异步请求
std::vector<std::future<RPCLIB_MSGPACK::object_handle>> futures;
for (int i = 0; i < 10; i++) {
    futures.push_back(client.async_call("process", i));
}

// 收集结果
for (auto& f : futures) {
    int result = f.get().as<int>();
    // 处理结果...
}
```

### 用例3: 日志和监控

```cpp
// 设置日志级别
rpc_utils::Logger::set_log_level(rpc_utils::LogLevel::DEBUG);

// 使用计时器
rpc_utils::Timer timer;
double result = client.call<double>("heavy_computation", data);
rpc_utils::Logger::info("Computation took " + 
    std::to_string(timer.elapsed_ms()) + " ms");
```

## 故障排查

### 问题1: 编译错误 "rpc/client.h not found"
**解决**: 检查include/rpc目录是否存在
```bash
ls include/rpc/client.h
```
如果不存在，需要确保rpc头文件已复制到include/目录

### 问题2: 链接错误 "cannot find -lrpc"
**解决**: 检查librpc.a是否存在
```bash
ls lib/librpc.a
```

### 问题2: 运行时错误 "Connection refused"
**解决**: 确保服务器已启动并监听正确端口
```bash
netstat -tlnp | grep 8080
```

### 问题3: 客户端超时
**解决**: 增加超时时间
```cpp
client.set_timeout(10000);  // 10秒
```

## 下一步

- 📖 阅读完整的 [README.md](README.md)
- 💻 研究 [examples/](examples/) 目录中的示例代码
- 🚀 开始在你的项目中使用RPC工具类

## 获取帮助

- 查看示例代码中的注释
- 阅读头文件中的API文档
- 参考rpclib官方文档: https://github.com/rpclib/rpclib
- 检查include/rpc目录中的头文件

---

**祝你使用愉快！** 🎉
