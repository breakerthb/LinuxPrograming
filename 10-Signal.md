# 信号

# 1. 概念

信号是进程间传递信息的重要方式。

每个信号都有一个名字，以SIG开头。

信号名称在<signal.h>中定义:

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

某个信号出现时，可以让内核完成下面的动作：

- 忽略信号

除了SIGKILL和SIGSTOP都可以被忽略。

- 捕捉信号

- 执行默认动作

大多数信号的默认动作是终止进程。如图所示：

![10-1](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/10-1.png)

### Ref : P252

# 2. signal函数

    #include <signal.h>
    void (*signal(int signo, void (*func)(int)))(int);

### 返回值

- 成功，返回当前的信号处理配置
- 失败，返回SIG_ERR

### 参数

- signo

图10-1中的信号名。

- func

常量SIG_IGN（忽略此信号）、常量SIG_DFL（执行系统默认动作）或当接到此信号后要调用的函数地址。

*注意：* 这个函数已经过时，不建议继续使用。

## Demo 1

处理Ctrl + c信号。

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_signal.c>

第一次按下Ctrl+c，ouch捕获，设置为默认。第二次按下结束。

## Demo 2

捕捉两个用户自定义信号，并打印。

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/signals/sigusr.c>

运行结果：

![10-2](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/10-2.png)


### PS

- 程序启动时，所有信号都是默认相应状态
- fork时，子进程继承了父进程的处理方式

# 3. 不可靠的信号

不可靠是指信号可能会丢失。

# 4. 中断的系统调用

# 5. 可重入函数

### Ref : P262

# 6. SIGCLD语义

# 7. 可靠信号术语和语义

# 8. kill() raise()

- kill函数将信号发送给进程或进程组
- raise函数允许进程向自身发送信号


    #include <signal.h>
    
    int kill(pid_t pid, int sig);
    int raise(int sig);
    
### 返回

- 成功，返回0
- 失败，返回-1

调用

    raise(sig);
    
等价于：

    kill(getpid(), signo);
    
|||
|:-:|:-:|
|pid > 0|发送给进程pid|
|pid == 0|发送给进程组中的所有进程|
|pid < 0|发送给进程组id等于pid绝对值的进程|
|pid == −1|发送给有权限的所有进程（能给谁发就给谁发）|


# 9. alarm() pause()

alarm设置一个定时器，超时时会产生一个SIGALRM信号。

    #include <unistd.h>
    unsigned int alarm(unsigned int seconds);

### 返回值

0或者定时到产生信号之间的秒数。

    #include <unistd.h>
    int pause(void);
    
把程序挂起到收到一个信号为止。
    
### Demo 1

- fork一个子进程
- 安排好消息捕获后暂停
- 子进程sleep5秒后向父进程发送一个SIGALRM信号
- 父进程收到一个信号后继续运行

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_alarm.c>

### Demo 2

让自己休眠一段时间

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/signals/sleep1.c>

# 10. 信号集

表示多个信号的数据类型。


    #include <signal.h>

    int sigemptyset(sigset_t* set); // 初始化set指向的信号集，清除所有信号
    int sigfillset(sigset_t* set);  // 初始化set指向的信号集，使其包含所有信号
    int sigaddset(sigset_t* set, int signo);    // 添加一个信号
    int sigdelset(sigset_t* set, int signo);    // 删除一个信号
    
    int sigismember(const sigset_t *set, int signo);

## 实现方法

用一个32位的int表示31个信号，某个信号设置了为1，没设置为0。

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/signals/setops.c>

# 11. sigprocmask()

定义当前阻塞而不能递送给该进程的信号集。

    #include <signal.h>
    int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset);

### Ref : P275

# 12. sigpending()

    #include <signal.h>
    
    int sigpending(sigset_t *set);
    
通过set返回一个信号集，其中的各个信号是阻塞不能递送的。

### Ref : P276

# 13. sigaction()

检查或者修改与指定信号有关的处理动作。

    #include <signal.h>
    int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);

推荐使用的信号接口。用来替代signal函数。

### 参数

- signo

需要检查或修改的信号no

- act

要修改的动作

- oact

返回目前的关联动作

    struct sigaction {
        void (*sa_handler)(int);    /* addr of signal handler, or SIG_IGN, or SIG_DFL */
        sigset_t sa_mask;           /* additional signals to block */
        int sa_flags;               /* signal options, Figure 10.16 alternate handler */
        void (*sa_sigaction)(int, siginfo_t *, void *);
    };

### Demo 1

用sigaction实现signal

    #include "apue.h"
    /* Reliable version of signal(), using POSIX sigaction(). */
    Sigfunc *   signal(int signo, Sigfunc *func)
    {
        struct sigaction act, oact;
        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        if (signo == SIGALRM) 
        {
            #ifdef SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;
            #endif
        } 
        else 
        {
            act.sa_flags |= SA_RESTART;
        }
        if (sigaction(signo, &act, &oact) < 0)
            return(SIG_ERR);
        
        return(oact.sa_handler);
    }

### Demo 2

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_sigaction.c>

按下组合件Ctrl+C能够看到捕获信号，但程序仍然进行。按下Ctrl+\程序结束。

# 14. sigsetjmp() siglongjmp()

# 15. sigsuspend()

# 16. abort()

使程序异常终止。

    #include <stdlib.h>
    void abort(void);
    
此函数将SIGABRT信号发给调用进程。相当于：

    raise(SIGABRT);
    
### Demo

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/signals/abort.c>
    
    
# 17. system()的另外实现方式

[system()](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/system.md)

# 18. sleep() nanosleep() clock_nanosleep()

### Ref : P298

# 19. sigqueue()

让信号排队。

    #include <signal.h>
    int sigqueue(pid_t pid, int signo, const union sigval value)
    Returns: 0 if OK, −1 on error
    
# 20. 作业控制信号

    SIGCHLD Child process has stopped or terminated.
    SIGCONT Continue process, if stopped.
    SIGSTOP Stop signal (can’t be caught or ignored).
    SIGTSTP Interactive stop signal.
    SIGTTIN Read from controlling terminal by background process group member.
    SIGTTOU Write to controlling terminal by a background process group member.
    
# 21. 信号名和编号

### Ref : P303
    
