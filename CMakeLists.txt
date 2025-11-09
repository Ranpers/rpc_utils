cmake_minimum_required(VERSION 3.10)
project(rpc_utils VERSION 1.0.0 LANGUAGES CXX)

# 注意：编译器应该通过 build.sh 脚本或 cmake 命令行参数指定
# 例如: cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
# 或使用: ./build.sh --compiler=gcc

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置rpclib路径（从external目录）
set(RPCLIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/rpclib")
set(RPCLIB_INCLUDE_DIR "${RPCLIB_ROOT}/include")
set(RPCLIB_BUILD_DIR "${RPCLIB_ROOT}/build")
set(RPCLIB_LIBS "${RPCLIB_BUILD_DIR}/librpc.a")

# 检查rpclib是否存在
if(NOT EXISTS "${RPCLIB_INCLUDE_DIR}/rpc/client.h")
    message(FATAL_ERROR "rpclib not found at ${RPCLIB_INCLUDE_DIR}. Please run build.sh to setup rpclib.")
endif()

if(NOT EXISTS "${RPCLIB_LIBS}")
    message(FATAL_ERROR "rpclib library not found at ${RPCLIB_LIBS}. Please run build.sh to build rpclib.")
endif()

# 查找Threads库
find_package(Threads REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RPCLIB_INCLUDE_DIR}
)

# 源文件
set(COMMON_SOURCES
    src/common/rpc_utils.cpp
)

set(CLIENT_SOURCES
    src/client/rpc_client_wrapper.cpp
)

set(SERVER_SOURCES
    src/server/rpc_server_wrapper.cpp
)

# 创建静态库
add_library(rpc_utils_common STATIC ${COMMON_SOURCES})
add_library(rpc_utils_client STATIC ${CLIENT_SOURCES})
add_library(rpc_utils_server STATIC ${SERVER_SOURCES})

# 链接rpclib
target_link_libraries(rpc_utils_client ${RPCLIB_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rpc_utils_server ${RPCLIB_LIBS} ${CMAKE_THREAD_LIBS_INIT})

# 示例程序
option(BUILD_EXAMPLES "Build example programs" ON)

if(BUILD_EXAMPLES)
    # 服务器示例
    add_executable(example_server examples/example_server.cpp)
    target_link_libraries(example_server
        rpc_utils_server
        rpc_utils_common
        ${RPCLIB_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
    )

    # 客户端示例
    add_executable(example_client examples/example_client.cpp)
    target_link_libraries(example_client
        rpc_utils_client
        rpc_utils_common
        ${RPCLIB_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
    )

    # 设置编译标志
    set_target_properties(example_server example_client
        PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )
endif()

# 安装规则
install(TARGETS rpc_utils_common rpc_utils_client rpc_utils_server
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# 打印配置信息
message(STATUS "")
message(STATUS "RPC Utils Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "  CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "  RPCLIB Include Dir: ${RPCLIB_INCLUDE_DIR}")
message(STATUS "  RPCLIB Library: ${RPCLIB_LIBS}")
message(STATUS "")
