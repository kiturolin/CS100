1. 注意! 一元(unary)运算符"+/-"和二元运算符"+/-"是两个完全不同的运算符
    运算符优先级: [一元+/-] > [*, /, %] > [二元+/-]

2. 对于二元的加减乘除运算符, 最后所有的操作数都会被转换成相同的类型
    对于浮点和整形做运算, 最后的答案是浮点型
    对于int型和long long型的运算, 两个操作数都会被转换为long long型, 结果也是long long型

3. 对于除法/ ,当两个操作数都是整形时, 其结果是整形, 当其中一个操作数为浮点型, 其结果应该为浮点型
    当两个整形进行除法运算时, 其运算结果应该会向0取整, 比如3/2应该为1, 而3/-2应该为-1 (Verified!)

4. 如何让两个整数实现得到浮点数的除法? 使用浮点型字面量1.0或0.0, 比如(a + 0.0) / b或1.0 * a / b 

5. 对于取模运算, 两个操作数必须是整数, 但两个操作数的正负性可能导致让人困惑的结果, 但我们只需要记住:
    (a / b) * b + (a % b) == a 

6. 整形溢出:
```c
    int ival = 100000; long long llval = ival;
    int result1 = ival * ival;          // (1) overflow
    long long result2 = ival * ival;    // (2) overflow
    long long result3 = llval * ival;   // (3) not overflow
    long long result4 = llval * ival * ival;    // (4) not overflow
```
    在上面的例子中, 你要尤其注意result2也会溢出!!!!因为ival * ival的类型是int而不是long long !

7. 有符号整形溢出是一种Undefinded Behavior! 你不能对其有任何期待, 它的运行结果是完全无法预知的

8. 无符号整数从不会溢出! 无符号整数的实际存贮值为n mod 2^k, 比如
```c 
uint32_t a = UINT32_MAX; // now a == 2^32 - 1
a += 1;
// now a == 0 because a + 1 = 2^32 while 2^32 mod 2^32 === 0
```

9. if之后的语句可以是单一的一条语句(one single statement)或者用大括号包裹的block(语句块{})

10. 悬垂else:
```c 
if (i == 1)
    if (j == 2)
        printf("aaa\n");
else // Incorrectly indented! This `else` is associated with `if (j == 2)`.
    printf("bbb\n");
``` 

11. 一个很妙的写法: 当i < sqrt(n)时运行循环:
```c 
for (int i = 0; i * i <= n; ++i) // We don't need sqrt.
// ...
```

12. 现代的方法是在for循环的括号内定义循环变量i, j! 
```c 
int main(void) {
// ...
for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
// ...
}
```

13. 重要!!!当参与运算的整数的类型长度小于int时, integer promotion会被首先应用! 将所有长度小于int 的整数类型转换为int, 比如signed char + long会先转换为int + long在转换为long + long 最后的结果为long

需要注意的是, integer promotion对于一元运算符+/-也是起作用的

14.  
