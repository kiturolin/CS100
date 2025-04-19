1. Class Basic Knowledge
    1. 在 C++中, 无论是类class还是结构体struct, 它们都可以有成员函数
    2. 使用private和public关键字来管理类成员的可访问性, 不需要为每个member都显式声明其为public 或 private, 只需要在class scope中的两个关键字下方声明即可
    3. 当不显式写出private或public关键字时, struct默认这些member是public的, 而class默认这些member是private的, 这是C++中struct和class的唯一区别
    4. this指针: 指向对象自己, 使用this->member来访问对象, 类似python的self
    5. const修饰的成员函数
        1. 例子:
        ```cpp
        class Student {
            std::string name, id;
            int entranceYear;
            public:
            const std::string &getName() const; // returns the name of the student.
            const std::string &getID() const; // returns the id of the student.
            bool valid() const; // verifies whether the leading four digits in `id`
            // is equal to `entranceYear`.
            void adjustID(); // adjust `id` according to `entranceYear`.
        };
        ```
        2. const关键字需要加载参数列表括号")"之后, 函数体大括号"{"之前
        3. 使用const修饰的成员函数将无法修改任何成员变量! 你也无法通过在const函数中调用其他非const函数来修改成员变量: Use const whenever possible.
        注意! const类型的成员函数无法调用非const类型的成员函数!
        ```cpp
        class Student {
            public:
            void foo() const {
                name += 'a'; // Error: `name` is `const std::string` in a const member
                // function. It cannot be modified.
            }
        };
        ```
    6. const修饰的类对象:
        假设现在有一个const修饰的类对象const MyArr my_array, 那么它无法调用类内的非const成员函数!
        因为你使用了const来修饰对象, 相当于你向编译器承诺: 在生命周期内你不会修改my_array(不会改变状态)
        如果你一定要在const对象上使用某个成员函数, 请保证该成员函数前后都有const:
        ```cpp
        class someClass(){
            const int * some_func(...) const {...}
        }
        ```
        第一个const保证返回的对象不可被修改(const对象不会被修改, 你不加这个const也能通过编译, 但极不安全: 外部获取到了可修改的const对象!)
        第二个const保证在该函数内部, 对象不会被修改(const对象调用该函数的必要条件: 如果不是const成员函数, 无法调用!)

    6. 为什么我们需要这么多getter和setter? 直接让外面的代码随意修改就行了吧?
    ```cpp
    class Student {
        std::string name;
        std::string id;
        int entranceYear;
        public:
        const   std::string &getName()    const       { return name; }
        const   std::string &getID() const            { return id; }
        int     getEntranceYear() const                 { return entranceYear; }
        void    setName(const std::string &newName)    { name = newName; }
        void    setID(const std::string &newID)        { id = newID; }
        void    setEntranceYear(int year)              { entranceYear = year; }
    };
    ```
    - 需要手动控制读写的过程, 你不能随意修改, 必须制止你随意的不合法的修改:
        ```cpp
        void setEntranceYear(int year) {
            if (year < 2013) // 制⽌不正确的改动
                throw std::invalid_argument{"Entrance year impossible!"};
            entranceYear = year;
        }
        ```
    - 随意的修改实现细节, 让外部对类成员的操作方法和类成员本身解耦合:
        ```cpp
        class Student {
            std::string name;
            std::string id; // 我不想存 entranceYear 了
            // 没关系，⽤户本来也不能直接访问 entranceYear
        public:
            int getEntranceYear() const { return std::stoi(id.substr(0, 4)); }
        };
        ```

2. Constructors
    0. 例子:
    ```cpp
    class Student {
        std::string name;
        std::string id;
        int entranceYear;
        public:
        Student(const std::string &name_, const std::string &id_, int ey)
        : name(name_), id(id_), entranceYear(ey) {}
        Student(const std::string &name_, const std::string &id_)
        : name(name_), id(id_), entranceYear(std::stoi(id_.substr(0, 4))) {}
    };
    Student a("Alice", "2020123123", 2020);
    Student b("Bob", "2020123124"); // entranceYear = 2020
    Student c; // Error: No default constructor. (to be discussed later)
    ```
    1. 构造函数有以下特点:
        1. 没有返回值类型, 甚至没有void, 函数名必须和类名相同
        2. 常常使用重载来定义多个同名的构造函数, 适用于不同的初始化场景, 拥有不同的参数列表
        3. constructor的函数体一般是空的{}
        4. 语法: 需要一个冒号":"加载参数列表后, 函数体之前, 内部由逗号分割多个initializer(初始化器), initializer的顺序必须与成员变量的顺序严格一致, 同时必须一次性初始化所有成员变量, 语法为member_name(init_value), 注意是圆括号(或大括号{}), 而非赋值 = 
        5. 成员变量在constructor中的初始化顺序为, 按照成员变量的声明顺序进行初始化, 而不是constructor函数的initializer顺序!
            如果你的构造函数初始值列表中的成员的顺序和它们在类内的声明顺序不同，编译器会以 warning 的⽅式提醒你。
        6. 如果constructor中没有初始化某个成员变量怎么办? 会自动调用In-class initializers.

    2. In-class initializers
        1. 即成员变量具有一个默认值, 使用 = 或 {}来确定:
        ```cpp
        class Student {
            std::string name = "Alice";
            std::string id;
            int entranceYear{2024}; // equivalent to `int entranceYear = 2024;`.

            ...
        }
        ```
    
    3. 如果你就是不想用构造器constructor, 会发生什么?
    ```cpp
    class Student {
        // ...
        public:
            Student(const std::string &name_, const std::string &id_) {
            name = name_;
            id = id_;
            entranceYear = std::stoi(id_.substr(0, 4));
        }
    };
    ```
        实际上, 程序会这么运行:
        1. 先调用默认的in-class initializer
        2. 进入函数体, 再次进行你写的所谓的"initialize"
        这涉及到多次赋值, 效率很低
        事实上, 不是所有变量都能够被默认initialize: 
            比如, const类型变量, 引用类型变量等
        3. 不采用constructor的方案会导致你的变量初始化顺序非常混乱, 极易导致错误!
        [Best Practice] 尽量使用constructor
    
    4. Default constructor
        是不接受参数的初始化器, 给每个变量一个"默认值"
        ```cpp
        class Point2d {
            double x, y;
        public:
            Point2d() : x(0), y(0) {}
            Point2d(double x_, double y_) : x(x_), y(y_) {}
        };

        Point2d p1; // 调⽤默认构造函数，(0, 0)
        Point2d p4(); 
        // 这样写是错的! 不会调用默认初始化器, 编译器会认为你试图在声明一个返回值为Point2d类型的函数!
        ```
        一些调用默认构造器的例子:
        ```cpp
        Dynarray a, b{};
        Dynarray *c = new Dynarray, *d = new Dynarray(), *e = new Dynarray{};
        ```

    5. 编译器合成的构造函数
        如果你写的类内部没有任何user-declared的构造函数, 那么编译器会试图帮你合成一个构造函数
        这个构造函数会使用类内初始化值来初始化成员变量, 如果没有类内初始值, 那么它会试图初始化变量
        如果无法默认初始化这个值(比如const类型变量), 编译器会摆烂: 不帮你合成这个默认初始化器

        如果你写的类内存在至少一个user-declared构造函数, 那么编译器不会帮你合成构造函数
            在它看来，这个类的所有合理的初始化⾏为就是你给出的那⼏个构造函数，
            因此不应该再画蛇添⾜地定义⼀个默认⾏为。
        
        你也可以通过加入=default来显式要求编译器合成一个默认构造函数
        ```cpp

        class Student {
        public:
            Student(const std::string &name_, const std::string &id_, int ey)
            : name(name_), id(id_), entranceYear(ey) {}
            Student(const std::string &name_, const std::string &id_)
            : name(name_), id(id_), entranceYear(std::stoi(id_.substr(0, 4))) {}
            Student() = default;
            // 强制合成默认初始化器
        };

        ```

        