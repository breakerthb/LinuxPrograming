# fork() vfork()

# 1. fork()

## 1.1. 定义

创建一个新进程，这个进程的配置完全复制父进程,它和父进程是同一份代码。

fork与exec需要结合使用，fork之后，两个进程共享同一套资源，当任意一个进程修改了区域之后，那块区域会创建一个副本。

    #include <unistd.h>
    pid_t fork(void);

### 返回值

- 子进程返回0
- 父进程返回子进程ID
- 出错，返回-1


    pid_t new_pid = fork();
    switch(new_pid)
    {
    case -1: // Error
        break;
    case 0: // child
        // 这里的new_pid是0
        break;
    default: // parent
        // 这里的new_pid是子进程的pid
        break;
    }

### Demo 1

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_fork1.c>

由于主进程提前结束，因此输出较乱。

### Demo 2

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/proc/fork1.c>

父进程与子进程的变量相互独立。

    $ ./fork1
    $ ./fork1 > tmp.out
    
两种方式输出内容相同，这说明父进程与子进程共享同一个stdout的偏移量。

## 1.2. 文件描述符

fork之后，处理文件描述符有两种常见情况：

- 父进程等待子进程完成

这种情况下无需其他处理，因为只有子进程在进行读写操作。

- 父进程和子进程各自执行不同的程序段

这种情况下，父进程和子进程各自需要关闭它们不需要使用的文件描述符，这样就不会干扰对方的文件描述符。

网络服务中常用方法。

## 1.3. fork失败原因

- 系统中有太多进程
- 实际用户ID的进程总数超过了系统限制

## 1.4. fork的两种方法

- 父进程希望复制自己

父进程和子进程同时执行不同的代码段。

网络服务进程中常用，父进程等待客户端请求，当请求到达时fork子进程去完成服务，父进程继续等待下一个请求。

- 一个进程要执行一个不同的程序

子进程从fork返回后立即调用exec通过一个新的shell执行。

# 2. vfork()

与fork操作相同，功能不同。

vfork创建一个新进程，新进程的目的是exec一个新程序。

与fork的区别：

- 子进程并不复制父进程的地址空间，因为新进程的目的是exec一个新程序。
- vfork对exec进行了优化，提高了效率。
- vfork保证子进程先行，在子进程调用exec或exit后父进程才被调用运行。（相当于顺序运行 子进程 -> 父进程）

### Demo

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/proc/vfork1.c>

说明：

- 子进程在父进程的地址空间中运行，因此公用变量
- 使用_exit不冲洗缓冲区