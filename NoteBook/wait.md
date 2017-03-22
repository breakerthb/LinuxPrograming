# wait() 和 waitpid()

# 1 wait函数

主进程可以通过wait等待子进程结束。

    #include <sys/types.h>
    #include <syst/wait.h>
    
    pid_t wait(int* stat_loc);
    
返回子进PID。

如果stat_loc不是空指针，状态信息将会写在它指向的位置。解释状态信息宏：

|||
|:-:|:-:|
|WIFEXITED(stat_val)|如果子进程正常结束，它就取一个非零值|
|WEXITSTATUS(stat_val)|如果WIFEXITED非零，它返回子进程的退出码|
|WIFSIGNALED(stat_val)|如果子进程因为一个未捕获的信号而终止|
|WTERMSIG(stat_val)|如果WIFSIGNALED非零，它返回一个信号代码|
|WIFSTOPPED(stat_val)|如果子进程意外终止，它就取一个非零值|
|WSTOPSIG(stat_val)|如果WIFSTOPPED非零，它返回一个信号代码|

### Demo

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_wait.c>

### 子进程销毁条件

- 父进程正常结束
- 父进程通过wait得到子进程结束状态

否则会产生僵尸进程。

# 2 waitpid函数

等待某个特定进程。

    #include <sys/types.h>
    #include <sys/wait.h>
    
    pid_t waitpid(pid_t pid, int *stat_loc, int options);
    
- pid

|||
|:-:|:-:|
|pid == -1|等待任一子进程。等效于wait()|
|pid > 0|等待进程pid的子进程|
|pid == 0|等待组ID等于调用进程组ID的任一子进程|
|pid < -1|等待组ID等于pid绝对值的任一子进程|

- stat_loc

如果不为空，得到返回信息状态

- options

用来改变waitpid的行为。最有用的是WNOHANG，防止waitpid调用将调用者的执行挂起。
如果需要周期性的检查，则用下面语句：

    waitpid(child_pid, (int*)0, WNOHANG);

这样不会挂起，如果子进程没有结束或意外终止，返回0，否则返回child_pid。如果失败，返回-1.

# 3. 调用后的情况

调用这两个函数之后可能发生：

- 如果所有子进程都还在运行，则阻塞
- 如果一个子进程已终止，正等待父进程获取状态，则立即返回
- 如果没有子进程，返回出错。

# 4. 两个方法的区别

- wait使用阻塞等待，waitpid有一个选项可以使调用者不阻塞
- wait等待第一个子进程，waitpid等待一个特定的进程
- waitpid通过WUNTRACED和WCONTINUED选项支持作业控制

## Demo

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/wait1.c>

如果一个进程fork一个子进程，但不要等待子进程终止，也不希望进程处在僵尸状态直到父进程终止的方法是，fork两次。

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/fork2.c>

# 5. waitid()

    #include <sys/wait.h>
    int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
    Returns: 0 if OK, −1 on error

类似于waitpid，但提供了更多灵活性。

ref : P194

# 6. wait3() wait4()


