1. Only Once函数: 只被调用一次的函数
    要实现这个函数, trivial的方法是使用一堆全部变量来标记这个函数是否被使用过
    然而这样做会添加很多不必要的全局变量  
    你可以在函数中使用static修饰局部变量, 使用static修饰的局部变量的生命周期和全局变量一样
    static修饰的局部变量和行为和全局变量一样, 只不过它不会污染全局命名空间

2. 隐式初始化
    当你声明变量时, 如果不对变量加以初始化, 非static变量的值是未定的, 而static修饰的变量将被隐式初始化为"广义的0"(指针被初始化为NULL, float被初始化为0.0等)
    你在处理未初始化变量时需要极度小心, int n;后 (n == n)不一定为true! 甚至在运行过程中n的值会改变!(这是未定义行为)

3. const
    const修饰的变量将无法被显式的修改, 若你尝试修改它会报错, 但间接的修改是未定义行为, 可能无法被编译器侦测出

4. 如果你使用了const修饰变量却不直接初始化它, 那么这个变量几乎变得不可用了: 你无法对它初始化或使用它, 这些都是会导致错误的行为

5. int     * ptr 和int * ptr都是合法的指针类型变量声明

6. 如果你需要在一行内声明多个指针类型变量, 你需要在每一个变量前加上*号
```c
    int* p1, p2, p3; // `p1` is of type `int*`, but `p2` and `p3` are ints.
    int *q1, *q2, *q3; // `q1`, `q2` and `q3` all have the type `int*`.
    int* r1, r2, *r3; // `r1` and `r3` are of the type `int*`,
        // while `r2` is an int.
```

7. 取地址符&的操作数只能是左值(lvalue), 不能是右值, 比如&42, &(a + b)是不被允许的

8. 解引用符传回的是变量本身(左值)而不是变量的值(右值)
    比如ptr指向的变量为a, a的值为42, 那么++*ptr等价于++a而不是++(42)

9. 非static修饰的局部指针变量在未初始化时的值是未定的(称为野指针) 
10. 在指针的解应用时先使用&&运算符的短路特性判断ptr的值是不是NULL
```c 
    if (ptr != NULL && *ptr == 42) { /* ... */ }
```
11. 数组的长度必须是常数表达式或在编译式能知晓的值( 比如某个变量的值, 这是C99引入的Varaible-Length Array) 

12. 同样的, 若数组为非static的局部变量, 未初始化同样的会导致未知的初始值
13. 数组可以使用大括号初始化, 这种初始化方式很方便, 初始化的值的数量也不一定要恒等于数组长度
```c 
    int a[10] = {2, 3, 5, 7};   // Correct: Initializes a[0], a[1], a[2], a[3]
    int b[2] = {2, 3, 5};       // Error: Too many initializers
    int c[] = {2, 3, 5};        // Correct: 'c' has type int[3].
    int d[100] = {};            // Correct in C++ and since C23.
```
14. 当数组的其他部分没有被显式初始化时, 它们会被初始化为0
```c 
    int main(void) {
        int a[10] = {1, 2, 3};  // a[3], a[4], ... are all initialized to zero.
        int b[100] = {0};       // All elements of b are initialized to zero.
        int c[100] = {1};       // c[0] is initialized to 1,
        // and the rest are initialized to zero.
    }
```

15. 二维数组(嵌套数组)在内存空间上是连续的! 这非常重要!!!
    比如int a[4][3]在内存空间中占据连续的4 * 3 = 12个int长度的内存空间
    *(a + 5)等价于a[1][1]
16. 二维数组的初始化, 有点复杂但你要忍一下
```c 
    // 注意这个数组只初始化了前三个行, 最后一行也被初始化为0了
    int a[4][3] = { // array of 4 arrays of 3 ints each (4x3 matrix)
        { 1 },      // row 0 initialized to {1, 0, 0}
        { 0, 1 },   // row 1 initialized to {0, 1, 0}
        { [2]=1 },  // row 2 initialized to {0, 0, 1}
                    // row 3 initialized to {0, 0, 0}
    };

    int b[4][3] = {
         // array of 4 arrays of 3 ints each (4x3 matrix)
        1, 3, 5, 2, 4, 6, 3, 5, 7 // row 0 initialized to {1, 3, 5}
    }
    // row 1 initialized to {2, 4, 6}
    // row 2 initialized to {3, 5, 7}
    // row 3 initialized to {0, 0, 0}
;
        int y[4][3] = {[0][0]=1, [1][1]=1, [2][0]=1}; // row 0 initialized to {1, 0, 0}
    // row 1 initialized to {0, 1, 0}
    // row 2 initialized to {1, 0, 0}
    // row 3 initialized to {0, 0, 0}
```


