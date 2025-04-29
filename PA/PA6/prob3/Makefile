
CC = g++
CFLAGS = -std=c++17 -I./detail/ -Wall -Wpedantic -Wextra -Wno-unused-variable -fdiagnostics-color=always  # 自定义编译标志

# 获取所有.c文件并生成对应的.out文件名
SRCS = $(wildcard *.cpp)
EXECS = $(SRCS:.cpp=.out)

# 默认目标：编译所有可执行文件
all: $(EXECS)

# 模式规则：将每个.c文件编译为对应的.out文件
%.out: %.cpp
	$(CC) $(CFLAGS) $< -o $@

# 清理生成的可执行文件
clean:
	rm -f $(EXECS)

# 声明伪目标，避免与同名文件冲突
.PHONY: all clean
