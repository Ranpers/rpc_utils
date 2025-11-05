#!/bin/bash

# RPC Utils 构建脚本
# 用于简化编译过程

set -e  # 遇到错误时退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

print_info "RPC Utils Build Script"
print_info "======================"

# 检查rpclib库是否存在
RPCLIB_LIB="${SCRIPT_DIR}/lib/librpc.a"

if [ ! -f "${RPCLIB_LIB}" ]; then
    print_error "rpclib not found at ${RPCLIB_LIB}"
    print_error "Please ensure librpc.a is in the lib/ directory"
    exit 1
else
    print_info "Found rpclib at ${RPCLIB_LIB}"
fi

# 返回到项目目录
cd "${SCRIPT_DIR}"

# 清理旧的构建（如果指定了clean）
if [ "$1" = "clean" ]; then
    print_info "Cleaning build directory..."
    rm -rf "${BUILD_DIR}"
fi

# 创建构建目录
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# 配置CMake
print_info "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_C_COMPILER=occlum-gcc \
         -DCMAKE_CXX_COMPILER=occlum-g++

# 编译
print_info "Building project..."
make -j$(nproc)

print_info ""
print_info "Build completed successfully!"
print_info ""
print_info "Generated files:"
print_info "  - Libraries:"
print_info "    * librpc_utils_common.a"
print_info "    * librpc_utils_client.a"
print_info "    * librpc_utils_server.a"
print_info "  - Examples:"
print_info "    * example_server"
print_info "    * example_client"
print_info ""
print_info "To run the examples:"
print_info "  1. Start server: ./example_server"
print_info "  2. In another terminal, run client: ./example_client"
print_info ""
