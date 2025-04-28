## 运算符重载
0. 运算符重载的一些规则:
 - 各个运算对象从左向右依次作为这个函数的参数。
 - 如果是成员函数，则最左侧的运算对象绑定到 *this 。

1. 运算符重载有两种方式: 作为某个对象类内的成员函数, 以及非成员函数
```cpp
a[i]    //a.operator[](i)
a = b   //a.operator=(b)
*a      //a.operator*()
f(arg1, arg2, arg3, ...)    //f.operator()(arg1, arg2, arg3, ...)
a == b  //operator==(a, b)
a + b   //operator+(a, b)
```

2. 但有些运算符不建议重载: 比如&&和||, 你无法保证重载后的运算符还能保持"短路"的特性!

3. 运算符重载的规则: 至少一个操作数需要时你定义的类; 你不能发明或者新定义一个新的运算符
```cpp
int operator+(int, int); // Error.
MyInt operator-(int, int); // Still error.
double operator**(double x, double exp); // Error.
```
运算符重载不会改变优先级, 以及evaluation order
```cpp
std::cout << a + b; // Equivalent to `std::cout << (a + b)`.
```

4. Example: 有理数Rationale 这段代码有太多需要注意的地方了!
```cpp
class Rational {
    friend Rational operator-(const Rational &); // Unary `operator-` as in `-x`.
    int m_num; // numerator
    unsigned m_denom; // denominator IMPORTANT: 注意这里的unsigned!!!
public:
    Rational &operator+=(const Rational &rhs) {
        m_num = m_num * static_cast<int>(rhs.m_denom) // Be careful with `unsigned`! IMPORTANT!!
            + static_cast<int>(m_denom) * rhs.m_num;    // 注意我们在进行int与uint的混合计算! 必须进行显式类型转换!
        m_denom *= rhs.m_denom;
        simplify();
        return *this; // `x += y` should return a reference to `x`.
                        // 反直觉! 这里的(x += y)表达式的返回值应该是x!
    }
};

Rational operator-(const Rational &x) {
    return {-x.m_num, x.m_denom};
    // The above is equivalent to `return Rational(-x.m_num, x.m_denom);`.
    // IMPORTANT! 相当于直接返回了一个构造器!
}
```

5. 什么时候用成员函数类型的运算符重载, 什么时候不用成员函数?
    成员函数运算符重载总是比对应的非成员函数版本少一个显式参数（因为左操作数或唯一操作数通过 this 隐式传递了）!
    - 当你需要对称性, 即左操作数不是你定义的类对象时(比如int + MyClass), 使用成员函数运算符重载就无法满足你的要求
        阿贝尔群上(有交换律/对称的)的运算符就需要使用非成员函数来重载!

6. 继续完善Rationale类: 巧妙运用+=运算符! 注意是否是成员函数! 注意到二元运算符一般是不适用成员函数来重载
```cpp
class Rational {
public:
    Rational &operator-=(const Rational &rhs) {
        // Makes use of `operator+=` and the unary `operator-`.
        return *this += -rhs;
    }
};
Rational operator+(const Rational &lhs, const Rational &rhs) {
    return Rational(lhs) += rhs; // Makes use of `operator+=`.
}
Rational operator-(const Rational &lhs, const Rational &rhs) {
    return Rational(lhs) -= rhs; // Makes use of `operator-=`.
}
```

```cpp
// 假设我们已经实现了operator<
bool operator>(const Rational &lhs, const Rational &rhs) {
    return rhs < lhs;
}
bool operator<=(const Rational &lhs, const Rational &rhs) {
    return !(lhs > rhs);
}
bool operator>=(const Rational &lhs, const Rational &rhs) {
    return !(lhs < rhs);
}
bool operator!=(const Rational &lhs, const Rational &rhs) {
    return !(lhs == rhs);
}
```

7. [Best Practice] Avoid Repetition: 只实现一两个核心的运算符重载, 其他的重载函数依赖这个"元函数"来实现!

8. 注意++x和x++: 自增自减运算符的重载并不一样!
    ++x --> x.operator++(void)
    x++ --> x.operator++(0)

    x++的重载函数会有一个int类型的dummy parameter用以区分++x和x++!
```cpp
class Rational {
public:
    // 想想看: 为什么++x的返回值是x本身的引用, 而x++返回的是一个值?
    // IMPORTANT: 
    Rational &operator++() { ++m_num; simplify(); return *this; }
    Rational operator++(int) { // This `int` parameter is not used.
        // The postfix version is almost always defined like this.
        auto tmp = *this;
        ++*this; // Makes use of the prefix version.
        return tmp;
    }
};
```

9. 重载IO运算符: 必须使用 非成员函数来重载! 因为其参数中有输入输出流! 而你定义的类对象的成员函数无法接受输入输出流作为参数!
std::istream 和 std::ostream 都不能拷⻉，必须按引⽤传递，⽽且不能是常量引⽤。
```cpp
std::ostream &operator<<(std::ostream &, const Rational &);
std::istream &operator>>(std::istream &, Rational &);

// 错误! 不能使用常量引用!
std::istream &operator>>(const std::istream &, Rational &); // Error!
```

10. 有关Rationale的输出输出流重载:
```cpp
std::istream &operator>>(std::istream &is, Rational &r) {
    int x, y; is >> x >> y; // 这句话的意思是从is中读取两个int类型的变量
                            // 这是重载输入输出流的核心! 
    if (!is) { // Pay attention to input failures!
                // 注意! input failure对应的istream为0!
        x = 0;
        y = 1;
    }
    if (y < 0) { y = -y; x = -x; }
    r = Rational(x, y);
    return is;
}
```

11. 使用[]下标运算符重载: 用于Dynarray
```cpp
class Dynarray {
public:
    int &operator[](std::size_t n) {
        return m_storage[n];
    }
    const int &operator[](std::size_t n) const {
        return m_storage[n];
    }
};
```
    为什么需要两种类型的(const和非const)的[]下标运算符重载?
    因为const类型的Dynarray对象只能调用const类型的成员函数! 所以你必须写这个重载!

12. 重载解引用*运算符: 用于WindowPtr
```cpp
class WindowPtr {
    WindowWithCounter *m_ptr;
public:
    Window &operator*() const {
        return m_ptr->theWindow;
    }
};
```
为什么operator*函数需要是const的? 因为你需要保证const的Window对象可以调用!
记住! const的对象__不能调用__非const的成员函数!
然而以上的代码仍然有一个问题: 你可以尝试修改返回的对象! 而这不是const函数所承诺的内容: 保证不会修改原对象

13. 重载operator-> 用于WindowPtr
```cpp
class WindowPtr {
public:
    Window *operator->() const {
        return std::addressof(operator*());
    }
};
```
其实std::addressof(x)的作用和&x几乎无异, 但关键点是, 程序员很可能自己重载了&运算符!因此你最好不要用&!

14. 重载类型转换运算符:
```cpp
class YourClass {
public:
    // 语法: operator TargetType() [const] [explicit] [noexcept] { ... }
    operator TargetType() const {
        // ... 实现转换逻辑 ...
        // 必须返回一个 TargetType 类型的值，
        // 或者可以隐式转换为 TargetType 的值。
        TargetType result = /* ... */;
        return result;
    }
    // ... 其他成员 ...
};
```
注意以下几点:
 - 类型转换运算符只能定义为成员函数
 - 语法一定是operator起手, operator是一个关键字! 而不是返回值类型
 - 必须加上const修饰!

如果你不期望发生如下的隐式类型转换:
```cpp
class Rational {
public:
    // conversion from `Rational` to `double`.
    operator double() const { return 1.0 * m_num / m_denom; }
};
Rational r(3, 4);
double dval = r; // 0.75 IMPORTANT! 此处发生了隐式类型转换!

void bar(const std::vector<int> &vec) { /* ...*/ }
bar(1000); // ??? Too weird!

std::vector<int> v2 = 1000; // No! This should never happen. Too weird!
```
你就应该在类型转换重载函数的前面使用explicit关键字:
这样就保证了你只能使用TypeName(ClassType)的方式来实现类型转换, 而不会发生任何形式的隐式类型转换
```cpp
class Rational {
public:
    explicit operator double() const { return 1.0 * m_num / m_denom; }
};

Rational r(3, 4);
double d = r; // Error.
void foo(double x) { /* ... */ }
foo(r); // Error.
foo(double(r)); // OK.
foo(static_cast<double>(r)); // OK. 
```

14. 但有一个例外: bool类型的类型转换, 即使你加了explicit, 在以下场景中编译仍然通过:
 - if (expr) , while (expr) , for (...; expr; ...) , do ... while (expr)
 - as the operand of ! , && , ||
 - as the first operand of ?: : expr ? something : something_else 

## Recitation

0. 如何打包函数的返回值, 或者说, 返回多个值?

1. 我们可以类比C里实现返回多个值的我方法: 返回一个自己定义的结构体struct
    C++中也有类似的东西: 聚合类
    - 聚合类所有的成员都是public的
    - 聚合类没有自定义的构造函数, 初始化 === 一次性对所有成员进行初始化

    struct也可以是聚合类! 在C++中struct和class的区别很小
    判断一个类型是否为聚合类，要看它是否满足聚合类的定义规则，而不是看它用了 struct 还是 class 关键字。
```cpp
struct MinAndMax {
    int min;
    int max;
};

MinAndMax getMinMax(/* ... */) {
    int min = INT_MAX, max = INT_MIN;
    // ...
    return {min, max};
}
```
2. 如你所见, 我们需要使用大括号来初始化一个聚合类, 按照大括号中的顺序有序地初始化聚合类的第一个到最后一个元素
    但是如果MinAndMax有一个用户提供的构造函数, 那么也可以用这个构造函数来实现大括号初始化
    ```cpp
    struct MinAndMax {
        int min;
        int max;
        // 有序初始化
        MinAndMax(int min_, int max_) : min{min_}, max{max_} {}
    };
    ```

旗帜鲜明地反对以下这种写法: 直接访问成员变量!
    ```cpp
    MinAndMax result;
    result.min = a;
    result.max = b;
    ```

3. 使用结构化绑定! 类似python的, 更加现代方便的语法
    可以绑定不同类型的变量(需要auto), 完全可以保留变量的constness, 引用,
    变量类型也很自由, 可以是类对象, 可以是enum, 也可以是基本数据类型
    ```cpp
    struct MinAndMax { int min, max; };
    MinAndMax getMinMax(const std::vector<int> &);

    auto [minVal, maxVal] = getMinMax(numbers);
    std::cout << minVal << ' ' << maxVal << '\n';
    ```

    ```cpp
    enum class Gender { Male, Female };
    struct PersonInfo {
        std::string name;
        Gender gender;
        int birthYear;
    };
    void foo(const std::vector<PersonInfo> &persons) {
        // 在for循环中使用结构化绑定: 简直是Python!
        for (const auto &[n, g, by] : persons) {
        // ...
        }
    }
    ```
4.  ⼀个快速、随意的数据结构: std::pair 如果你需要返回两个值, 那就用它吧
    pair中的变量类型不一定要一致! 你在构造pair时不需要指明变量类型, C++17有CTAD类型推导
    ```cpp
    std::pair p3{a, b}; // 编译器根据 a 和 b 的类型来推导模板参数
                        // 这⾥⽤圆括号也可以，但 {} 更 modern

    std::pair<int, std::string> foo() {
        // ...
        return {42, "hello"};
    }
    ```
    访问pair中的元素: 使用.first和.second成员变量
    ```cpp
    auto p = foo();
    std::cout << foo.first << ", " << foo.second << std::endl;

    // 也可以在pair上使用结构化绑定
    auto [ival, sval] = foo();
    std::cout << ival << ", " << sval << std::endl;
    ```

5. 重载函数调用运算符operator()
```cpp
struct Adder {
    int operator()(int a, int b) const {
        return a + b;
    }
};
std::cout << Adder{}(2, 3) << std::endl; // 5
// Adder{}(2, 3) 相当于 Adder{}.operator()(2, 3)
// ⼀般地， f(arg1, arg2, ...) 被视为 f.operator()(arg1, arg2, ...)
// 这里有两对括号! 第一对括号是函数名operator()里的内容! 第二对括号才是参数列表!
```

6. 避免滥用类型转换运算符! 观察以下的代码, 请问输入的是字符串还是整数? 无法判断!
```cpp
struct Rational {
int n, d;
    operator double() const {
        return static_cast<double>(n) / d;
    }
    operator std::string() const {
        return std::to_string(n) + '/' + std::to_string(d);
    }
};
Rational r{3, 2};
std::cout << r << std::endl; // 1.5 还是 3/2 ？
```
7. 很有意思的案例: 到底要不要类型转换? 情况彻底乱套了!
```cpp
struct Rational {
    operator double() const;
};
struct X {
    X(const Rational &);
};

// foo被重载为可接受两种类型的参数
void foo(int);
void foo(X);
Rational r{3, 2};
foo(r); // 到底是先转成 double 然后转成 int 匹配 foo(int)
        // 还是直接转成 X 匹配 foo(X)？
```