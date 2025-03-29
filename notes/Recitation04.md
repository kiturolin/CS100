1. 任何不同指针类型之间的隐式转换都是极不安全的：
```c 
int i = 42;
float *fp = &i;
++*fp; //未定义行为这与++i 不等价。
```

2. Pointer Aliasing指针别名: 当两个指针指向同一块内存, 就是别名
    如果a, b指向同一块内存区域, 那么编译器可能会并行优化, 导致错误
```c 
void foo(double* a, double* b, int n) {
    for (int i = 0; i < n; i++) {
        b[i] += a[i];
    }
}
```

3. Strict Aliasing严格别名, 这是一种假设, 假定不同类型的指针不会指向同一片内存(而不是同种类型指针指向同一块内存, 应该是"不同类型"指针!)
    违反严格别名规则——通过指向 不兼容类型 的指针访问内存——会因编译器基于该
假设进行优化而导致 未定义行为。

4. 当对一个实际指向类型为 T2 的对象的 T1* 指针进行解引用时，会导致 未定义行为，
    除非T1与T2足够相似, 或完全相同
    足够相似的定义:
    1. Qualification Conversion 当 T2 为一个经过 const 、 volatile 或 restrict 限定的与 T1 兼容的类型时。 (比如int *转为const int *)
    2. Signed/Unsigned Conversion 如果T2是T1的有符号或无符号版本
        (如int转为unsigned int)
    3. 若T2为char, unsigned char或signed char, 则任何类型的T *都可被转换为字符类型
        这种转换在检查原始内存的函数中被广泛应用，因为字符指针提供了一种标准化的
访问任意数据的机制。

5. 指向任意类型的指针都可被隐式转换为void *, C中 void *也可被转换为任何类型指针

6. 避免对 malloc 的返回值进行类型转换! 应该使用
```c 
int* p = malloc(sizeof(int));
```

```c 
int* p = (int *)malloc(sizeof(int)); // 不必要的类型转换
```

7. NULL是一个"宏"(marco definition), 它的值是Implementation Defined的, "一般为" 0
    问题: func(NULL)匹配哪个函数?
```c 
void func(int a){...}

void func(char *ptr){...}
```
    记住!!! NULL是一个值! 是一个常量! 而非指针本身!

8. 在现代C++和现代C中, 应该用nullptr来保证类型安全和代码清晰
    nullptr的类型为nullptr_t, 它既不是整数类型也不是指针类型, 但它可以被隐式转换为布尔值或任意指针变量, 但不可能被隐式转换为整数, 从而防止歧义和意外转换

9. E1[E2] 等价于 *((E1) + (E2)) 。
```
a[3] == 3[a]
```

10. Low-Level Constness: const int *a 或 int const *a
    Top-Level Constness: int * const p;
    低层级const可以理解为"指向一个const类型的指针", 高层次const可以理解为"一个类型为const的指针(指针本身的值(地址)不可被修改)"
```c 
int n = 42;     // n的值为 42
int* p = &n;    // p指向 n
const int* cp = p;  // cp为指向 const int 的指针（安全）
p = cp;         //警告：丢弃 const 限定符
p = (int*)cp;   //显式转换（风险较高）
*p = 50;        // 未定义行为：修改 const 值
```

```c 
int n = 42;
int m = 100;
int* const p = &n // p是一个指向 int 的常量指针
p = &n; 
*p = 50;    // 可以：所指对象可被修改
p = &m; // 错误：p 为常量指针，不能重新赋值
```

11. 尽量不要使用可变长度数组VLA int a[n], a的长度在运行时确定

12. 从array decay到pointer (衰变)
    Type[N]的数组会衰变为Type *类型的指针

    以下情况不会发生decay:
    1. 数组arr作为sizeof的操作数
    2. 数组arr作为取地址符的操作数&arr 
    3. 字符串字面量的初始化 s = "Hello!"

13. fgets和puts具有内存安全性
    puts会在末尾增加一个\n !!!!!!
    str中的终止字符\0不会被写入到输出
    
    fgets:
```c 
char* fgets(char* str, int count, FILE* stream);
```
    最多读一行(\n分隔)的内容!!!
    它最多从指定流中读取 count - 1 个字符。
    当遇到换行符（ \n ）或文件结束符（ EOF ）时停止读取。
    成功时返回 str 的指针；失败时返回空指针。

14. 后缀自增运算符 ++ 的优先级高于解引用运算符 * 
    因此(*str++ == '\0')这种写法是合法的

15. free(NULL)并不是未定义行为! 这是合法的操作!

16. malloc(0) calloc(0, N)等操作的行为是Implementation Defined的
    它们可能不分配内存或者返回空指针
    但是解引用这些指针一定会引发未定义行为
    得到的指针也必须被freeee

17. 
