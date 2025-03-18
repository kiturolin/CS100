1. 结构体是一种"类型", 它内部的成员变量是按照顺序紧挨着存储在一起的

2. 使用"."号来访问结构体中的成员变量, 但如果是结构体指针, 你要使用pStruct->member的方式访问!
    或者使用*(pStruct).member, 但*pStruct.mem是不正确的!

3. 两个成员变量相同, 但顺序不同的结构体可能有不同的size! 这是因为操作系统内部通常进行字节对齐, 这会导致struct内部有空白的空间
```c 
struct A {
    int x;
     // 4 bytes
    char y;
     // 1 byte
    // 3 bytes padding
    double z; // 8 bytes
};

struct B {
    char x;
     // 1 byte
    // 7 bytes padding
    double y; // 8 bytes
    int z;
     // 4 bytes
    // 4 bytes padding
};

where sizeof(struct A) == 16 but sizeof(struct B) == 24
```

4. 如果你不显式赋值, 且没有在全局作用域下声明结构体, 或在局部作用域下用static声明结构体, 那么结构体内成员的值是未定的! 可以是任何值!

5. 使用designator来赋值! 这是优良的传统
```c 
struct Student stu = {.name = "Alice", .id = "2024533000", .entrance_year = 2024, .dorm = 8};
```

6. 注意! 下标操作符[]返回的是左值(变量的实体)! 如下操作是正确的! student_list[i]返回值类型为struct Student, 而不是struct Student * !
    这被称作复合字面量(Compound Literals)
```c
struct Student *student_list = malloc(sizeof(struct Student) * n);
for (int i = 0; i != n; ++i) {
    student_list[i] = (struct Student){.name = A(i), .id = B(i), .entrance_year = C(i), .dorm = D(i)};
}
```

7. 若将结构体作为参数传递, 则这种传递是深拷贝传递!(值传递, 而非引用传递), 这意味着进入函数后, 参数会是一个全新的, 但与传进来的结构体一模一样的新结构体
```c 
void print_student(struct Student s) {
    printf("Name: %s, ID: %s, dorm: %d\n", s.name, s.id, s.dorm);
}
```

8. 若一个函数返回值是结构体, 严格来说, 这个函数的返回也是一种深拷贝! 不是引用传递!
```c 
struct Student fun(void) {
    struct Student s = something();
    some_operations(s);
    return s;
}
student_list[i] = fun();
```

9. 如果你在结构体中定义了一个数组, 那么结构体的赋值操作会全部把整个数组拷贝过来! 这是element-wise级别的拷贝, 很奇特! 
```c 
struct A {
    int array[10];
    // ...
};

int a[10];
int b[10] = a; // Error!

struct A a;
struct A b = a; // OK
```

10. 只能使用getchar(), 如何实现输入一个整数? 将得到的char变量减去'0' !

11. 使用递归的方式, 把一个非负整数(可以有很多位)打印在屏幕上, 你能看懂代码吗:
```c 
void print(unsigned x) {
    if (x >= 10)
        print(x / 10);
    putchar(x % 10 + '0');
}
```

12. 利用递归的方式实现选择排序: 先找到当前数组中的最小值, 将其与第一位元素交换, 随后解决去除第一个元素的新数组:
```c 
void sort_impl(int *a, int k, int n) {
    if (k == n - 1)
    return;
    int m = k;
    for (int i = k + 1; i < n; ++i)
    if (a[i] < a[m])
    m = i;
    swap(&a[m], &a[k]); // the "swap" function we defined in previous lectures
    sort_impl(a, k + 1, n); // sort the rest part recursively
}
void sort(int *a, int n) {
    sort_impl(a, 0, n);
}
```
