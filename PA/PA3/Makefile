
CC = gcc
CFLAGS = -std=c17 -Wall -Wpedantic -Wextra -Werror -fdiagnostics-color=always -fsanitize=undefined -fsanitize=address -lm -ggdb -O0  # 自定义编译标志

# 获取所有.c文件并生成对应的.out文件名
SRCS = $(wildcard *.c)
EXECS = $(SRCS:.c=.out)

# 默认目标：编译所有可执行文件
all: $(EXECS)

# 模式规则：将每个.c文件编译为对应的.out文件
%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

# 清理生成的可执行文件
clean:
	rm -f $(EXECS)

# 声明伪目标，避免与同名文件冲突
.PHONY: all clean
