1. 右值引用
    0. 动机: 逐元素elementwise拷贝是效率很低的操作, 针对"用后即弃"的右值, 我们希望将右值的内容直接"占为己有", 
        而不是拷贝右值再销毁右值, 这样太蠢了
    1. 注意! 函数的返回值也是一种右值: 它再表达式之后生命周期就结束了
    2. 右值引用: 一种引用类型, 必须绑定到右值上, 无法绑定到左值!
    ```cpp
    int &r = 42; // Error: Lvalue reference cannot be bound to rvalue.
    int &&rr = 42; // Correct: `rr` is an rvalue reference.
    const int &cr = 42; // Also correct:
                        // Lvalue reference-to-const can be bound to rvalue.
    const int &&crr = 42;   // Correct, but useless:
                            // Rvalue reference-to-const is seldom used.
    int i = 42;
    int &&rr2 = i; // Error: Rvalue reference cannot be bound to lvalue.
    int &r2 = i * 42; // Error: Lvalue reference cannot be bound to rvalue.
    const int &cr2 = i * 42; // Correct
    int &&rr3 = i * 42; // Correct
    ```

    3. 观察如下重载的函数签名
    ```cpp
    void fun(const std::string &);
    void fun(std::string &&);
    ```
    fun(s1 + s2)会匹配到第二个函数
    而fun(s1)会匹配到第一个函数

2. 移动操作Move Operation
    0. 两种constructor的区别:
    ```cpp
    Dynarray d3(d1 + d2);   //Move Constructor

    Dynarray d1, d2, d3;
    d3 = d1 + d2;           //Move Assignment Operator
    ```
    1. Move Constructor
        1. 注意constructor的参数列表: &&右值引用类型代表了Move Constructor
        ```cpp
        class Dynarray {
            int *m_storage;
            std::size_t m_length;
        public:
            Dynarray(const Dynarray &other) // copy constructor
            : m_storage(new int[other.m_length]), m_length(other.m_length) {
                for (std::size_t i = 0; i != m_length; ++i)
                    m_storage[i] = other.m_storage[i];
        }
            Dynarray(Dynarray &&other) noexcept // move constructor
            : m_storage(other.m_storage), m_length(other.m_length) {
                    other.m_storage = nullptr;
                    other.m_length = 0;
                }
        };
        ```
        观察Move Constructor内部: 他只是简单的将对方的存储空间"抢占"为自己的, 并将对方的资源清零
        有人问, 只要抢占了资源就好, 为什么还要将other的m_storage指针设置为nullptr?
        因为右值在生命周期结束后会调用dtor释放资源! 如果还让other的m_storage保留,
        那么other的dtor会销毁这块storage! 这是灾难性的!

    2. Move Assignment Operator 
    ```cpp
    class Dynarray {
    public:
        Dynarray &operator=(Dynarray &&other) noexcept {
            if (this != &other) {
                delete[] m_storage;
                m_storage = other.m_storage; m_length = other.m_length;

                // 移动操作应将被移动的对象置于有效的、可被安全析构的状态。
                other.m_storage = nullptr; other.m_length = 0;
            }
            return *this;
        }
    };
    ```
    以上的代码需要确认四件事:
    - Self-assign safe(this != &other)
    - 避免内存泄漏! 这也是为什么需要调用delete[] m_storage的原因
        注意!! 这里delete的是this的m_storage而不是other的m_storage!
        这是因为在这个赋值操作前, this的m_storage已经有值!(与Move Constructor不同的地方)
        因此在抢占前你必须手动释放之前分配的内存, 避免内存泄漏
    - 抢占资源(最简单)
    - 移动操作应将被移动的对象置于有效的、可被安全析构的状态。

    ```cpp
    a = concat(b, c); // calls move assignment operator,
    // because `concat(b, c)` generates an rvalue.
    a = b; // copy assignment operator
    ```
    小练习: 以下的代码有什么问题? Look at my eyes! 回答我!
    ```cpp
    class Dynarray {
    public:
        Dynarray &operator=(const Dynarray &other) {
            auto new_data = new int[other.m_length];
            std::copy_n(other.m_storage, other.m_length, new_data);
            m_storage = new_data;
            m_length = other.m_length;
            return *this;
        }
    };
    ```
    3. 编译器自动合成的移动操作constructor与运算符重载
    ```cpp
    struct X {
        X(X &&) = default;
        X &operator=(X &&) = default;
    };
    ```

    使用=default关键字要求编译器为你手动合成一个移动构造器

    4. 使用std::exchange来更方便地实现移动构造函数:
    ```cpp
    class Dynarray {
    public:
        Dynarray(Dynarray &&other) noexcept
        : m_length{std::exchange(other.m_length, 0)},
            m_storage{std::exchange(other.m_storage, nullptr)} {}
        Dynarray &operator=(Dynarray &&other) noexcept {
            if (this != &other) {
                delete[] m_storage;
                // !!! !!!
                m_length = std::exchange(other.m_length, 0);
                m_storage = std::exchange(other.m_storage, nullptr);
            }
        }
    };
    ```

3. The Rule of Five:
    以下的"拷贝控制成员", 你要么全部定义(定义五个), 要么一个也不定义
    - Copy Constructor
    - Copy Assignment Operator
    - Move Constructor
    - Move Assignment Operator
    - Destructor

4. std::move
    1. 这个函数可以帮助你将左值转换成右值, 从而匹配T &&类型的参数->你可以直接"移动"它!
        但这看起来很诡异: 你抢夺了一个"左值"的资源! 而左值的生命周期应该是持久的!
        这就是问题所在, std::move相当于一个声明: 我不会再使用这个左值变量了
        或者说: 我希望就地终止这个左值变量的生命周期, 并允许其它对象抢夺它的资源
    
    2. 例子
    ```cpp
    int ival = 42;
    int &&rref = ival; // Error
    int &&rref2 = std::move(ival); // Correct

    void foo(X &&x); // moves `x`
    void foo(const X &x); // copies `x`
    foo(std::move(x)); // matches `foo(X&&)`, so that `x` is moved.
    ```

    3. 比较以下两个constructor
    ```cpp
    Dynarray(Dynarray &&other) noexcept
    : m_storage(other.m_storage), m_length(other.m_length),
        m_label(other.m_label) {        // Isn't this correct?
        other.m_storage = nullptr;
        other.m_length = 0;
    }

    Dynarray(Dynarray &&other) noexcept
    : m_storage(other.m_storage), m_length(other.m_length),
        m_label(std::move(other.m_label)) { // !!
        other.m_storage = nullptr;
        other.m_length = 0;
    }

    ```

    注意在构造器列表中m_label的处理: 你应该使用std::move()而不是直接写other.m_label
    这是因为: 在构造器函数中, other是一个左值!(虽然他实际上是右值)
    Golden rule: Anything that has a name is an lvalue.
    所以, 右值引用是一个左值!
    因此你如果直接写other.m_label, 这是一个拷贝操作!

5. Move and Copy Elision
    考虑如下代码:
    ```cpp
    std::string foo(const std::string &a, const std::string &b) {
        return a + b; // a temporary
    }
    std::string s = foo(a, b);
    ```
    s的赋值操作时copy还是move?

    两者都不是! s将会被直接以a+b初始化, 如下:
    ```cpp
    std::string s(a + b);
    ```
    在C++17后这被称作copy elison

6. Named Return Value Optimization, NRVO
    考虑如下代码:
    ```cpp
    Dynarray concat(const Dynarray &a, const Dynarray &b) {
        Dynarray result(a.size() + b.size());
        // ...
        return result;
    }
    Dynarray a = concat(b, c); // Initialization
    ```
    经过编译器的NRVO优化后, 代码会被修改为:
    ```cpp
    // Pseudo C++ code.
    void concat(Dynarray &result, const Dynarray &a, const Dynarray &b) {
        // Pseudo C++ code. For demonstration only.
        // 以下语句在函数头第一行发生!
        result.Dynarray::Dynarray(a.size() + b.size()); // construct in-place
        // ...
    }
    Dynarray a@; // Uninitialized.
    concat(a@, b, c);
    ```
    相当于直接给concat函数加了一个参数: 传入未经初始化的最终变量a, 所有的操作都会在a中进行
    完全规避了任何可能发生的move和copy, 相当于完全不用中间变量!

7. Pass by value then move
    一种非常取巧的写法! 考虑一个类中的字符串成员, 你需要对它做拷贝控制
    按理说你需要手动写Move Constructor和Copy Constructor
    但这种方法允许你"利用"std::string类中自带的拷贝控制系统来帮你决定是要Move还是Copy
    ```cpp
    class Message {
        std::string m_contents;
    public:
        explicit Message(std::string contents) : m_contents{std::move(contents)} {}
    };
    ```
    注意此处的构造函数的参数是按值传递的! 所以无论任何, 进入这个函数就会创建一个新的std::string content对象
    好的, 现在如果传入的是左值:
    相当于赋值操作content = lvalue, 这个左值赋值的操作会在std::string中被重载: 使用copy
    如果传入的是右值:
    相当于content = rvalue, std::string内的Move Assignment Operator会发力, 转而使用move操作

    而在函数体内部, 左值contents都会被std::move()转换为右值, 再赋值给m_contents, 类似于
    m_contents = rvalue, 此处再次利用了std::string的Move Assignment Operator移动语义

    因此:
    对于左值输入 (拷贝情况): 会有一次拷贝（初始化 contents）+ 一次移动（初始化 m_contents），而两个构造函数的版本只需要一次拷贝。
    对于右值输入 (移动情况): 会有一次移动（初始化 contents）+ 一次移动（初始化 m_contents），而两个构造函数的版本只需要一次移动。


8. Copy-and-swap
问题的起因: 赋值 = 拷贝新值 + 析构(销毁)旧值, 我们能不能利用拷贝构造函数和析构函数来实现重载赋值运算符?
先实现一个简单的swap函数: 只交换指针, 几乎零开销!又快又好
```cpp
class Dynarray {
public:
    void swap(Dynarray &other) noexcept {
        std::swap(m_storage, other.m_storage);
        std::swap(m_length, other.m_length);
    }
};
```
注意以下的代码:
```cpp
class Dynarray {
public:
    Dynarray &operator=(const Dynarray &other) {
        auto tmp = other;
        swap(tmp);
        return *this;
    }
};
```
拷贝构造函数会负责正确的将值从other搬运到tmp中
随后调用swap, 将当前对象的值与tmp的值互换: 此时已经完成了赋值!
最后离开函数作用域, tmp将被析构(注意! 此时tmp内的内容就是对象本来的内容! 因为swap过)

进化! 又好写又简单而且自我赋值安全
```cpp
class Dynarray {
public:
    Dynarray &operator=(const Dynarray &other) {
        Dynarray(other).swap(*this); // C++23: auto{other}.swap(*this);
        return *this;
    }
};
```

更进一步, 我们可以直接在传参的时候就做好拷贝: 参数采用值传递而非引用!
```cpp
class Dynarray {
public:
    Dynarray &operator=(Dynarray other) noexcept {
        swap(other);
        return *this;
    }
};
```
在传参时, 如果传入右值, other会被移动初始化, 如果传入左值, 那么会被拷贝初始化
因此, 对于传入右值或者左值的情况都能胜任
这既是一个拷贝赋值运算符, 又是一个移动赋值运算符!

