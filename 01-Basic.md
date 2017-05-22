# Linux 开发基础

# 1. 登录

# 2. 文件和目录

### Demo

列出一个目录中所有文件名。

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/intro/ls1.c>

# 3. 输入和输出

# 4. 程序和进程

# 5. 出错处理

## 5.1 错误信息

系统函数出错时，会返回一个负值，`errno`被赋值。

之前的定义：

    #include <erro.h>
    extern int errno;
    
为了满足多线程存取，引入了下面的定义：

    extern int *__errno_location(void);
    #define errno (*__errno_location()
    
`errno`规则如下：

- 如果没有出错，值不会被改写
- 任何函数不会将其设为0
- `errno.h`中定义的所有常量都不是0

# 5.2 打印错误信息

将`errno`映射为一个出错字符串。

```cpp
#include <string.h>
char* strerror(int errnum);
```

返回出错信息字符串。

在标准错误中产生一条错误信息：

```cpp
#include <stdio.h>
void perror(const char* msg);
```

首先输出msg字符串，之后输出“: ”,之后是errno值的出错信息。

### Demo

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/intro/testerror.c>

# 6. 用户标识

# 7. 信号

# 8. 时间值

# 9. 日志

    #include <syslog.h>
    
    void syslog(int priority, const char *message, arguments ...);

----
# Append

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


