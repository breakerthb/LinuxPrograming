# 进程终止

进程终止8种方法

## 正常终止：

- 从main返回
- 调用exit
- 调用_exit或_Exit
- 最后一个线程从其启动例程返回
- 从最后一个线程调用pthread_exit

## 异常终止：

- 调用abort
- 接到一个信号
- 最后一个线程对取消请求做出响应

# 1. main函数返回

main函数内调用return语句返回。相当于执行exit。

# 2. 调用exit()

## 2.1 定义

    #include <stdlib.h>
    void exit(int status);

这个函数先执行一些清理工作（调用atexit），之后进入内核。

status为对外返回值。

    exit(0);
    
等价于：

    return(0);
    
## 2.2 atexit()

一个进程可以登记多至32个函数，这些函数将由exit自动调用。登记方法：

    #include <stdlib.h>
    int atexit(void (*func)(void));
    
返回值：

- 成功：0
- 失败：非0

exit调用这些函数的顺序和atexit注册的顺序相反。同一个函数如果注册多次则被调用多次。

![7-2](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/7-2.png)

C程序的启动和终止。

### Demo 

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/environ/doatexit.c>

# 3. 调用_exit() _Exit()

    #include <stdlib.h>
    void _Exit(int status);

    #include <unistd.h>
    void _exit(int status);
    
这两个函数立即进入内核。

# 4. 最后一个线程在其启动例程中执行return语句（ref：11.5）

该线程的返回值不用作进程返回值，该进程返回0。

# 5. 最后一个线程调用pthread_exit() （ref：11.5）

这种情况进程终止状态总是0。

# 6. 调用abort()

它可以产生SIGABRT信号，这是下一种异常终止的特例。

# 7. 进程收到某信号

ref : chap 10

# 8. 最后一个线程对“取消”请求做出响应


*PS: *无论进程如何终止，最后都会执行内核中的同一代码。