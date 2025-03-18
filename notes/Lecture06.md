1. 若int a[3] = {1, 2 ,3}, 那么*a == 1 (数组变量a会被隐式转换为指向第一个变量的指针)
2. 使用指针来遍历数组
```c 
int a[10];
bool find(int value) {
for (int *p = a; p < a + 10; ++p)
    if (*p == value)
        return true;
    return false;
}
```
3. 指针之间的和差类型是ptrdiff_t(比如p1 - p2)
4. 指针运算只能在一个数组内发生! 如果指针超出数组所在的内存范围, 那么将是未定义行为
    (SIGSEGV )
    * p1 - p2 , where p1 and p2 point to the positions of two different arrays.
    * p + 2 * N , where p points to some element in an array of length N .
    * p - 1 , where p points to the first element a[0] of some array a .
    (p - 1这个形式也是未定义行为! 即使看上去这么"天真无邪")

5. 当数组被作为参数传递到函数中时会退化为指针, 以下声明都是合法且等价的:
```c 
void fun(int *a);
void fun(int a[]);
void fun(int a[10]);
void fun(int a[2]);
```

6. 即使将T类型的数组定义为参数, 你也可以在该参数的位置处传入任意类型的T *类型变量
```c 
void print(int a[10]) {
    for (int i = 0; i < 10; ++i)
        printf("%d\n", *(a + i));
}
int main(void) {
    int x[20] = {0}, y[10] = {0}, z[5] = {0}, w = 42;
    print(x); // OK
    print(y); // OK
    print(z); // Allowed by the compiler, but undefined behavior!
    print(&w); // Still allowed by the compiler, also undefined behavior!
}
```

7. for循环的执行顺序for(A; B; C), 先A, 随后C再B知道循环结束:
    A(body)CB(body)CB(body)CB(body)CB(body)CB(body)CB(body)CB(body)CB(body)CB...

8. 你可以对指针进行下标运算! []运算符返回p后第i个元素的值
```c
void print(int *a, int n) {
    for (int i = 0; i < n; ++i)
        printf("%d\n", a[i]); // Look at this!
}
```

9. 如何在函数中返回一个数组? 你不可能返回一整个数组! 除非你返回数组首地址

10. 在C中, 指针会有隐式类型转换(见下例子), 但这种转换是极度危险的并且在C++中是一个错误! 绝大多数情况下这是一种未定义行为
```c 
int i = 42;
float *fp = &i;
++*fp; // Undefined behavior. It is not ++i.
```

11. 如何在函数的参数中传递嵌套数组(二维数组?)
    根据之前的理论, 传入函数的, 存储T类型的数组会退化为T *类型变量
    而二维数组(比如 int a[4][3])存储的的类型为T [N] (第一维数组的内容是数组)
    因此T arr[M][N] 会退化为指向T [N]类型的指针!(指向数组的指针)

    因此, 不同的二维数组的类型是不同的! 比如T[10]和T[20]对应的指针类型为
    T(*)[10]和T(*)[20], 这是两个不同类型的指针!
    以下声明参数列表的方式都是等价且有效的
```c 
void fun(int (*a)[N]);
void fun(int a[][N]);
void fun(int a[2][N]);
void fun(int a[10][N]); 
```
    在以上的例子中, 传入函数的数组指针只能是int (*)[N]类型的, 假设N为10, 则不能传入int b[15][20]
    但可以传入a[15][10]

12. 数组指针(指向数组的指针): int (*parr)[N]; parr的类型为 ( int (*)[N] 更符合直觉但错误的写法是 (int [])* )
    指针数组(存储指针的数组): int *arrp[N], arr的类型为(int *[])

13. int *(*arr)[M][N], 是一个指针, 指向一个大数组有M个元素, 大数组中存储M个小数组, 每个小数组有N个int *类型的值, arr是一个指向二维数组([M][N]决定)的指针(左侧的*决定arr是指针) 二维数组存储的数据类型为int *(由右侧的*arr决定)

14. 一些容易混淆的情况
```c 
void func(int *a[]);
void func(int *a[N]);
void func(int **a);
```
    这些情况都是等价的, 都不能传入二维数组, 参数都是一个二重指针(指向指针的指针)




