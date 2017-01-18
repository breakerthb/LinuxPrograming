# 1 : 让自己习惯C++

## 条款01 : 视C++为一个语言联邦

- C
- Object-Oriented C++
- Template C++
- STL

## 条款02 : 尽量以const, enum, inline替换 #define

define的宏名不参与编译，因此如果报错很难定位。

    #define ASPECT_RATIO 1.653
    #define ABC "Hello World"
    
改为

    const double AspectRatio = 1.653;
    const string ABC("Hello World");
    
    #define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))
    
改为

    template<typename T>
    inline void callWithMax(const T& a, const T& b)
    {
        f(a > b ? a : b);
    }
    
- 对于单纯的宏，最好以const对象或enums替换#define
- 对于行似函数的宏，最好改用inline函数替换#define

## 条款03 : 尽可能使用const

    char greeting[] = "Hello";
    char* p = greeting;
    const char* p = greeting;
    char* const p = greeting;
    const char* const p = greeting;
    
- const出现在*左边，被指物是常量


    const char* p = greeting;
    char const* p = greeting;

两者意义相同

- const出现在*右边，指针是常量

### const类型返回值

    const int fun();
    
返回值用const修饰的意义：

当我们需要写：

    if (fun() == 1)
    
可能会写成

    if (fun() = 1)
    
这是就能及时发现错误。

## 条款04 : 确定对象被使用前已先被初始化

- 为内置对象进行手工初始化，因为C++不保证初始化它们
- 构造函数最好使用成员初值列，而不要在构造函数体内赋值。初值列中的顺序应该是成员变量声明顺序。成员初始值列发生在构造函数内部赋值之前。

# 2. 构造/析构/赋值运算

## 条款05 : 了解C++默默编写并调用哪些函数

- 编译器可以暗自为class创建default构造函数、copy构造函数、copy assignment操作符，以及析构函数。

## 条款06 : 若不想使用编译器自动生成的函数，就该明确拒绝

    class HomeForSale{
        public:
            ...
        private:
            HomeForSale(const HomeForSale&);
            HomeForSale& operator=(const HomeForSale&);
    }
    
这样copy构造函数和copy assignment操作符将不可用。

- 不想使用自动生成的函数，就声明为private

## 条款07 : 为多态基类声明virtual析构函数

例如在工厂模式中。

    #include <iostream>

    using namespace std;
    
    class Base
    {
    public:
    	Base()
    	{
    		cout << "Base Init ..." << endl;
    	}
    
    	virtual ~Base()
    	{
    		cout << "Destroyed Base ..." << endl;
    	}
    };
    
    class Impl : public Base
    {
    public:
    	Impl()
    	{
    		cout << "Impl init ..." << endl;
    	}
    
    	~Impl()
    	{
    		cout << "Destroyed Impl" << endl;
    	}
    };
    
    Base* GetInstance()
    {
    	Impl* p = new Impl;
    
    	return p;
    }
    
    int main()
    {
    	Base* p = GetInstance();
    
    	delete p;
    
    	return 0;
    }
    
执行结果：

![](http://i.imgur.com/4AkH0TV.png)

如果去掉virtual ~Base()中的virtual,结果如下：

![](http://i.imgur.com/Yrd6qzH.png)


## 条款08 : 别让异常逃离析构函数

## 条款09 : 绝不在构造和析构过程中调用virtual函数

构造函数执行时，virtual函数还不是函数。

- 在构造函数中调用virtual函数从不下降至derived class。

## 条款10 : 令operator=返回一个reference to *this

    Widget& operator=(int rhs)
    {
        ...
        return *this;
    }
    
这样可以支持连锁赋值。

## 条款11 : 在operator= 中处理“自我赋值”

当用户写一个w=w时，不能报错

## 条款12 : 复制对象时勿忘其每一个成分

# 3. 资源管理

## 条款13 : 以对象管理资源

- 为防止内存泄露，请使用RAII对象，他们在构造函数中获得资源并在析构函数中释放资源
- 两个常用的RAII classes分别是tr1::shared_ptr和auto_ptr。

## 条款14 : 在资源管理类中小心copying行为

## 条款15 : 在资源管理类中提供对原始资源的访问

## 条款16 : 成对使用new和delete是要采取相同形式

- 如果在new中使用[]在delete中也使用[]
- 如果new中没有使用[]在delete中不要使用[]

## 条款17 : 以独立语句将newed对象置于智能指针

# 4. 设计与声明

## 条款18 : 让接口容易被正确使用，不易被误用

## 条款19 : 设计class犹如设计type

## 条款20 : 宁以pass-by-reference-to-const替换pass-by-value

    void print(const window& w);

## 条款21 : 必须返回对象时，别妄想返回其reference

    class Rational
    {
        
    }
    
    inline const Rational()
    {
        return Rational(a, b);
    }


## 条款22 : 将成员变量声明为private

## 条款23 : 宁以non-member、non-friend替换member函数

## 条款24 : 若所有参数皆需类型转换，请为此采用non-member函数

## 条款25 : 考虑写一个不抛异常的swap函数

- 当std::swap对你的类型效率不高时，提供一个swap成员函数，并确定这个函数不会抛异常

# 5. 实现

## 条款26 : 尽可能延后变量定义式出现的时间

## 条款27 : 尽量少做转型动作

## 条款28 : 避免返回handles指向对象内部成分

## 条款29 : 为“异常安全”而努力是值得的

## 条款30 : 透彻了解inlining的里里外外

## 条款31 : 将文件间的编译依存关系将至最低

# 6. 继承与面向对象设计

## 条款32 : 确定你的public继承塑模出is-a关系

is-a关系，父类能做的事子类都能做，反之不成立

## 条款33 : 避免遮掩继承而来的名称

子类的成员名称会遮掩父类的同名成员。要注意。

## 条款34 : 区分接口继承和实现继承


## 条款35 : 考虑virtual函数以外的其他选择

strategy设计模式。

## 条款36 : 绝不重载non-virtual函数

## 条款37 : 绝不重载缺省参数函数

## 条款38 : 通过复合塑模出has-a或“根据某物实现出”

一对多的关系可以用STL实现

## 条款39 : 明智而审慎地使用private继承

## 条款40 : 明智而审慎地使用多重继承

相当于Java中接口的使用

# 7. 模板与泛型编程

## 条款41 : 了解隐式接口和编译期多态

- class和template都支持接口和多态
- class接口是显式的，以函数签名为中心，多态是通过virtual函数发生在运行期
- template接口是隐士的，奠基于有效表达式，多态是通过template具现化发生在编译期

## 条款42 : 了解typename的双重意义

    template<class T> class Widget;
    template<typename T> class Widget;
    
两种方式完全相同。

## 条款43 : 学习处理模板化基类内的名称

## 条款44 : 将与参数无关的代码抽离templates

## 条款45 : 运用成员函数模板接受所有兼容类型

## 条款46 : 需要类型转换时请为模板定义非成员函数

## 条款47 : 请使用traits classes 表现类型信息

## 条款48 : 认识template元编程

# 8. 定制new和delete

## 条款49 : 了解new-handler的行为

## 条款50 : new/delete合理替换时机

改善性能，内存对齐，heap错误调试，收集heap信息。

## 条款51 : new/delete固守常规

new含有无限循环分配内存，无法分配调用new-handler，处理0字节和超额申请；

delete处理null指针和超额申请。

## 条款52 : 写了placement new就要写placement delete

placement new在已有的缓冲区内申请对象；

不要掩盖已有的版本。

# 9. 杂项

## 条款53 : 不要忽视警告

严肃对待警告信息；

不过度依赖警告信息。

## 条款54 : 熟悉TR1标准库

智能指针、Boost库。

## 条款55 : 熟悉Boost

# ref

<http://www.cnblogs.com/fanzhidongyzby/archive/2012/11/18/2775603.html>