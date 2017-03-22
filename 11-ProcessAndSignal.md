
# 1. 进程介绍

进程是一个地址空间，这个地址空间中运行着一个或多个线程和这些线程所需要的资源。

在Linux编程中，多进程的实现比多线程更容易。

# 2. 进程查看

ref : [任务管理](https://www.zybuluo.com/breakerthb/note/429591)

# 3. 启动新进程

## 3.1 system函数

    # include <stdlib.h>
    
    int system(const char* string);
    
函数作用：运行传递给它的命令，命令执行情况类似于：

    $ sh -c "command"
    
### 返回值

- 无法启动shell，返回127
- 其他错误，返回-1
- 执行成功，返回命令退出码

### Demo

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_system.c>

### PS

不建议使用system函数启动其他进程，因为它需要先启动一个新的shell，这对shell安装情况和环境的依赖很大。

函数的执行效率不高。

## 3.2 exec 函数

替换进程映像。把当前进程替换为一个新进程，新进程由path和file参数指定。可以通过exec从一个程序切换到另一个程序。

    #include <unistd.h>
    extern char **environ;
    
    int execl(const char *path, const char *arg, ...);
    int execlp(const char *file, const char *arg, ...);
    int execle(const char *path, const char *arg, ..., char * const envp[]);
    
    int execv(const char *path, char *const argv[]);
    int execvp(const char *file, char *const argv[]);
    int execve(const char *path, char *const argv[], char *const envp[]);
    
exec是一组函数，分为两大类：

前三个参数可变，后三个参数固定。

以字母p结尾的函数通过搜索PATH环境变量来查找新程序中可执行程序的位置。

全局变量environ可以把一个值传递传递到新程序的环境变量中。

### 返回值

一般情况，exec不会返回，除非出现错误。出错时，exec返回-1，并且设置错误变量errno。

### Demo

常见的六种方式：

    #include <unistd.h>
    
    char* const ps_argv[] = {"ps", "ax", NULL};
    
    char* const ps_envp[] = {"PATH=/bin:/usr/bin", "TERM=console", NULL};
    
    execl("/bin/ps", "ps", "ax", NULL);
    execlp("ps", "ps", "ax", NULL);
    execle("/bin/ps", "ps", "ax", NULL, ps_envp);
    
    execv("/bin/ps", ps_argv);
    execvp("ps", ps_argv);
    execve("/bin/ps", ps_argv, ps_envp);
    
ref:

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_execlp.c>

### 注意：

- 新进程替换旧进程

exec产生了一个新的程序，因此执行后只能看到新程序的输出，看不到之前程序中exec代码后面的执行情况。

- 前两个参数

前两个参数都是被调用的程序名称，不同的是第一个参数要表明路径，第二个参数只是程序名。

    execl("./abc", "abc", "param_1", "param_2", NULL);





## 3.6 输入输出重定向

通过exec调用实现一个过滤程序。

- demo_upper.c

实现一个把小写字母转换成大写字母的过滤程序

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_upper.c>

执行：

    $ ./run_upper
    hello ABC
    HELLO ABC
    ^C

或

    $ ./run_upper < file.txt

通过另一个程序调用。

- demo_useupper.c

通过exec来调用run_upper程序实现参数过滤。

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_useupper.c>

执行：

    $ ./run_useupper file.txt
    
- demo_multi_useupper.c

通过子进程来同时完成多个exec调用。

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_multi_useupper.c>

执行：

    $ ./run_multi_useupper file.txt file.txt

# 4. 信号

## 4.1 介绍

信号是进程间传递信息的重要方式。

信号名称在signal.h中定义：

||||
|:-:|:-:|:-:|
|01|SIGHUP|挂起（hangup）|
|02|SIGINT|中断，当用户从键盘按Ctrl+C键或^break键时|
|03|SIGQUIT|退出，当用户从键盘按Ctrl+\键时|
|04|SIGILL|非法指令|
|05|SIGTRAP|跟踪陷阱（trace trap），启动进程，跟踪代码的执行|
|06|SIGIOT|IOT指令|
|07|SIGEMT|EMT指令|
|08|SIGFPE|浮点运算溢出|
|09|SIGKILL|杀死、终止进程|
|10|SIGBUS|总线错误|
|11|SIGSEGV|段违例（segmentation  violation），进程试图去访问其虚地址空间以外的位置|
|12|SIGSYS|系统调用中参数错，如系统调用号非法|
|13|SIGPIPE|向某个非读管道中写入数据|
|14|SIGALRM|闹钟。当某进程希望在某时间后接收信号时发此信号|
|15|SIGTERM|软件终止（software  termination）|
|16|SIGUSR1|用户自定义信号1|
|17|SIGUSR2|用户自定义信号2|
|18|SIGCLD|某个子进程死|
|19|SIGPWR|电源故障| 
|20|SIGSTP|Ctrl + z|

## 4.2 signal函数

    #include <signal.h>
    void (*signal(int sig, void (*func)(int)))(int);
    
通过它处理信号。

这个函数已经过时，不建议继续使用。

### Demo

处理Ctrl + c信号。

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_signal.c>

第一次按下Ctrl+c，ouch捕获，设置为默认。第二次按下结束。

## 4.3 发送信号

### kill

    #incluld <sys/types.h>
    #include <signal.h>
    
    int kill(pid_t pid, int sig);
    
把信号sig发给进程pid。

### alarm

    #include <unistd.h>
    
    unsigned int alarm(unsigned int seconds);

安排在seconds之后安排一个SIGALRM信号。

### pause

    #include <unistd.h>
    
    int pause(void);
    
把程序挂起到收到一个信号为止。
    
### Demo

- fork一个子进程
- 安排好消息捕获后暂停
- 子进程sleep5秒后向父进程发送一个SIGALRM信号
- 父进程收到一个信号后继续运行

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_alarm.c>

## 4.4 sigaction函数

    #include <signal.h>
    
    int sigaction(int sig, const struct sigaction* act, struct sigaction* oact);
    
推荐使用的信号接口。用来替代signal函数。

<https://github.com/breakerthb/LinuxPrograming/tree/master/11_ProceeAndSignal/demo_sigaction.c>

按下组合件Ctrl+C能够看到捕获信号，但程序仍然进行。按下Ctrl+\程序结束。

## 4.5 信号集

    #include <signal.h>
    
    int sigaddset(sigset_t* set, int signo);
    int sigemptyset(sigset_t* set);
    int sigfillset(sigset_t* set);
    int sigdelset(sigset_t* set, int signo);