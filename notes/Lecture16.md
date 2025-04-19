1. Type Aliasing 类型别名 
    1. C++中有比c的typedef好用100000倍的类型别名系统: using
    ```cpp
    // C-style
    typedef long long LL;
    // C++-style
    using LL = long long;

    // C-style
    typedef int (&ref_to_array)[1000];
    // C++-style
    using ref_to_array = int (&)[1000];

    // C-style
    typedef int (&ref_to_array)[1000];
    // C++-style
    using ref_to_array = int (&)[1000];
    ```

    2. 你也可以在class中使用using自己定义某个类型, 但如果你在类的外面使用这个类型, 请使用ClassName::TypeName格式!
        同时, 这个类型是否public也会影响外部代码的accessbility!
    ```cpp
    class Dynarray {
    public:
        using size_type = std::size_t;
        size_type size() const { return m_length; }
    };

    for (Dynarray::size_type i = 0; i != a.size(); ++i)
    // ...
    ```

    ```cpp
    class A {
        using type = int;
    };

    A::type x = 42; // Error: Accessing private member of `A`.
    ```

    3. 所有标准库的"容器"的.size()成员函数的返回值都是ContainerType::size_type类型的! 而不是C中的size_t或int!
    ```cpp
    std::string::size_type i = s.size();
    std::vector<int>::size_type j = v.size(); // Not `std::vector::size_type`!
                                                // The template argument `<int>`
                                                // is necessary here.
    std::list<int>::size_type k = l.size();
    ```
    因为你很难用统一的size_t来描述各种复杂类型的size, 这是C++更加进步更加现代的体现

2. 类中的static
    1. static变量:
        在类中用static修饰的成员变量是所有类对象"共有"的, 而且所有类对象访问它都会得到相同的值
        所有类对象都可以修改它, 但是! 你需要在类的外面显式声明它, 这个语法非常奇怪且反直觉, 但你要忍一下
        或者你可以理解为, 没有任何类成员初始化器可以"初始化"类的static对象, 因此需要收到初始化static的对象
        ```cpp
        int Dynarray::s_cnt; // Zero-initialize, because it is `static`.
        // or
        int Dynarray::s_cnt = 42; //initialize it with some value explicitly
        ```
    2. 可以将static变量用作类对象的计数器:
    ```cpp
    class Dynarray {
        static int s_cnt; // !!!
        int *m_storage;
        std::size_t m_length;
        int m_id;
    public:
        Dynarray(/* ... */) : /* ... */, m_id(s_cnt++) {}
    };
    ```
    3. static函数:
        与static变量类似, 是所有类对象"共有"的函数
        IMPORTANT: 在这个函数中没有自带的this指针!(因为这个函数不会绑定到任何一个类对象上)
        因此, 你无法在类的static函数中修改类中的非static成员变量!

3. friend关键字
    0. 设想一个需求: 有时我们需要从外部访问一个类的private变量, 这显然是不可能的
        但总会遇到这么些尴尬的时刻, 虽然这不符合设计哲学: private变量就不应该给外部访问
    
    1. 当你真的需要访问private变量时, 可以在class内的任意位置使用friend声明某个函数是自己的friend
        从而让该函数获得private变量的访问权
    ```cpp
    class Student {
        friend void print(const Student &); // The parameter name is not used in this
                                            // declaration, so it is omitted.
        std::string m_name;
        std::string m_id;
        int m_entranceYear;
        public:
        Student(const std::string &name, const std::string &id)
        : m_name(name), m_id(id), m_entranceYear(std::stol(id.substr(0, 4))) {}
        auto graduated(int year) const { return year - m_entranceYear >= 4; }
        // ...
    };

    void print(const Student &stu) {
        std::cout << "Name: " << stu.m_name << ", id: " << stu.m_id
        << "entrance year: " << stu.m_entranceYear << '\n';
    }
    ```

    2. 你可以在类中的任意位置声明friend, 但习惯上我们把friend的声明位置放在每个类的最前面

    3. 考虑friend class: 一个对本class的private成员拥有全部访问权限的类:
    ```cpp
    class X {
        friend class Y;
        // ...
    };
    ```

4. Class的definition与declaration
    1. 类比函数的declaration, class的declaration的定义是: 在class后的大括号{}中包含其所有成员的declaration
    2. 由以上定义立即可得: 你完全可以不在class里定义成员函数的函数体! 而只是保留他们的声明, 如下:
    ```cpp
    class Widget {
    public:
        Widget();
        Widget(int, int);
        void set_handle(int);
        // `const` is also a part of the function type, which should be present
        // in its declaration.
        const std::vector<int> &get_gadgets() const;
        // ...
    private:
        int m_handle;
        int m_length;
        std::vector<int> m_gadgets;
    };
    ```
    3. 但是你总归要明确的定义类成员函数的函数体: 在class外定义!
    ```cpp
    class Widget {
    public:
        const std::vector<int> &get_gadgets() const; // A declaration only.
        // ...
    }; // Now the definition of `Widget` is complete.

    // Define the function here. The function name is `Widget::get_gadgets`.
    const std::vector<int> &Widget::get_gadgets() const {
        return m_gadgets; // Just like how you do it inside the class body.
        // The implicit `this` pointer is still there.
    }
    ```
    注意! 在类外面定义类的函数需要在名称前显式的加上ClassName::运算符

    4. Incomplete Type
    如果我们只知道一个类或结构体的名称, 其余一概不知, 那么这个类的类型就是incomplete type
    我们能做的唯一一件事就是定义一个指向它的指针或者引用:
    ```cpp
    class Student; // We only have this declaration(incomplete).

    void print(const Student &stu) { // OK. Declaring a reference to it is OK.
        std::cout << stu.getName(); // Error. We don't know anything about its members.
    }
    ```
    这个特性在多文件编程中特别好用

5. dtor revisited:
    1. 以下的代码有问题吗: 在destructor中使用了似乎已经被"销毁"的变量:
    ```cpp
    class Student {
        std::string m_name;
        std::string m_id;
        int m_entranceYear;
    public:
        ~Student() { std::cout << m_name << '\n'; }
    };
    ```
    这是完全合法的! 只有当函数体执行完后, 成员变量才会被销毁!

    2.
