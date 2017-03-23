
# 1. 进程介绍

进程是一个地址空间，这个地址空间中运行着一个或多个线程和这些线程所需要的资源。

在Linux编程中，多进程的实现比多线程更容易。

# 2. 进程查看

ref : [任务管理](https://www.zybuluo.com/breakerthb/note/429591)

# 3. 启动新进程












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
    
    
## 6.4 文件流错误

为了表明错误，许多stdio库函数会返回一个超出范围的值，比如空指针或EOF常数。此时，错误由外部变量errno指出：

    #include <erro.h>
    
    extern int errno;
    
这个值只有在函数失败时才有意义。

也可以通过检查文件流状态判断是否出错。

    #include <stdio.h>
    
    int ferror(FILE *stream);
    int feof(FILE *stream);
    void clearerr(FILE *stream);

## 6.5 文件流和文件描述符

    #include <stdio.h>
    
    int fileno(FILE *stream);
    FILE *fdopen(int fd, const char *mode);
    
# 7. 文件和目录的维护


# 9. 错误处理

## 9.1 strerror函数

把错误代码映射成一个字符串

    #incluce <string.h>
    
    char *strerror(int errnum);
    
## 9.2 perror函数

把错误代码映射成一个字符串，并输出到标输出。

    #include <stdio.h>
    
    void perror(const char *s);
    
# 10 /proc文件系统

这个目录中有很多特殊的文件用来对驱动程序和内核信息进行访问。

## 10.1 /proc/cpuinfo

CPU的详细信息

    $ cat /proc/cpuinfo
    
## 10.2 /proc/meminfo

内存使用情况

    $ cat /proc/meminfo
    
## 10.3 /proc/version

内核版本信息

    $ cat /proc/version
    
## 10.4 /proc/net/sockstat

网络套接字使用统计

    $ cat /proc/net/sockstat
    
## 10.5 /proc/sys/fs/file-max

同时打开文件总数

    $ cat /proc/sys/fs/file-max
    $ echo 80000 > /proc/sys/fs/file-max

# 11. 高级主题：fcntl和mmap

## 11.1 fcntl系统调用



## 11.2 mmap函数

内存映射函数，建立一段可以被多个程序读写的内存。

可以通过管理内存的方式读写文件。

    #include <sys/mman.h>
    
    void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
    
- addr

请求某个特定的内存地址。推荐使用0，系统会自动分配地址。这样执行效率最高。

- off

共享内存段访问文件的起始位置。

- fd
打开文件标识符。

- prot
设置内存访问权限


    PROT_READ   允许读该内存段
    PROT_WRITE  允许写该内存段
    PROT_EXEC   允许执行该内存段
    PROT_NONE   该内存段不能被访问
    
- flags
控制程序对该内存段的改变所造成的影响


    MAP_PRIVATE 内存段私有，对它的修改只对本进程有效
    MAP_SHARED  把对该内存的修改保存到磁盘文件中
    MAP_FIXED   该内存段必须位于addr指定的地址处
    
### msync函数

把对内存段的修改写回到映射文件中

    #include <sys/mman.h>
    
    int msync(void *addr, size_t len, int flags);
    
flags参数控制修改方式：

    MS_ASYNC        采用异步写方式
    MS_SYNC         采用同步写方式
    MS_INVALLDATE   从文件中读回数据
    
### munmap函数

释放内存段

    #include <sys/mman.h>
    
    int munmap(void *addr, size_t len);
    
### Demo 

通过mmap存取一个结构化数据文件。





# 4. 临时文件

    #include <stdio.h>
    
    char *tmpnam(char* s);
    
返回一个不与任何文件同名的有效文件名

    #include <stdio.h>
    
    FILE *tmpfile(void);
    
给文件命名的同时打开它。

### Demo：tmpam.c

# 5. 用户信息

    #include <sys/types.h>
    #include <unistd.h>
    
    uid_t getuid(void); // 返回UID
    char *getlogin(void); // 返回登录名
    
更有效的用户信息接口

    #include <sys/types.h>
    #include <pwd.h>
    
    struct passwd *getpwuid(uid_t uid);
    struct passwd *getpwnam(const char *name);
    
# 6. 主机信息

- gethostname
- uname
- gethostid

# 7. 日志

    #include <syslog.h>
    
    void syslog(int priority, const char *message, arguments ...);
    
# 8. 资源和限制

# 9. 时间和日期

Ref : <https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/Time.md>