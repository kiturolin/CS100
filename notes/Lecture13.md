1. 类型系统
    1. 有一些隐式类型转换是会丢失精度的(比如long long到int的类型转换)
        因此, 会丢失精度的("narrowing conversion")在C++中被完全禁止了!
    2. 更强的类型检查系统
        任何不同类型之间的指针转换在C++中都是不可行的! 包括转换到void *类型!(is a hole in the type system.)
        ```cpp 
        const int x = 42, *pci = &x;
        int *pi = pci; // Warning in C, Error in C++
        ++*pi; // undefined behavior
        char *pc = pi; // Warning in C, Error in C++
        void *pv = pi; char *pc2 = pv; // Even no warning in C! Error in C++.
        int y = pc; // Warning in C, Error in C++
        ```
    3. C++中的显式类型转换: "An ugly behavior should have an ugly looking."
        1. const_cast<T>(expr)
            将expr的low level constness去除: cast away
            ```cpp
            int ival = 42;
            const int &cref = ival;
            int &ref = cref; // Error: casting away low-level constness
            int &ref2 = const_cast<int &>(cref); // OK
            int *ptr = const_cast<int *>(&cref); // OK
            ```
        
        2. reinterpret_cast<T>(expr)
            完全忽略expr之前的类型, 强制认为它是新类型T(非常危险! 不要使用!)
            ```cpp 
            int ival = 42;
            char *pc = reinterpret_cast<char *>(&ival);
            std::string str(pc); // undefined behavior
            ```

        3. static_cast<T>(expr)
            看上去更常用, 更"无害"的类型转换(比如double到int)
            但是, int到double不需要使用这种类型转换! 因为它不是 narrowing conversion!
            可以进行隐式转换!
            ```cpp
            double average = static_cast<double>(sum) / n;
            int pos = static_cast<int>(std::sqrt(n));
            ```

    4. 类型推断Type Deduction
        使用auto来自动确认变量的类型, 但你必须显式初始化这个变量!
        auto还可以自动推导复合类型:
        ```cpp
        auto x = 42; // `int`, because 42 is an `int`.
        auto y = 3.14; // `double`, because 3.14 is a `double`.
        auto z = x + y; // `double`, because the type of `x + y` is `double`.
        auto m; // Error: cannot deduce the type. An initializer is needed.

        auto &r = x; // `int &`, because `x` is an `int`.
        const auto &rc = r; // `const int &`.
        auto *p = &rc; // `const int *`, because `&rc` is `const int *`.
        ```

        注意! 对于字符串字面量的auto deduction, 其类型为const char *, 而非std::string!
        ```cpp
        auto str = "hello"; // `const char *`
        ```
        
        同时, 还可以使用auto来自动推断函数的返回值类型
        ```cpp
        auto sum(int x, int y) {
            return x + y;
        }
        ```

        auto可以让我们尽情享受静态类型系统带来的好处: 所有变量的类型在编译期就可以确定, 因此
        你无需在写代码时过分操心某个变量的类型
        ```cpp
        std::vector<std::string>::const_iterator it = vs.begin();
        // equals
        auto it = vs.begin();
        ```

        同时, 在C++中有一些类型只有编译器才能知道, 你作为人类是不可能知晓这些类型的
        比如lambda函数:
        ```cpp
        auto lam = [](int x, int y) { return x + y; } // A lambda expression.
        ```

        delctype(expr)可以在"不求值(evaluating)"的情况下自动推断expr的类型
        ```cpp
        auto fun(int a, int b) { // The return type is deduced to be `int`.
            std::cout << "fun() is called.\n"
        } return a + b;
        int x = 10, y = 15;
        decltype(fun(x, y)) z; // Same as `int z;`.
        // Unlike `auto`, no initializer is required here.
        // The type is deduced from the return type of `fun`.
        ```

2. C++中的函数
    1. 默认参数
        0. 示例
        ```cpp
        std::string get_screen(std::size_t height = 24, std::size_t width = 80, char background = ' ');

        auto default_screen = get_screen(); // 24x80, filled with ' '
        auto large_screen = get_screen(66); // 66x80, filled with ' '
        auto larger_screen = get_screen(66, 256); // 66x256, filled with ' '
        auto special_screen = get_screen(66, 256, '#'); // 66x256, filled with'#'
        ```
        1. C++中支持函数的默认参数, 但默认参数只能在函数参数列表的最后(靠右手边)
        ```cpp
        std::string get_screen(std::size_t height = 24, std::size_t width,char background); // Error.
        ```
        2. C++在传参时, 无法根据你传入的参数自动推断应该对应到哪个默认参数:
        ```cpp
        auto scr = get_screen('#'); // Passing the ASCII value of '#' to`height`.
        // `width` and `background` are set to
        // default values (`80` and `' '`).
        ```
    2. 函数重载
        1. 函数名可以相同, 但参数类型是不一样的
        ```cpp
        void move_cursor(Coord to);
        void move_cursor(int r, int c); // OK, differ in the number of arguments

        int fun(int);
        double fun(int); // Error: functions that differ only in their return type cannot be overloaded.
        ```

        2. 你需要避免看似"重载"的函数, 有些参数有两种表达形式, 但实际上它们是一样的:
        ```cpp
        // the same function. They are not overloading.
        void fun(int *);
        void fun(int [10]);
        ```

        3. 有些参数确实不同的同名函数, 也确实是函数重载, 但同一变量有可能同时match到两个函数上(exact-match), 这种现象称之为"ambiguious call"
        ```cpp
        void fun(int *a);
        void fun(int (&a)[10]);
        int arr[10]; fun(arr); // Error: ambiguous call
        ```

        4. 有些不那么显然的重载函数调用, 你需要避免使用它们:
        ```cpp
        void fun(int);
        void fun(double);
        void fun(int *);
        void fun(const int *);

        int ival = 42;
        // fun(int *) or fun(const int *)?
        fun(&ival);
        fun('a'); // fun(int) or fun(double)?
        fun(3.14f); // fun(int) or fun(double)?
        fun(NULL); // fun(int) or fun(int *)?
        ```

3. NULL和nullptr
    1. 在C中, 根据平台和库的不同, NULL可能是(void *)0 , 0 , (long)0等等值
        但在C++中, 由于我们ban了强制类型转换, 因此C++中的NULL是int类型的0!
        因此, 在C++中调用func(NULL)会匹配到func(int)这个重载后的函数->往往不是我们想要的!
        正确做法: 在C++中使用nullptr来代表空指针
        ```cpp
        fun(NULL); // May call fun(int),
        // or may be ambiguous.
        fun(nullptr); // Calls fun(int *).
        ```

4. range-based for loop
    1. 你可以使用range for来遍历一个数组, 用于输入(好用!)
    ```cpp
    int arr[100] = {}; // OK in C++ and C23.
    // The following loop will read 100 integers.
    for (auto &x : arr) // int &
    std::cin >> x;
    ```
    2. 如果你希望在函数中使用range for遍历传入的数组参数, 应该使用显式指明数组的参数类型, 而非T *指针(指向数组)
    ```cpp
    void print(int *arr) {
    for (auto x : arr) // Error: `arr` is a pointer, not an array.
        std::cout << x << ' ';
        std::cout << '\n';
    }

    void print(const int (&arr)[100]) {
    for (auto x : arr) // OK. `arr` is an array.
        std::cout << x << ' ';
        std::cout << '\n';
    }
    ```
    注意, 如果你采用后者的定义方式, 那么你只能传入长度为100的数组!(!!!与C不同!)
    ```cpp
    int a[100] = {}; print(a); // OK.
    int b[101] = {}; print(b); // Error.
    double c[100] = {}; print(c); // Error.
    ```






