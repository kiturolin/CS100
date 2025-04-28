1. C++是一门**多范式** 的编程语言, 而不仅仅是面向对象的! 支持更高阶的数据抽象
2. 你最好将C++视为一系列编程语言的联盟: C, OOP-C++, Template C++, The STL
3. C++引入了如下修改(相较于C)

   1. bool ture false现在内置, 为bool类型, 而不需要引入外部库
   2. || && ! 以及>= == <= > <等逻辑运算符的类型为bool, 而非C中的int
   3. 字符串字面量""hello""的类型为const char [N+1]二分char [N+1]
   4. 单个字符'a'的类型为char, 而非C中的int
   5. 危险的类型转换将不会被隐式类型转换, 而会变成报错
   6. const类型的变量会在编译期初始化, 你可以用他们当作数组的长度
   ```cpp
    const int maxn = 1000;
    int a[maxn]; // a normal array in C++, but VLA in C
   ```
   7. int func()这种类型的声明是被允许的(括号中无void), 说明这个函数不接受任何参数
4. C++中 main函数的声明一般是int main()而不是int main(void)
5. C++的iostream

   1. 使用 `<iostream>`而不是 <stdio.h> , 使用std::cin和std::cout代替scanf和printf
   2. cin 不需要用&x来取输入变量的地址, C++能够自动获知应该存储在哪里!
   3. cin也不需要显式的用格式化字符串(如%d\)指定输入哪种类型的数据, C++同样能够自动获知你输入的是什么类型的数据!
   4. 
   5. 需要注意! std::cin和std::cout不是函数! 它们是对象! 真正起到函数作用的是"">>""和"<<"运算符, 它们被重载(overloaded)了, 不再是移位运算的意思
   6. 非常方便且巧妙的特性: std::cin >> x的返回值类型是std::cin! 这意味着
      ```cpp
      std::cin >> x >> y >> z
      // equals
      ((std::cin >> x) >> y) >> z
      // or
      std::cin >> x;
      std::cin >> y;
      std::cin >> z;
      ```
6. 标准库

   1. C++中的标准库不再有扩展名.h! 都是诸如 `<iostream>`或者 `<string>`这样的名称
   2. 所有老式的C的库<xxx.h>会被重命名为<cxxx>, 比如<string.h>在C++中的对应位<cstring>
   3. 你可以写using std::cin来只对cin引入std命名空间, 这样你在以下的scope中只写cin即可, 不需要再写std::cin了

7. std::string
    1. 使用std::string来声明一个cpp字符串类型!
    ```cpp
    std::string str = "Hello world";
    // equivalent: std::string str("Hello world");
    // equivalent: std::string str{"Hello world"}; (modern)
    ```
    2. 如果在声明字符串时不使用字符串字面量进行初始化, 那么字符串将是空字符串! 没有任何的未定义值或者undefined behavior!
    3. 你可以用如下方式方便的定义字符串(语法糖)
    ```cpp
    std::string s1(7, 'a');
    std::cout << s1 << std::endl; // aaaaaaa
    std::string s; // "" (empty string)
    ``` 

    4. 存储std::string的内存会被自动的分配和释放, 同时, 修改字符串的长度导致的内存重新分配也会被自动完成, std::string不需要以'\0'结尾!它能自动识别字符串结尾!
        当你写std::string时, 你不要有太多心智负担, 只考虑内部的内容即可, 不需要关注它的底层实现
    5. 成员函数
        1. str.size(): 返回字符串的长度, 不要使用strlen()或者sizeof()!
        2. str.empty(): 返回字符串是否为空
    6. 字符串拼接: 直接使用 + 或 += ! 完全不需要担心内存重新分配的问题
    ```cpp
    std::string s1 = "Hello";
    std::string s2 = "world";
    std::string s3 = s1 + ' ' + s2; // "Hello world"
    s1 += s2; // s1 becomes "Helloworld"
    s2 += "C++string"; // s2 becomes "worldC++string"

    std::string hello{"hello"};
    std::string s = hello + "world" + "C++";
    // + is left-associated. (hello + "world") is of type std::string
    ```
    7. 你应该尽量使用+=, 在C++中, a = a + b不一定完全等价于 a += b, 在字符串操作中, a = a + b创建一个临时变量, 将a b的值分别拷贝到这块内存中, 再将其复制到a中, 
        这非常慢! 而使用 += ,是在原字符串末尾直接添加b, 更快
        ```cpp
        std::string result;
        for (int i = 0; i != n; ++i)
        result += 'a'; // Fast

        std::string result;
        for (int i = 0; i != n; ++i)
        result = result + 'a'; // Very slow
        ```
    8. 使用 "="等号能够直接复制字符串! s1 = s2说明我们将字符串s2的内容整个拷贝到了s1! 不需要strcpy!
        使用 "==", "<="等操作符直接比较字符串!(字典序), 不需要使用strcmp!
        需要注意的是, str1==str2的返回值类型为true or false! 而非strcmp奇奇怪怪的返回值

    9. range-based for loops
        类似Python的for循环结构, 比C Style的遍历字符串更加现代, 更加简洁, 推荐使用!
        ```cpp
        for (char c : s) // The range-based for loops
        if (std::isupper(c)) // in <cctype>
        std::cout << c;
        std::cout << std::endl;
        ```

    10. 将数字转换字符串: 使用std::to_string

    
