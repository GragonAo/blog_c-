#!/bin/bash

# 颜色定义
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# 检查并安装 time 工具
if [ ! -f /usr/bin/time ]; then
    echo -e "${YELLOW}检测到缺少 time 工具，尝试安装...${NC}"
    sudo apt install -y time
fi

# 开始计时
START_TIME=$(date +%s)

echo -e "${GREEN}>>> 正在清理并配置 (C++23)...${NC}"
mkdir -p build
cd build || exit
rm -rf ./*

# 开启 ccache 提速
# 这样即使 rm -rf，只要代码没变，编译就是秒级
cmake .. -DCMAKE_CXX_STANDARD=23 \
         -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

echo -e "${GREEN}>>> 正在并行构建 (j20)...${NC}"

# 使用真正的 /usr/bin/time 渲染详细时间
/usr/bin/time -f "\n--------------------------\n🚀 编译细节报告:\n实际耗时: %E\nCPU利用率: %P\n最大内存: %M KB\n--------------------------" make -j20

# 结束计时
END_TIME=$(date +%s)
echo -e "${GREEN}✅ 构建总耗时: $((END_TIME - START_TIME)) 秒${NC}"