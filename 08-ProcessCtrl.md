# 进程控制

# 1. 概述

# 2. 进程标识

进程ID，唯一的非负数。

- ID : 0

调度进程 / 交换进程（swapper） / 系统进程

- ID : 1

init进程，负责在自举内核后启动一个UNIX系统。

- ID : 2

页守护进程。支持虚拟存储器系统的分页操作。

相关的函数操作。

    #include <unistd.h>
    pid_t getpid(void);     // 返回进程ID
    pid_t getppid(void);    // 父进程ID
    uid_t getuid(void);     // 实际用户ID
    uid_t geteuid(void);    // 有效用户ID
    gid_t getgid(void);     // 实际组ID
    gid_t getegid(void);    // 有效组ID

*注意：* 这些函数都没有错误返回。

# 3. fork() vfork()

[fork与vfork介绍](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/fork.md)

# 4. exit()

[进程终止的种类](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/exit.md)

## init收养

父进程在子进程之前终止，子进程的父进程会变为init(PID:1)，保证每个活着的进程都有父进程。

## 回收

子进程先结束，父进程可以通过wait()和waitpid()回收子进程资源。

## 僵尸进程

一个已经终止的子进程，如果父进程没有对其进行资源回收，这个子进程就成为僵尸进程。ps命令时，僵尸进程的状态是Z

init收养的进程不会成为僵尸进程，因为init会自动调用wait()进行回收。

# 5. wait() waitpid() waitid()

[回收子进程](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/wait.md)

# 6. 竞争条件

- 如果父进程需要等待子进程终止，需要调用wait()
- 如果子进程需要等待父进程终止，可以使用下列形式：


    while (getppid() != 1)
        sleep(1);

这样的轮询方法对CPU消耗过大，应该采用进程间通讯的方式：

    #include "apue.h"
    TELL_WAIT(); /* set things up for TELL_xxx & WAIT_xxx */
    if ((pid = fork()) < 0) 
    {
        err_sys("fork error");
    } 
    else if (pid == 0) /* child */ 
    {
        // Do something
        TELL_PARENT(getppid()); // tell parent we’re done 
        WAIT_PARENT(); // wait for parent
        
        // Do another something
        
        exit(0);
    }

    // Parent do something
    TELL_CHILD(pid); // tell child we’re done
    WAIT_CHILD(); // and wait for child

    // Parent do another something

    exit(0);
    
相关宏定义：
    
    void	TELL_WAIT(void);
    void	TELL_PARENT(pid_t);
    void	TELL_CHILD(pid_t);
    void	WAIT_PARENT(void);
    void	WAIT_CHILD(void);
    
函数内容：<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/lib/tellwait.c>


### Demo

父进程和子进程各输出一个字符串。由于输出依赖于内核，因此存在一个竞争条件。

没有进程间通信：

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/tellwait1.c>

输出冲突产生乱码。

有进程间通信：

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/tellwait2.c>

不产生冲突。

# 7. exec()

[exec函数](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/exec.md)

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
    
