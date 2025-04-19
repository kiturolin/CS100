1. 引用(Reference)
    1. 语法为ReferredType &, 相当于给对象"起别名"
    ```cpp
    int ival = 42;
    int &ri = ival; // `ri` refers to `ival`.
    // In other words, `ri` is an alternative name for `ival`.
    std::cout << ri << '\n'; // prints the value of `ival`, which is `42`.
    ++ri; // Same effect as `++ival;`.
    ```

    2. A reference is an alias
        假设ri为ival的refernece, 那么ri = a;这条语句是什么意思? 
        --> 与ival = a;等价! 是对ival的重新赋值! 而不是将ri绑定为a的引用(reference)

        reference一旦被绑定就再也无法修改或解除绑定, 因此, 在声明reference时必须加上定义(对已存在的对象的绑定)
        或者说, reference必须与"左值(lvalue)"绑定
        ```cpp 
        int &r1 = 42; // Error: binding a reference to a literal
        int &r2 = 2 + 3; // Error: binding a reference to a temporary object
        int a = 10, b = 15;
        int &r3 = a + b; // Error: binding a reference to a temporary object
        ```
    3. reference不是一个对象! 不存在"引用的引用"
    ```cpp
    int ival = 42;
    int &ri = ival; // binding `ri` to `ival`.
    int & &rr = ri; // Error! No such thing!

    int &ri2 = ri;
    // ri2 is a reference that is bound to ival .
    // Any use of a reference is actually using the object that it is bound to!
    ```
    记住这句话: 任何reference的使用都是在使用其绑定的对象!(你自己在脑子里做个文本替换即可)

    3.5. 注意! 数组的成员不能是引用! 意为: 你不能创建一个类型为引用的数组, 即数组不能存储引用!
    ```cpp
    // "引用数组" 不是"对数组的引用"!
    int &ar[N]; // No such thing!

    // 这才是"对数组的引用"
    int a[10];
    int (&ar)[10] = a;
    ar[0] = 42; // a[0] = 42
    ```

    4. 指针必须指向对象的地址, 不存在"指向reference的指针"
    ```cpp
    int ival = 42;
    int &ri = ival; // binding `ri` to `ival`.
    int &*pr = ri; // Error! No such thing!
    int *pi = &ri; // Same as `int *pi = &ival;`.
    // 注意此处的&是取地址操作符, 而不是引用!
    ```
    4.5 将数组引用作为函数参数: 如果你将一个对数组的引用作为函数的参数, 那么该参数不会退化为首地址指针! 它将成为一个真正的数组! 这是C中没有的特性, 而且非常好用!
        但你必须在参数中显示的声明数组的长度! 并且参数只能是arr[10], 长度必须严格对应, 否则报错
    ```cpp
    void print_array(const int (&array)[10]) { // 只读，所以加 const
        for (int x : array) // 基于范围的 for 语句可以⽤来遍历数组！
            std::cout << x << ' ';
        std::cout << std::endl;
    }
    int a[10], ival = 42, b[20];
    print_array(a); // Correct
    print_array(&ival); // Error!
    print_array(b); // Error!
    ```
    5. 定义reference的style和指针十分类似:
    ```cpp
    int ival = 42, &ri = ival, *pi = &ival;
    // `ri` is a reference of type `int &`, which is bound to `ival`.
    // `pi` is a pointer of type `int *`, which points to `ival`.

    int& x = ival, y = ival, z = ival;
    // Only `x` is a reference. `y` and `z` are of type `int`.
    ```

    6. 在range-for中使用的临时变量是原对象的一个拷贝copy!
        如果你在for循环中更改循环变量c的值, 不会影响原对象!
        如果同时你也想修改原对象, 那么在for定义循环变量时, 使用引用吧!
    ```cpp
    for (char c : str)
        c = std::toupper(c); // This has no effect.

    for (char &c : str)
        c = std::toupper(c); // Has effect!
    ```

    7. 虽然, 普通的reference类型不能与右值(临时值)相绑定, 但const修饰的引用类型可以绑定右值!
    ```cpp
    const int &rci = 42; // OK.
    const std::string &rcs = a + b; // OK.

    std::string tmp = a + b;
    const std::string &rcs = tmp;
    ```

    8. 因此, 你可以用const reference来作为函数的参数, 这么做有以下好处
        1. 在函数传参时, 并没有发生对象的拷贝, 这非常快, 我们传递的是对象的引用
        2. 引用reference是比指针pointers更高一层的抽象, 你不需要关心对象如何存储在内存中, 降低心智负担
        2. const修饰使得函数中试图修改原对象的行为变成了错误
        3. const修饰的引用类型允许我们传入右值(临时值)
    ```cpp
    int count_lowercase(const std::string &str) { // `str` is a reference-to-`const`.
        int cnt = 0;
        for (char c : str)
        if (std::islower(c))
        ++cnt;
        return cnt;
    }

    std::string a = something(), b = some_other_thing();
    int res1 = count_lowercase(a); // OK.
    int res2 = count_lowercase(a + b); // OK.
    int res3 = count_lowercase("hello"); // OK.
    ```

2. std::vector
    1. std::vector是一个模板(Template), 用于自动化的构建一个类, 经常被当作动态数组来使用
    通过模板创建类的操作称作"实例化"(instantiation), 我们在创建vector类时, 使用尖括号<T>来指定vector存储的变量类型
    ```cpp 
    std::vector v; // Error: missing template argument.
    std::vector<int> vi; // An empty vector of `int`s.
    std::vector<std::string> vs; // An empty vector of strings.
    std::vector<double> vd; // An empty vector of `double`s.
    std::vector<std::vector<int>> vvi; // An empty vector of vector of `int`s.
    // "2-d" vector.
    ```
    几种创建并定义vector的方法(注意最后两种!):
    ```cpp 
    std::vector<int> v{2, 3, 5, 7}; // A vector of `int`s,
    // whose elements are {2, 3, 5, 7}.
    std::vector<int> v2 = {2, 3, 5, 7}; // Equivalent to ↑
    std::vector<std::string> vs{"hello", "world"}; // A vector of strings,
    // whose elements are {"hello", "world"}.
    std::vector<std::string> vs2 = {"hello", "world"}; // Equivalent to ↑
    std::vector<int> v3(10); // A vector of ten `int`s, all initialized to 0.
    std::vector<int> v4(10, 42); // A vector of ten `int`s, all initialized to 42.
    ```

    几种拷贝vector的方法(自动执行的element-wise拷贝! 无需写一个loop!)
    ```cpp
    std::vector<int> v{2, 3, 5, 7};
    std::vector<int> v2 = v; // `v2`` is a copy of `v`
    std::vector<int> v3(v); // Equivalent
    std::vector<int> v4{v}; // Equivalent

    std::vector<int> v1 = something(), v2 = something_else();
    v1 = v2;
    ```

    2. C++17引入的CTAD: Class Template Argument Deduction
        只要你在初始化中提供了足够的说明, 编译器将自动推断vector的类型
    ```cpp
    std::vector v1{2, 3, 5, 7}; // vector<int>
    std::vector v2{3.14, 6.28}; // vector<double>
    std::vector v3(10, 42); // vector<int>, deduced from 42 (int)
    std::vector v4(10); // Error: cannot deduce template argument type
    ```

    3. size()和empty()方法: 与字符串一致
        v.clear()将清除vector内的所有元素

    4. 在vector末尾添加元素:v.push_back(x), 在vector末尾移除元素:v.pop_back()
        v.back()和v.front()会返回vector的末尾和头部元素的"引用reference"
    ```cpp
    // we can modify the corresponding element by:
    v.front() = 42;
    ++v.back();
    ```

    5. C++中的第一个UB!
        若vector为空, 那么back, front以及pop_back的行为都是未定义的!

    6. 使用range-based for loop来遍历vector
    ```c 
    std::vector<int> vi = some_values();
    for (int x : vi)
    std::cout << x << std::endl;
    std::vector<std::string> vs = some_strings();
    for (const std::string &s : vs) // use reference-to-const to avoid copy
    std::cout << s << std::endl;
    ```

    7. 你可以使用下标v[i]来访问vector内的元素
        为了性能考虑, 标准库中使用下标访问的操作都没有边界检查:
        第二个UB! index out of range是未定义行为

3. new和delete
    类似于malloc和free, 但new比malloc多做了一件事: 构建并返回一个对象
    new做了两件事: 创建新对象并为其分配内存
    而malloc只是返回一个指向某块内存区域的指针, 这是他们的最大区别
    使用new + 类型名(或类名)来创建对象, 如果在类型名后加上大括号{}表示进行值初始化, 否则为默认初始化
    默认初始化 (default-initialization)：就是未初始化，具有未定义的值
    值初始化 (value-initialization)：类似于 C 中的“空初始化”，是各种零
    ```cpp
    int *pi1 = new int; // 动态创建⼀个默认初始化的 int
    int *pi2 = new int(); // 动态创建⼀个值初始化的 int
    int *pi3 = new int{}; // 同上，但是更 modern
    int *pi4 = new int(42); // 动态创建⼀个 int，并初始化为 42
    int *pi5 = new int{42}; // 同上，但是更 modern

    int *pai1 = new int[n]; // 动态创建了 n 个 int，默认初始化
    int *pai2 = new int[n](); // 动态创建了 n 个 int，值初始化
    int *pai3 = new int[n]{}; // 动态创建了 n 个 int，值初始化
    int *pai4 = new int[n]{2, 3, 5}; // 动态创建了 n 个 int，前三个元素初始化为 2,3,5
    // 其余元素都被值初始化（为零）
    // 如果 n<3，抛出 std::bad_array_new_length 异常
    ```
    使用 new[]语法来创建一个数组对象, 使用delete[]来删除一个数组对象
    new 必须对应 delete ， new[] 必须对应 delete[] ，否则是 undefined behavior
    违反下列规则的一律是 undefined behavior:

    - `delete ptr` 中的 `ptr` 必须等于某个先前由 `new` 返回的地址
    - `delete[] ptr` 中的 `ptr` 必须等于某个先前由 `new[]` 返回的地址
    - `free(ptr)` 中的 `ptr` 必须等于某个先前由 `malloc`, `calloc`, `realloc` 或 `aligned_alloc` 返回的地址。

    



