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

# 打印使用说明
print_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  clean                      Clean build directory before building"
    echo "  --compiler=<type>          Specify compiler type: occlum (default), gcc, clang"
    echo "  --cc=<path>                Specify C compiler path"
    echo "  --cxx=<path>               Specify C++ compiler path"
    echo "  -h, --help                 Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                         # Build with default occlum compiler"
    echo "  $0 clean                   # Clean and rebuild"
    echo "  $0 --compiler=gcc          # Build with system gcc/g++"
    echo "  $0 --compiler=clang        # Build with clang/clang++"
    echo "  $0 --cc=gcc --cxx=g++      # Specify compilers explicitly"
    echo ""
}

# 获取脚本所在目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

# 默认编译器设置
COMPILER_TYPE="occlum"
C_COMPILER=""
CXX_COMPILER=""
CLEAN_BUILD=false

print_info "RPC Utils Build Script"
print_info "======================"

# 解析命令行参数
for arg in "$@"; do
    case $arg in
        clean)
            CLEAN_BUILD=true
            ;;
        --compiler=*)
            COMPILER_TYPE="${arg#*=}"
            ;;
        --cc=*)
            C_COMPILER="${arg#*=}"
            ;;
        --cxx=*)
            CXX_COMPILER="${arg#*=}"
            ;;
        -h|--help)
            print_usage
            exit 0
            ;;
        *)
            print_error "Unknown option: $arg"
            print_usage
            exit 1
            ;;
    esac
done

# 根据编译器类型设置编译器路径（如果未明确指定）
if [ -z "$C_COMPILER" ] || [ -z "$CXX_COMPILER" ]; then
    case $COMPILER_TYPE in
        occlum)
            C_COMPILER="occlum-gcc"
            CXX_COMPILER="occlum-g++"
            ;;
        gcc)
            C_COMPILER="gcc"
            CXX_COMPILER="g++"
            ;;
        clang)
            C_COMPILER="clang"
            CXX_COMPILER="clang++"
            ;;
        *)
            print_error "Unknown compiler type: $COMPILER_TYPE"
            print_error "Supported types: occlum, gcc, clang"
            exit 1
            ;;
    esac
fi

print_info "Compiler Configuration:"
print_info "  C Compiler:   $C_COMPILER"
print_info "  C++ Compiler: $CXX_COMPILER"

# 检查编译器是否存在
if ! command -v "$C_COMPILER" &> /dev/null; then
    print_error "C compiler not found: $C_COMPILER"
    print_error "Please ensure the compiler is installed and in PATH"
    exit 1
fi

if ! command -v "$CXX_COMPILER" &> /dev/null; then
    print_error "C++ compiler not found: $CXX_COMPILER"
    print_error "Please ensure the compiler is installed and in PATH"
    exit 1
fi

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
if [ "$CLEAN_BUILD" = true ]; then
    print_info "Cleaning build directory..."
    rm -rf "${BUILD_DIR}"
fi

# 创建构建目录
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# 配置CMake
print_info "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_C_COMPILER="$C_COMPILER" \
         -DCMAKE_CXX_COMPILER="$CXX_COMPILER"

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
