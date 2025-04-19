1. 每个局部变量的生命周期都如下: 自其被初始化开始, 到它所在的作用域结束而结束, 需要特别注意, 块作用域包含if, else, for循环等
    单独的大括号括起来的语句块也是一个作用域!
    ```cpp
    {
        int a = 0;
        float c = 2.56;
    }
    ```
    然而, 某些局部变量持有一些资源, 每次它退出作用域你就需要手动释放它持有的资源, 比如std::string可能占有一块内存空间, 当它的生命周期结束后, 你需要free掉它的内存空间

2. 因此, 在生命周期的开始和结束, 你需要构造器和destructor来自动帮你分配和释放资源(destructor称作"析构函数")
    Destructor定义: A destructor of a class is the function that is automatically called when an object of that class type is destroyed

3. 析构函数的具体行为
    1. 在类类型变量生命周期结束时, 析构函数会被自动调用来销毁对象
    2. 按照成员变量声明的顺序倒序销毁他们
    3. 如果成员变量自身也是一个类类型变量(比如std::string), 那么析构函数会自动调用他们的析构函数
        但是不需要你显式的调用! 编译器会自动插入这些代码
    ```cpp
    class Student {
        std::string name;
        std::string id;
        int entranceYear;
    public:
        ~Student() {} // 编译器会在最后插⼊代码来调⽤ id 和 name 的析构函数。
    };
    ```
    一些常见的错误
    ```cpp
    class Dynarray {
        std::size_t m_size;
        int *m_storage;
    public:
        ~Student() {
            delete[] m_storage;
            delete this; // 错误！
            // 下⾯的赋值毫⽆意义。
            m_size = 0;
            m_storage = nullptr;
        }
    };
    ```

    C++不允许存在不可调用的析构函数! 这意味着它无法被销毁! 你没法delete它
    什么情况下析构函数不可被调用?
    a. 显示的标注~ClassName() = delete;
    b. 如果析构函数时private的, 那么它无法在类外被调用



3. destructor语法: ~Classname(){...}
    example:
    ```cpp
    struct A {
        A() {
            std::cout << 'c';
        }

        ~A() {
            std::cout << 'd';
        }
    };

    for (int i = 0; i != 3; ++i) {
        A a;
        // do something ...
    }

    // Output:
    // Called automatically when the object is destroyed
    cdcdcd
    ```

4. 你可以利用destructor来帮助你自动释放资源! 避免内存泄漏等情况!
    ```cpp
    class A {
        SomeResourceHandle resource;
        public:
        A(/* ... */) : resource(obtain_resource(/* ... */)) {}
        
        ~A() {
            release_resource(resource);
        }
    };
    ```

5. 使用构造器来实现Dynamic Array: 注意构造器的语法! 圆括号内的语句(...)应该是成员变量的值!
    如果你自己定义了一个构造器, 那么编译器不会帮你生成默认构造器!
    ```cpp
    class Dynarray {
        int *m_storage;
        std::size_t m_length;
    public:
        Dynarray(std::size_t n) : m_storage(new int[n]{}), m_length(n) {}

        ~Dynarray() {
            delete[] m_storage; // Pay attention to `[]`!
        }
    };

    // No default constructor.
    Dynarray a; // Error.
    ```

5. 有一个很容易犯的错误: 你绝对不应该在析构函数中手动调用成员变量的析构函数!
    ```cpp
    class Student {
        std::string m_name;
        std::string m_id;
        int m_entranceYear;
    public:
        ~Student() { (手动调用m_name的析构函数); }
    };
    ```
5. 析构函数的妙用: guard类型变量使用析构函数来保证退出生命周期后, 资源被合理的释放:
    ```cpp
    struct PrinterGuard {
        PrinterGuard() {
            hide_cursor();
            disable_input_echo();
        }
        ~PrinterGuard() {
            show_cursor();
            enable_input_echo();
        }
    };
    void drawUI(/* ... */) {
        PrinterGuard guard;
        // 画⼀些东⻄ ...
        // guard 被销毁，光标和输⼊显⽰被恢复
    }

    void loadMap(const std::string &path) {
        std::ifstream file(path);
        // ...
        // std::ifstream 的析构函数会关闭⽂件
    }

    void criticalSection(std::mutex &m) {
        std::lock_guard guard(m);
        // ...
        // std::lock_guard 的析构函数会解锁 m
    }
    ```

6. 如果new[0], 那么要不要释放内存, 或者说, 下面的代码是正确的吗?
    ```cpp
    class Dynarray {
        // ...
        ~Dynarray() {
        if (m_length != 0)
            delete[] m_storage;
        }
    };
    ```
    不正确! 即使new[0]也会分配某个内存, 这是implementation defined的! 也必须要deallocate掉它!

7. new和delete的行为:
    1. new会先分配内存, 再调用初始化器构造对象
    2. delete会先调用析构函数销毁对象, 然后释放内存

    ```cpp
    std::string *p = new std::string("Hello");
    std::cout << *p << std::endl;
    delete p;
    p = new std::string; // 调⽤了默认构造函数，*p 为空串 ""
    std::cout << p->size() << std::endl;
    delete p;
    ```

8. const成员变量:
    bg: 如果我们希望使用at()成员函数来访问动态数组下标处的某个值, 应该如何实现?
    ```cpp
    class Dynarray {
        std::size_t m_size;
        int *m_storage;
    public:
        int &at(std::size_t n) {
            return m_storage[n];
        }
    };

    void print(const Dynarray &arr) {
        for (std::size_t i = 0; i != arr.size(); ++i)
            std::cout << arr.at(i) << ' ';
    }
    // ⽆法编译： at 不是 const 成员函数，⽆法在 const Dynarray & 上调⽤！
    ```

    正确的解决方案: 使用const和non-const重载同一个函数:
    ```cpp
    class Dynarray {
            std::size_t m_size;
            int *m_storage;
        public:
            const int &at(std::size_t n) const {
                return m_storage[n];
            }
            int &at(std::size_t n) {
                return m_storage[n];
            }
    };
    ```
    因此, const对象只会调用const函数, 而non-const对象则会调用非const版本的函数(这是完美匹配)

9. 在函数重载中避免重复
    重载的两个函数, 函数体都是一模一样的内容, 写两遍实在太不优雅了:
    ```cpp
    class Dynarray {
    public:
        const int &at(std::size_t n) const {
            if (n >= m_length)
                throw std::out_of_range{"Dynarray subscript out of range."};
            log_access();
            verify_integrity();
            return m_storage[n];
        }

        int &at(std::size_t n) {
            if (n >= m_length)
                throw std::out_of_range{"Dynarray subscript out of range."};
            log_access();
            verify_integrity();
            return m_storage[n];
        }
    };
    ```

    解决方案: 让non-const版本的函数调用const版本的函数
    为了实现这个调用, 我们在non-const函数中手动给this指针加上const, 以合法地调用const函数
    再在返回的时候, 手动去除const函数返回值中的const修饰符, 得到可改变的返回值:
    1. 先⽤ static_cast<const Dynarray *>(this) 为 this 添加底层 const
    2. 这时调⽤ ->at(n) ，就会匹配 const 版本的 at
    3. 将返回的 const int & 的底层 const ⽤ const_cast 去除

    ```cpp
    class Dynarray {
    public:
        const int &at(std::size_t n) const {
            if (n >= m_length)
                throw std::out_of_range{"Dynarray subscript out of range."};
            log_access();
            verify_integrity();
            return m_storage[n];
        }

        int &at(std::size_t n) {
            return const_cast<int &>(static_cast<const Dynarray *>(this)->at(n));
        }
    };
    ```

    能不能反过来, 让const版本调用non-const函数?
    不能! 如果此时non-const意外修改了成员变量的值, 会导致灾难性的后果
    注意! const_cast很危险!
    先添加const再去除const是安全的
    而先去除const再添加const非常危险!

10. 拷贝控制Copy control
    我们可以非常方便的创建字符串的拷贝, 如下s2, s3, s4的值都是s1的值, 但他们四个都是互相独立的对象:
    ```cpp
    std::string s1 = some_value();
    std::string s2 = s1; // s2 is initialized to be a copy of s1
    std::string s3(s1); // equivalent
    std::string s4{s1}; // equivalent, but modern
    ```

    对于一般的类(比如Dynarray), 如果要实现精确的拷贝, 你需要在类中显式定义Copy constructor, 比如下面的操作
    ```cpp
    Type b = a;
    Type b(a);
    Type b{a};
    ```

    实现Dynarray的copy constructor:
    ```cpp
    class Dynarray {
    public:
        Dynarray(const Dynarray &other)
        : m_storage(new int[other.size()]{}), m_length(other.size()) {
            for (std::size_t i = 0; i != other.size(); ++i)
            m_storage[i] = other.at(i);
        }
    };
    ```
    注意这个copy constructor的名称就是类名! 且参数为另一个Dynarray对象的"引用"!
    为什么使用引用: 
    - 避免值传递, 这效率很低而且可能导致递归(?)
    为何使用const?
    - 我们不希望原始对象在拷贝过程中被修改

    注意, 这个copy构造器中我们不仅为新对象分配了内存, 还逐个拷贝了成员变量的值
    如果这个类没有user-declared的copy constructor, 那么编译器会自动帮class合成一个
    默认的copy constructor逻辑非常简单: 它只是自动的拷贝所有成员变量的值:
    ```cpp
    class Dynarray {
    public:
        Dynarray(const Dynarray &other)
        : m_storage(other.m_storage), m_length(other.m_length) {}
    };
    ```
    这显然不是我们想要的! 因为此时两个对象在共用同一块内存空间m_storage! 这很危险!

    如果我们明确知道我们不需要拷贝行为(比如某个复杂的客制化对象, 我们确实不需要拷贝)
    那么可以用delete关键字阻止编译器为我们合成默认的拷贝函数
    ```cpp
    class ComplicatedDevice {
    // some members
    // Suppose this class represents some complicated device,
    // for which there is no correct and suitable behavior for "copying".
    public:
        ComplicatedDevice(const ComplicatedDevice &) = delete;
    };

    ComplicatedDevice a = something();
    ComplicatedDevice b = a; // Error: calling deleted function
    ```

    以上我们谈论的内容都是Copy-Initialization(在初始化时拷贝某个已经存在的对象)
    Copy-Initialization发生在declaration statement(声明语句)中
    Copy-Assignment发生在Assign expression(赋值表达式)中
    ```cpp
    std::string s1 = s2 //declaration statement, not an expression

    std::string s1 = "hello", s2 = "world";
    s1 = s2; // Assign expression
    ```

    下面我们来探讨Copy-Assignment
    ```cpp
    class Dynarray {
    public:
        Dynarray &operator=(const Dynarray &other) {
            int *new_data = new int[other.size()];
            for (std::size_t i = 0; i != other.size(); ++i)
                new_data[i] = other.at(i);
            delete[] m_storage;
            m_storage = new_data;
            m_length = other.size();
            return *this;
        }
    };
    ```

    手动定义Copy-Assignment的行为需要重载运算符(确切的是重载"="运算符)
    该函数的返回值为一个Dynarray类型的引用! 和constructor不同!
    该函数的名称为operator=
    我们认为返回值是"="运算符左侧的对象的引用(即"被赋值对象"的引用, "a=b"中的a)
    而参数为右侧对象的引用

    这个函数你需要注意以下两点:
    1. 在复制对象后, 正确地释放自己原本持有的资源(m_storage), 在此处使用delete
    2. 要保证self-assignment的安全性! a = a需要是安全的!

    为了做到第二点, 我们不能在函数开头就delete[] m_storage, 这会导致self-assign时数据丢失!
    因此要引入中间变量

11. The rule of three:
    copy ctor, copy assignment operator, dtor这三个构造器中, 你一旦需要自定义它们其中的一个
    则你一般也要定义剩下的两个! 
    如果你在这三个函数中至少自定义了一个, 那么编译器不会自动帮你合成这些函数!(三个都不会! 都需要你定义!)
    




