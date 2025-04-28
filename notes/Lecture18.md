## 智能指针

### std::unique_ptr<T>
0. 含义:表示对某个对象完全的所有权: 该对象的指针有且只有一个, 是排他的! 指针到对象的映射是单射
1. 例子代码:
```cpp
class WindowPtr {
    Window *ptr;
public:
    // 此处的 = nullptr的意思是默认参数为nullptr, 并不是赋值语句!
    // 即你不传入参数时, 默认p为nullptr
    WindowPtr(Window *p = nullptr) : ptr(p) {}
    // 利用destructor做自动垃圾回收, 生命周期结束时, 自动释放内存
    ~WindowPtr() { delete ptr; }
    // 不允许使用另一个已经存在的unique_ptr来初始化新的unique_ptr!(否则指向了同一个对象)
    WindowPtr(const WindowPtr &) = delete;
    // 不允许使用赋值语句复制另一个unique_ptr
    WindowPtr &operator=(const WindowPtr &) = delete;
    // Move COnstructor: 移动语义: 你可以移动右值类型的unique_ptr, 表现为资源抢占, 原来的unique_ptr将失效
    WindowPtr(WindowPtr &&other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
    // Move Assignment Operator:
    WindowPtr &operator=(WindowPtr &&other) noexcept {
        if (&other != this) {
            delete ptr; ptr = other.ptr; other.ptr = nullptr;
        }
        return *this;
    }
};
```

2. 注意unique_ptr是一种Move-only Type: 只能通过右值来传递内容
3. std::unique_type是模板名称, 而非类型名称! 和std::vector一样
    完整的类型名称应该是std::unique_type<T>
    若T!=U, 那么std::unique_type<T> != std::unique_type<U>

4. 创建std::unique_ptr的两种方式: constructor与make_unique完美转发
```cpp
std::unique_ptr<Student> p(new Student("Bob", 2020123123));

std::unique_ptr<Student> p1 = std::make_unique<Student>("Bob", 2020123123);
auto p2 = std::make_unique<Student>("Alice", 2020321321);
```

5. 如果你在初始化某个unique_ptr时不传入任何参数, 那么它将会是nullptr空指针
```cpp
std::unique_ptr<T> up;
```

6. unique_ptr的Move Only语法:
```cpp
auto p = std::make_unique<std::string>(5, 'c');
std::cout << *p << std::endl; // Prints "ccccc".
auto q = p; // Error. Copy is not allowed.

auto r = std::move(p); // !!!Correct.!!!

// Now the ownership of this string has been transferred to `r`.

std::cout << *r << std::endl; // Prints "ccccc".
if (!p) // true
std::cout << "p is \"null\" now." << std::endl;
```
注意! 在move完成后, 原来的unique_ptr会变成nullptr! 这是因为move操作会销毁原来的对象
(详见上面的代码)

7. 如果你返回一个unique_ptr, 那么会隐式的使用移动语义来"传递"指针的内容, 因为函数的返回值是右值:
```cpp
struct Window {
    // A typical "factory" function.
    static std::unique_ptr<Window> create(const Settings &settings) {
        auto pW = std::make_unique<Window>(/* some arguments */);
        logWindowCreation(pW);
        // ...
        return pW;
    }
};
auto state = Window::create(my_settings);
```
虽然这样的移动语言可能被NVRO优化

8. std::unique_ptr是零成本抽象! 使用unique_ptr和裸指针不会有任何时间上或空间上的区别!

9. 如果我需要一个指向数组的unique_ptr怎么办? 要知道, unique_ptr内部是通过delete来释放资源的
    而释放数组需要使用delete[] !
    解决方案: 使用std::unique_ptr<T[]>, 它内部使用delete[]来销毁对象
    但使用这种方案生成的unique_ptr无法支持operatoe->, operator*的操作!
    (忘掉C语言的"数组名就是首地址"吧!)
```cpp
auto up = std::make_unique<int[]>(n);
std::unique_ptr<int[]> up2(new int[n]{}); // equivalent
for (auto i = 0; i != n; ++i)
    std::cout << up[i] << ' ';
```

### std::shared_ptr<T>
0. 允许多个指针指向同一个变量, 或者说, 允许"被复制copy"而不是move only
    一个自然的问题是: 我们什么时候释放内存? 现在有很多个不同生命周期的指针指向同一块内存
    解决方案: 引用计数!
1. 例子:
```cpp
class WindowPtr {
    WindowWithCounter *ptr;
public:
    // Copy (Assignment) Constructor
    WindowPtr(const WindowPtr &other) : ptr(other.ptr) { ++ptr->refCount; }
    WindowPtr &operator=(const WindowPtr &other) {
        // 先加上对方引用计数: 保证self-assign的安全性! ptrA = ptrA不会导致ptrA被delete
        ++other.ptr->refCount;
        // ptrA = ptrB, 那么ptrA所指向的对象就少了一个指向它的指针, 因此要引用计数减一
        // 或者说, 原来指向A的指针现在指向了B, 自然A的引用计数要减一
        if (--ptr->refCount == 0)
            delete ptr;
        ptr = other.ptr;
        // 运算符重载必须显式地返回*this!!!
        // *this是对this指针的解引用, 类型为WindowPtr
        // 但是函数返回的是WindownPtr &, 这会导致编译器做一个"类型转换"
        // 实际上返回的是引用
        return *this;
    }

    //Move (Assignment) Constructor
    // 由于是移动, 是"抢夺"式的赋值, 所以根本不需要更新引用计数!
    // 比如WindowPtr ptrA(std::move(ptrB)), 原先ptrB指向B, 现在ptrA指向B而ptrB为nullptr
    // 从对象B的视角来看, 没有引用计数的变化!
    // 而ptrA在这个表达式之前不存在, 从未被初始化! 因此也不需要更新ptrA指向对象的引用计数(ptrA不指向任何一个对象)
    WindowPtr(WindowPtr &&other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
    WindowPtr &operator=(WindowPtr &&other) noexcept {
        if (this != &other) {
            // ptrA = ptrB
            // 你需要更新A对象的引用计数! 因为此时ptrA指向了其他对象
            if (--ptr->refCount == 0)
                delete ptr;
            ptr = other.ptr; other.ptr = nullptr;
        }
        return *this;
    }

    ~WindowPtr() {
        if (--ptr->refCount == 0)
            delete ptr;
    }
};
```
2. 创建智能指针: 使用std::make_shared()
make_shared 并不是“无中生有”，它执行了在堆上分配内存并调用构造函数来创建和初始化对象的操作，只是这个过程被封装在了函数内部。
std::make_shared()是一个方便且高效的方式，用于在堆上动态创建一个 T 类型的对象（使用 args... 作为构造函数参数进行初始化），
并立即用一个 std::shared_ptr<T> 来管理这个新对象的生命周期。
你不需要（也不应该）先手动 new 一个对象再传给 shared_ptr 的构造函数（虽然也可以，但效率较低且有异常安全风险），

__make_shared 一步就帮你完成了内存分配、对象构造和智能指针设置__
```cpp
// sp points to a string "cccccccccc".
auto sp = std::make_shared<std::string>(10, 'c');
auto pWindow = std::make_shared<Window>(80, 24, my_settings.mode);
```
[Best practice] 永远优先使用make_shared来创建shared_ptr

3. 你可以使用use_count来查看当前指针指向对象的引用计数:
```cpp
auto sp = std::make_shared<std::string>(10, 'c');
{
    auto sp2 = sp;
    std::cout << sp.use_count() << std::endl; // 2
} // `sp2` is destroyed, but the managed object is not destroyed.
std::cout << sp.use_count() << std::endl; // 1
```

4. shared_ptr不是零成本抽象! 因为你需要维护一个引用计数器!

5. 完美转发: make_shared和make_unique会把传入的参数忠实的完整地完美地传递给对应类型的构造函数!
    传入右值就会触发对应类型构造函数的移动语义, 完全保留传入参数的类型与左右值状态, 这被称作完美转发
```cpp
auto sp = std::make_shared<std::string>(10, 'c'); // "cccccccccc"

// 传入右值, 意为说明"你可以窃取sp的资源", 因此出发了std::string的Move Constructor, 将sp指向的资源抢夺走了!
// 因此sp指向的对象会被"归零"
auto sp3 = std::make_shared<std::string>(std::move(*sp));
std::cout << *sp << std::endl; // empty string
                                // 这确实证明了我们成功出发了Move Constructor!
```

6. 