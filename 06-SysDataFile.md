# 1. 口令文件

    /etc/passwd

    root:x:0:0:root:/root:/bin/bash
    squid:x:23:23::/var/spool/squid:/dev/null
    nobody:x:65534:65534:Nobody:/home:/bin/sh
    sar:x:205:105:Stephen Rago:/home/sar:/bin/bash

通过命令查看用户信息：

    $ finger -p sar
    Login: sar Name: Steve Rago
    Directory: /home/sar Shell: /bin/sh
    Office: SF 5-121, 555-1111 Home Phone: 555-2222
    On since Mon Jan 19 03:57 (EST) on ttyv0 (messages off)
    No Mail.

通过函数查看用户信息：

    #include <pwd.h>
    struct passwd *getpwuid(uid_t uid);
    struct passwd *getpwnam(const char *name);
    
如果要查看整个文件的记录项：

    #include <pwd.h>
    struct passwd *getpwent(void);
    //Returns: pointer if OK, NULL on error or end of file

    void setpwent(void);
    void endpwent(void);

# 2. 阴影口令

    #include <shadow.h>
    struct spwd *getspnam(const char *name);
    struct spwd *getspent(void);
    // Both return: pointer if OK, NULL on error
    void setspent(void);
    void endspent(void);

# 3. 组文件

    #include <grp.h>
    struct group *getgrgid(gid_t gid);
    struct group *getgrnam(const char *name);

    #include <grp.h>
    struct group *getgrent(void);
    // Returns: pointer if OK, NULL on error or end of file
    void setgrent(void);
    void endgrent(void);

# 4. 附属组ID

    #include <unistd.h>
    int getgroups(int gidsetsize, gid_t grouplist[]);
    // Returns: number of supplementary group IDs if OK, −1 on error
    #include <grp.h> /* on Linux */
    #include <unistd.h> /* on FreeBSD, Mac OS X, and Solaris */
    int setgroups(int ngroups, const gid_t grouplist[]);
    #include <grp.h> /* on Linux and Solaris */
    #include <unistd.h> /* on FreeBSD and Mac OS X */
    int initgroups(const char *username, gid_t basegid);
    // Both return: 0 if OK, −1 on error

# 5. 实现区别

### Ref : 148

# 6. 其他数据文件

一般情况，对每个数据文件至少有3个函数：

- get()
- set()
- end()

![6-6](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/6-6.png)

# 7. 登录账户记录

    struct utmp {
        char ut_line[8]; /* tty line: "ttyh0", "ttyd0", "ttyp0", ... */
        char ut_name[8]; /* login name */
        long ut_time; /* seconds since Epoch */
    };

# 8. 系统标识

    #include <sys/utsname.h>
    int uname(struct utsname *name);
    // Returns: non-negative value if OK, −1 on error

    struct utsname {
        char sysname[]; /* name of the operating system */
        char nodename[]; /* name of this node */
        char release[]; /* current release of operating system */
        char version[]; /* current version of this release */
        char machine[]; /* name of hardware type */
    };

历史上，有只返回主机名的接口：

    #include <unistd.h>
    int gethostname(char *name, int namelen);
    // Returns: 0 if OK, −1 on error

# 9. 时间和日期











## 1.2 文件和设备

比较重要的设备文件有三个：

- /dev/console

- /dev/tty

- /dev/null

空设备，所以无用的输出信息都可以重定向到这里




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