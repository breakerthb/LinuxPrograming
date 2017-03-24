# 进程关系

# 1. 终端登录

### Ref ：P228

# 2. 网路登录

### Ref ：P231

# 3. 进程组

## 3.1 进程组ID

一个或多个进程的集合。
同一个进程组中的各个进程接收来自同一终端的各种信号。每个进程组有一个唯一的进程组ID。

    #include <unistd.h>
    pid_t getpgrp(void);
    
得到当前进程的进程组ID。

    #include <unistd.h>
    pid_t getpgid(pid_t pid);

得到某个特定进程（pid）的进程组ID    

### 返回值

- 成功：返回进程组ID
- 失败：返回-1

getpgid(0) 等价于 getpgid()

## 3.2 进程组组长

每个进程组有一个组长进程。进程组ID等于进程组组长的PID。

    #include <unistd.h>
    int setpgid(pid_t pid, pid_t pgid);

加入一个现有进程组或者创建一个新进程组。
    
### Returns: 0 if OK, −1 on error
    
一个进程只能为他自己或子进程设置进程组ID。

# 4. 会话

一个或多个进程组的集合。

![](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/9-6.png)

图中，一个会话有三个进程组。

通常是由管道将几个进程编成一组。图中的分组可以这样建立：

    $ proc1 | proc2 &
    $ proc3 | proc4 | proc5

建立一个新会话：

    #include <unistd.h>
    pid_t setsid(void);
    
### 返回值

- 成功，返回进程组ID
- 失败，返回-1

如果调用此函数的进程不是进程组组长，则创建一个新会话：

- 该进程变成新会话的会话首进程。也是此时的唯一的进程。
- 该进程成为一个新进程组的组长进程。
- 该进程没有控制终端。

如果该进程已经是一个进程组组长，则函数出错。为了避免这种情况，通常fork后，让其父进程终止。

    #include <unistd.h>
    pid_t getsid(pid_t pid);
    
得到会话组首进程ID。

### 返回值

- 成功：返回会话首进程ID
- 失败：返回-1

# 5. 终端控制

会话和进程组的其他特性：

- 一个会话可以有一个控制终端。
- 建立与控制终端连接的会话首进程被称为控制进程。
- 一个会话中有一个前台进程组和一个或多个后台进程组。
- 中断键（Delete或Ctrl + C）会将中断信号发到前台进程组中的所有进程。
- 终端退出键（Ctrl + \）会将中断信号发到前台进程组中的所有进程。
- 网络断开是，终端会将挂断信号发送至控制进程（会话首进程）。

![9-7](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/9-7.png)

# 6. tcgetpgrp() tcsetpgrp() tcgetsid()

得到前台进程组ID

    #include <unistd.h>
    pid_t tcgetpgrp(int fd);
 
### 返回值

- 成功：返回前台进程组ID
- 失败：返回-1
 
设置前台进程组ID


    int tcsetpgrp(int fd, pid_t pgrpid);
    
识别会话首进程的进程组ID。

    #include <termios.h>
    pid_t tcgetsid(int fd);
    // Returns: session leader’s process group ID if OK, −1 on error
    
# 7. 作业控制

### Ref : P237

# 8. shell执行程序

### Ref : P240

# 9. 孤儿进程组

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/relation/orphan3.c>

当父进程运行结束后，子进程处在Ctrl + Z的状态。此时，子进程的ppid变成了1（init）

# 10. FreeBSD实现

### Ref : P246


