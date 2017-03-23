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

# 8. 更改用户ID和更改组ID

    #include <unistd.h>
    int setuid(uid_t uid);
    int setgid(gid_t gid);
    // Both return: 0 if OK, −1 on error
    
### Ref : P204

# 9. 解释器文件

    #! pathname [optional-argument]
    
如：

    #! /bin/sh

脚本文件就是一种解释器文件。

内核调用exec来实现解释器文件中的功能。

# 10. system()

[system 函数](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/system.md)

# 11. 进程会计

对进程进行计数。

### Ref : P215

# 12. 用户标识

    #include <unistd.h>
    char *getlogin(void);
    // Returns: pointer to string giving login name if OK, NULL on error

# 13. 进程调度

    #include <unistd.h>
    int nice(int incr);
    // Returns: new nice value − NZERO if OK, −1 on error

    #include <sys/resource.h>
    int getpriority(int which, id_t who);
    // Returns: nice value between −NZERO and NZERO−1 if OK, −1 on error

    #include <sys/resource.h>
    int setpriority(int which, id_t who, int value);
    // Returns: 0 if OK, −1 on error

# 14. 进程时间

    #include <sys/times.h>
    clock_t times(struct tms *buf );

    struct tms {
        clock_t tms_utime; /* user CPU time */
        clock_t tms_stime; /* system CPU time */
        clock_t tms_cutime; /* user CPU time, terminated children */
        clock_t tms_cstime; /* system CPU time, terminated children */
    };
    
### 返回值

- 成功，返回流逝的墙上时钟时间
- 失败，返回-1

### Demo 

对每个命令执行计时。

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/times1.c>
    

    $ ./times1 "sleep 5" "date" "man bash >/dev/null"

执行结果：

![](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/8-31.png)


    
