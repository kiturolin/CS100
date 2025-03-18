1. 运算符的优先级并不代表求值顺序的优先级!
    f() * g() + h();这个语句并不意味着f和g函数会先被求值! 求值顺序是未定的! 但计算顺序是由运算符确定的
2. 但有一类运算符能够决定求值顺序(&&和||, 以及三目运算符 ? :)

3. 与求值顺序有关的Undefined Behavior
    当一个表达式的求值顺序是未定的, 且该表达式的操作数有两个及以上修改或使用了同一个变量, 那么这就是一个未定义行为
    Q: 为什么第二个表达式也是未定义行为?
```c 
    i = ++i + i++;  // undefined behavior
    i = i++ + 1;    // undefined behavior
    printf("%d, %d\n", i, i++); // undefined behavior
```

4. a < b < c这个表达式是这样被求值的: (a < b) < c, 若a = 1, b = 8, c = 5, 那么这个表达式将会是true! 若c > 1 则表达式的值永远都是true

5. 逻辑运算符的优先级: ! > 比较运算符 > && > ||

6. 赋值运算符"="的返回值是运算符左边的值
    比如a = b = c其实是a = (b = c)而(b = c) == b故相当于
    a = b ,最后a, b的值都与c相同

7. 与do-while等价的while循环
```c
    do {
    // loop_body
    } while (condition);

    // Equals
    while (1) {
        // loop_body
        if (!condition)
        break;
    }
```

8. switch的case对应值应该是编译期可知的整数类型常量, 不可以为浮点值或变量!
```c 
int n; scanf("%d", &n);
int x = 42;
switch (value) {
    case 3.14: // Error: It must have an integer type.
       printf("It is pi.\n");
    case n:
     // Error: It must be a constant expression (known at compile-time)
        printf("It is equal to n.\n");
    case 42:
     // OK.
        printf("It is equal to 42.\n");
    case x:
     // Error: `x` is a variable, not treated as "constant expression".
        printf("It is equal to x.\n");
}
```

9. 函数名之后的一对括号()被称作"函数调用运算符", 这是一个运算符! 副作用是进行压栈和调用函数
    而函数名f自己是一个statement, 没有任何副作用, 就像5; 2+3; ; {} 一样

10. 每个类型不为void的函数内都应该有一条return语句, 如果没有, 那么使用这个函数的返回值是未定义行为!

11. 函数的定义与声明: 函数定义Definition本身就是一个声明Declaration, 因为其包含了所有声明包含的信息, 每个函数只能有一个Definition, 但可以有多个Declaration
    当函数被调用时, 其Declaration必须在当前Scope以及之前的Scope中可见

12. 你可以在函数中定义一个和全局变量同名的局部变量!
