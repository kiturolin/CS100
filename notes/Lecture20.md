## 迭代器 Iterators
0. iterator可以被理解为一个通用的"指针", 容器内的每一个对象都有自己的iterator

1. C++中每个内置的容器Container都有一个迭代器类型Contaioner::iterator
    比如std::vector<int>::iterator , std::forward_list<std::string>::iterator
    这么长这么丑的类型名, 一般我们用auto

2. 对于任意容器c, c.begin()返回容器内第一个对象的Iterator, 而c.end()返回容器内在__最后一个对象之后的对象__的Iterator
    一堆迭代器b, e是半开半闭区间! 指代[b,e)
    这样设计的好处有很多, 迭代范围就是e - b无需加减一
    如果b == e, 那么迭代范围就是空的了, 对于empty test很方便

3. 迭代器的基本操作
    - *it返回it指向对象的引用
        但*v.end()是未定义行为!
    - it->mem相当于(*it).mem
    - 支持it++与++it

4. 使用迭代器的例子
    ```cpp
    void swapcase(std::string &str) {
        for (auto it = str.begin(); it != str.end(); ++it) {
            if (std::islower(*it))
                *it = std::toupper(*it);
            else if (std::isupper(*it))
                *it = std::tolower(*it);
        } 
    }
    void print(const std::vector<int> &vec) {
        for (auto it = vec.begin(); it != vec.end(); ++it)
            std::cout << *it << ' ';
    }
    ```
5. 对于数组, 它也有迭代器类型! 
    比如Type a[N], begin iterator就是a, off-the-end iterator就是a + N
    对于std::begin(c)和std::end(c)两个库函数
    如果c是容器, 则返回c.begin()与c.end()
    如果c是数组, 则返回c与c+N, N为数组长度

6. 一个iterator是dereferenceable的当且仅当它指向的对象存在
    因此*c.end()是未定义行为
    ++it当it为end时也是未定义行为

7. 修改容器内容的操作很可能让所有之前取得的迭代器失效(Invalidation)!
    在此时继续使用之前的迭代器就是未定义行为
    比如vector当多次使用push_back之后可能会触发动态的内存申请与移动, 这意味着之前的iterator全都指向了无效的区域!
    ```cpp
    void foo(std::vector<int> &vec) {
        auto it = vec.begin();
        while (some_condition(vec))
        vec.push_back(*it++); // Undefined behavior.
    }
    ```
    永远不要使用失效的迭代器!

8. Iterators也分很多种类:
 - FowardIterator: 就是上面介绍过的iterator, 支持++it, it++, *it, it1 == it2等操作
 - BidirectionalIterator: 在上面的基础上还支持it--, --it
 - RandomAccessIterator: 支持it + N, it1 + it2, it1 - it2, it += n
                            还支持下标运算it[n]表示*(it + n)
                            还支持it之间的比大小: < > = >=等

9. 从Iterator range定义的初始化:
    ```cpp
    std::vector<char> v = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    std::vector v2(v.begin() + 2, v.end() - 3); // {'c', 'd', 'e', 'f'}
    std::string s(v.begin(), v.end()); // "abcdefghi"
    ```
相当于利用一个已有的iterator, 构造新的std::vector对象和std::string对象

## Algorithms

1. STL Algorithm库支持一iterator对的形式作为输入:
    ```cpp
    int a[N], b[N]; std::vector<int> v;
    std::sort(a, a + N);
    std::sort(v.begin(), v.end());
    std::copy(a, a + N, b); // copies elements in [a, a+N) to [b, b+N)
    std::sort(v.begin(), v.begin() + 10); // Only the first 10 elements are sorted.
    ```
2. 以_n结尾的algorithms接受的参数为一个begin iterator和一个正整数n, 表示遍历begin + n
    正常不带_n结尾的algorithm接受的参数是两个iterator! 分别是begin与end

    使用algorithm重写Dynarray类
    ```cpp
    Dynarray::Dynarray(const int *begin, const int *end)
    : m_storage{new int[end - begin]}, m_length(end - begin) {
        std::copy(begin, end, m_storage);
    }
    Dynarray::Dynarray(const Dynarray &other)
    : m_storage{new int[other.size()]}, m_length{other.size()} {
        std::copy_n(other.m_storage, other.size(), m_storage);
    } 
    Dynarray::Dynarray(std::size_t n, int x = 0)
    : m_storage{new int[n]}, m_length{n} {
        std::fill_n(m_storage, m_length, x);
    }
    ```

3. Algorithms的返回值一般是一个iterator, 用于表示容器内的某个position
    "Not found" / "No such element"一般是返回c.end()这个iterator
    ```cpp
    std::vector<int> v = someValues();
    auto pos = std::find(v.begin(), v.end(), 42);
    assert(*pos == 42);
    auto maxPos = std::max_element(v.begin(), v.end());

    // pos和maxPos都是Iterataor
    ```

4. 如果你在if中检查的临时变量在if-body内仍然要用, 这很常见
    你可以试试看C++17添加的if新语法: if(init_expr; condition)
    ```cpp
    if (auto pos = std::find(v.begin(), v.end(), something); pos != v.end())
    std::cout << *pos << '\n';
    ```
    init_expr就像for循环的第一个子句
    而init_expr定义的变量的作用域就是整个if-body

5. Algorithm可能对输入参数和容器有一些要求:
    - 对与输入的Iterator类型有要求, 比如std::sort()要求必须输入RandomAccessIterator
    - 对于容器内存储的内容有一定要求

    需要进行容器元素比较的Algorithm一般都需要你实现类对象的operator<和operator==运算符(你需要重载!)
    注意! 任何用到比较的Algorithm都只需要用到operator<和operator==, 
    你不需要重载其它的operator>, 等等

6. Algorithm承诺绝对不会修改容器的长度, 从来不会主动删除, 插入新的元素, 只会修改/替换原有元素
    ```cpp
    std::vector<int> a = someValues();
    std::vector<int> b(a.size());
    std::vector<int> c{};
    std::copy(a.begin(), a.end(), b.begin()); // OK
    std::copy(a.begin(), a.end(), c.begin()); // Undefined behavior!
                                            // c的长度为0, std::copy不会修改c!
    ```

7. std::unique(begin, end)
    unique用于消除容器内重复的元素, 需要你实现operator==和operator<
    注意! unique对容器内的元素有要求: 必须已经被排序为升序排列
    但std的algorithm承诺不会修改容器长度, 那么怎么做到去除重复元素呢?
    答案是unique会将所有的重复元素移动到容器末尾, 并返回重复元素开始的pos iterator
    ```cpp
    std::vector v{1, 1, 2, 2, 2, 3, 5};
    auto pos = std::unique(v.begin(), v.end());
        // Now [v.begin(), pos) contains {1, 2, 3, 5}.
        // [pos, v.end()) has the values {1, 2, 2}, but the exact order is not known.
    v.erase(pos, v.end()); // Typical use with the container's `erase` operation
        // Now v becomes {1, 2, 3, 5}.
    ```
8. Predicate
    std的Algorithm支持在最后传入一个callable对象, 返回值为Bool, 用于进行比较或搜寻
    ```cpp
    bool less_than_10(int x) {
        return x < 10;
    }
    std::vector<int> v = someValues();
    auto pos = std::find_if(v.begin(), v.end(), less_than_10);
    ```
    也可以用predicate来实现遍历并打印容器中每个值
    ```cpp
    void print_int(int x) { std::cout << x << ' '; }
    std::for_each(v.begin(), v.end(), print_int);
    ```

9. 如果这个Predicate依赖某些运行时定义的参数, 怎么办? 按如下定义!
    ```cpp
    struct LessThan {
        int k_; // No constructor is needed, and k_ is public.

        //重载operator()使其成为callable对象, 让std::find_if能够调用
        bool operator()(int x) const { return x < k_; }
    };

                                                // 在此处用k初始化LessThan对象
    auto pos = std::find_if(v.begin(), v.end(), LessThan{k}); // {} instead of ()
    ```
    fo(arg1, arg2, ...) is equivalent to fo.operator()(arg1, arg2, ...) . Any
    number of arguments is allowed.

    再来一个例子:
    ```cpp
    struct AbsCmp {
        bool operator()(int a, int b) const {
            return std::abs(a) < std::abs(b);
        }
    };
    std::sort(v.begin(), v.end(), AbsCmp{});
    ```
10. Lambda匿名函数
    语法: [capture_list](params) -> return_type { function_body }
        capture_list即为function_body需要用到的, 运行时决定的某些常数
    eg.
    ```cpp
    std::sort(v.begin(), v.end(),
    [](int a, int b) -> bool { return std::abs(a) < std::abs(b); });

    std::sort(v.begin(), v.end(),
    [](int a, int b) -> bool { return a > b; });

    auto pos = std::find_if(v.begin(), v.end(),
    [k](int x) -> bool { return x < k; });
    ```

    有了CTAD, 我们也可以省略返回值类型, 编译器会帮我们自动推断
    非常方便快捷的定义函数!
    ```cpp
    int k = 42;
    auto f = [k](int x) -> bool { return x < k; };
    bool b1 = f(10); // true
    bool b2 = f(100); // false
    ```
