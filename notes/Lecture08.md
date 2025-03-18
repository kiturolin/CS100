1. 利用malloc()创建"伪二维"数组: 下标操作出神入化的使用!
```c 
int **p = malloc(sizeof(int *) * n);
for (int i = 0; i < n; ++i)
    p[i] = malloc(sizeof(int) * m);

for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
        scanf("%d", &p[i][j]);
```

2. free(NULL)会发生什么事? 什么也不会发生, free()内部自带异常捕获

3. 连续两次free同一个指针会怎么样? 未定义行为! 通常会导致严重的运行时错误!

4. printf(NULL)是未定义行为! 

5. 所有接受字符串为参数的函数本质上是接受了一个字符指针char *作为参数, 前提是这个字符串必须以'\0'结尾!

6. 区分NULL, ""和'\0' : '\0'是一个char类型的字符, 值为0, ""是一个char [1]类型的字符串(字符数组), 只有一个元素'\0', NULL是一个空指针, 三者完全不同!

7. 命令行参数: argc为参数个数, char **argv为数组, 内部的元素为指向各个参数的字符串指针
    argv的最后一个元素总是为NULL, 若argv[0] == NULL, 说明没有任何输入参数

8. 如何读取一个不定长的输入? 使用malloc()动态分配内存! 当输入的长度超出初始长度时, 分配两倍大小的新空间, 并free掉旧的buffer, 另外一个需要特别注意的是, 当接受到了一个空白字符(意味着输入已经结束), 你需要在停止向缓冲区写入字符的同时, 将这个空白字符"返还"给stdin! 否则你就"吃"掉了这个字符!
```c
char *read_string(void) {
    // ...
    while (!isspace(c)) {
        if (cur_pos == capacity - 1) { // `-1` is for '\0'.
            char *new_buffer = malloc(capacity * 2);
            memcpy(new_buffer, buffer, cur_pos); // copy everything we have stored
            // to the new buffer
            free(buffer); // !!!!!!!!!!!
            capacity *= 2;
            buffer = new_buffer;
        }
        buffer[cur_pos++] = c;
        c = getchar();
    }
    // Now, `c` is a whitespace. This is not part of the contents we need.
    ungetc(c, stdin); // Put that whitespace back to the input.
    buffer[cur_pos] = '\0'; // Remember this!!!

    return buffer;
}
```
    用法: 千万不要忘记free!
```c 
int main(void) {
    char *content = read_string();
    puts(content);
    free(content);
}
```
