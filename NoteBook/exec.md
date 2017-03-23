# exec 函数

# 1. 定义

替换进程映像。把当前进程替换为一个新进程，进程ID不变。

新进程由path和file参数指定。可以通过exec从一个程序切换到另一个程序。

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

# 2. 区别

![](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/8-14.png)

# 3. 关系

![](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/8-15.png)

# 4. 实例

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
    
### Demo 1:

<https://github.com/breakerthb/LinuxPrograming/tree/master/LP/11_ProceeAndSignal/demo_execlp.c>

### 注意：

- 新进程替换旧进程

exec产生了一个新的程序，因此执行后只能看到新程序的输出，看不到之前程序中exec代码后面的执行情况。

- 前两个参数

前两个参数都是被调用的程序名称，不同的是第一个参数要表明路径，第二个参数只是程序名。

    execl("./abc", "abc", "param_1", "param_2", NULL);
    
### Demo 2:

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/exec1.c>

执行前把当前路径加入到PATH中：

    $ export PATH=.:$PATH

打印所有参数和环境变量：
<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/echoall.c>

### Demo 3:

通过exec调用实现一个过滤程序。

- demo_upper.c

实现一个把小写字母转换成大写字母的过滤程序

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_upper.c>

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

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_useupper.c>

执行：

    $ ./run_useupper file.txt
    
- demo_multi_useupper.c

通过子进程来同时完成多个exec调用。

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_multi_useupper.c>

执行：

    $ ./run_multi_useupper file.txt file.txt