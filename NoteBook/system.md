# system函数

# 1. 定义

    # include <stdlib.h>
    
    int system(const char* string);
    
函数作用：运行传递给它的命令，命令执行情况类似于：

    $ sh -c "command"
    
### 返回值

- 无法启动shell，返回127
- 其他错误，返回-1
- 执行成功，返回命令退出码

# 2. 原理

system在其实现中调用了fork、exec和waitpid,因此有三种返回值：

- 返回-1

fork失败或者waitpid返回出EINTR之外的错误，设置errno

- exit(127)

exec执行失败(不能执行shell)

- 成功

fork exec waitpid 都执行成功。

# 3. 实现方法

## 3.1 方法一

system的一种实现方法(没有进程通信)：

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/system.c>

测试这个实现方式：

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/proc/systest1.c>

注意：

在执行system时，要阻止system函数获得子进程的终止状态。使用如下：

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/signals/systest2.c>

## 3.2 方法二

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP/signals/system.c>

这个实现中加入了所有需要的信号处理。

# 4.system优势

在执行fork和exec的过程中，system进行了各种出错处理和信号处理。

不过，因为它需要先启动一个新的shell，这对shell安装情况和环境的依赖很大。执行效率也不高。

# 5. 实例

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/11_ProceeAndSignal/demo_system.c>





