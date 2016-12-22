# 文件操作

在Linux中，一切皆文件。在大多数情况下，都能用5个基本函数：

- open
- close
- read
- write
- ioctl

# 1. Linux文件结构

## 1.1 目录

一种特殊的文件

## 1.2 文件和设备

比较重要的设备文件有三个：

- /dev/console

- /dev/tty

- /dev/null

空设备，所以无用的输出信息都可以重定向到这里

# 2.系统调用和设备驱动程序

访问驱动程序的底层函数

- open  打开文件或设备
- close 关闭文件或设备
- read  从打开的文件或设备中读数据
- write 向文件或设备写数据
- ioctl 把控制信息传递给设备驱动程序

# 3. 库函数

底层函数开销较大，因此要减少使用次数。于是就要是用库函数。

# 4. 底层文件访问

当一个程序开始运行时，一般会有3个已经打开的文件描述符：

- 0: 标准输入
- 1: 标准输出
- 2: 标准错误

## 4.1 write系统调用

    #include <unistd.h>
    
    size_t write(int fildes, const void *buf, size_t nbytes);
    
## 4.2 read系统调用

    #include <unistd.h>
    
    size_t read(int fildes, void *buf, size_t nbytes);
    
## 4.3 open系统调用

创建一个新的文件描述符

    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    
    int open(const char *path, int oflags);
    int open(const char *path, int oflags, mode_t mode);
    
- O_RDONLY  以只读方式打开
- O_WRONLY  以只写方式打开
- O_RDWR    以读写方式打开

可选模式组合：

- O_APPEND  把写入数据追加在文件的末尾
- O_TRUNC   把文件长度设置为零，丢弃已有内容
- O_CREAT   如果需要，就按照参数mode中给出的访问模式创建文件
- O_EXCL    与O_CREAT一起使用，确保调用者创建出文件。

### 访问权限的初始值

## 4.4 close系统调用

    #include <unistd.h>
    
    int close(int fildes);
    
## 4.5 ioctl系统调用

    #include <unistd.h>
    
    int ioctl(int fildes, int cmd, ...);
    
## 4.6 其他文件管理相关的系统调用

### 4.6.1 lseek系统调用

对文件描述符fildes的读写指针进行设置。也就是当前读写位置

    #include <unistd.h>
    #include <sys/types.h>
    
    off_t lseek(int fildes, off_t offset, int whence);
    
### 4.6.2 fstat,stat和lstat系统调用

返回与打开文件描述符相关的文件状态信息。

    #include <unistd.h>
    #include <sys/stat.h>
    #incluce <sys/types.h>
    
    int fstat(int fildes, struct stat *buf);
    int stat(const char *path, struct stat *buf);
    int lstat(const char *path, struct stat *buf);
    
### 4.6.3 dup和dup2系统调用

dup提供了一种复制文件描述符的方法，使我们能供通过两个或更多不同的文件描述符来访问同一个文件。

    #include <unistd.h>
    
    int dup(int fildes);
    int dup2(int fildes, int fildes2);
    
# 5 标准I/O库

## 5.1 fopen

## 5.2 fread

## 5.3 fwrite

## 5.4 fclose

## 5.5 fflush

## 5.6 fseek

## 5.7 fgetc/getc/getchar

## 5.8 fputc/putc/putchar

## 5.9 fgets/gets

# 6 格式化输入输出

## 6.1 printf/fprintf/sprintf

## 6.2 scanf/fscanf/sscanf

## 6.3 其他流函数

- fgetpos
- fsetpos
- ftell
- rewind
- freopen
- setvbuf
- remove

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
    FILE *fdopen(int fildes, const char *mode);
    
# 7. 文件和目录的维护

## 7.1 chmod

## 7.2 chown

## 7.3 unlink/link/symlink

## 7.4 mkdir/rmdir

## 7.5 chdir/getcwd

# 8. 扫描目录

## 8.1 opendir

## 8.2 readdir

## 8.3 telldir

## 8.4 seekdir

## 8.5 closedir

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

对底层文件描述符提供了更多的操作方法。

    #include <fcntl.h>
    
    int fcntl(int fildes, int cmd);
    int fcntl(int fildes, int cmd, long arg);
    
对打开的文件描述符进程各种操作:

- 复制
- 获取和设置文件描述符标志
- 获取和设置文件状态标志
- 管理建议性文件锁

常用方法：

- fcntl(fildes, F_DUPFD, newfd)
返回一个新的文件描述符，其数值大于等于整数newfd。新文件描述符是fildes的一个副本。

- fcntl(fildes, F_GETFD)
返回文件描述符标志

- fcntl(fildes, F_SETFD, flags)
设置文件描述符标志

- fcntl(fildes, F_GETFL) | fcntl(fildes, F_SETFL, flags)
获取和设置文件状态标志和访问模式。

## 11.2 mmap函数

内存映射函数，建立一段可以被多个程序读写的内存。

可以通过管理内存的方式读写文件。

    #include <sys/mman.h>
    
    void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
    
- addr

请求某个特定的内存地址。推荐使用0，系统会自动分配地址。这样执行效率最高。

- off

共享内存段访问文件的起始位置。

- fildes
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























    

    
    
    