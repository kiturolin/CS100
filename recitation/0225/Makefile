# 定义编译器变量
CC=gcc

# 定义源文件变量
SRC=$(wildcard *.c)

flags=-Wall -Wextra -std=c17 -Wpedantic -Werror -Wmissing-prototypes -Wstrict-prototypes -O0 -ggdb

.PHONY: clean

all: 
	$(CC) $(flags) $(SRC) -o a.out

clean:
	rm -f a.out
