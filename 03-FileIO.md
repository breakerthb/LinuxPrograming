# 文件I/O

# 1. 不带缓冲的I/O

- open
- read
- write
- lseek
- close

# 2. 文件描述符

|Value|Descrip|<unistd.h>|
|:-:|:-:|:-:|
|0|标准输入|STDIN_FILENO|
|1|标准输出|STDOUT_FILENO|
|2|标准错误|STDERR_FILENO|

范围：0 ~ OPEN_MAX - 1

# 3. open()和openat()

创建一个新的文件描述符

    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    
    int open(const char *path, int oflags);
    int open(const char *path, int oflags, mode_t mode);
    int openat(int fd, const char *path, int oflags);
    int openat(int fd, const char *path, int oflags, mode_t mode);
    
## 返回值

- 成功，返回值
- 错误，返回-1
    
- O_RDONLY  以只读方式打开
- O_WRONLY  以只写方式打开
- O_RDWR    以读写方式打开

可选模式组合：

- O_APPEND  把写入数据追加在文件的末尾
- O_TRUNC   把文件长度设置为零，丢弃已有内容
- O_CREAT   如果需要，就按照参数mode中给出的访问模式创建文件
- O_EXCL    与O_CREAT一起使用，确保调用者创建出文件。
 
### ref : P50

# 4. creat()

创建一个新文件

    # include <fcntl.h>
    
    int creat(const char *path, mode_t mode);
    
## 返回值

- 成功，返回只写打开的文件描述符
- 失败，返回-1

这个函数可以被open替代如下：

    open(path, O_WRONLY|O_TRUNC, mode);
    
# 5. close()

关闭一个打开的文件

    #include <unistd.h>
    
    int close(int fd);

## 返回值

- 成功，返回0
- 失败，返回-1

# 6. lseek()


对文件描述符fd的读写指针进行设置。也就是当前读写位置

    #include <unistd.h>
    #include <sys/types.h>
    
    off_t lseek(int fd, off_t offset, int whence);

## 返回值

- 成功， 返回文件偏移量
- 失败，返回-1

## 参数

- offset, 偏移量
- whence, 起始位置

    SEEK_SET
    SEEK_CUR
    SEEK_END

# 7. read()

    #include <unistd.h>
    
    size_t read(int fd, void *buf, size_t nbytes);
    
## 返回值

- 读到的字节数，若已经到文件尾，返回0
- 若失败，返回-1

# 8. write()

    #include <unistd.h>
    
    size_t write(int fd, const void *buf, size_t nbytes);

## 返回值

- 成功，返回已写的字节数
- 失败，返回-1

# 9. I/O效率

### ref : P58

# 10. 文件共享

UNIX允许不同进程共享打开文件。

## 10.1 内核中打开文件的数据结构

用三个数据结构管理进程、文件、节点之间的关系。

- 一个进程打开两个文件

![3-7](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/3-7.png)

- 两个进程打开同一个文件

![3-8](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/3-8.png)

# 11. 原子操作

执行一个没有子集的操作，要么不执行，要么一次执行完成。中间不会被任何命令打断。

## 11.1 追加文件

打开文件时使用O_APPEND标志，这样两个进程同时写入一个文件时不能使用lseek修改写入位置。

## 11.2 pread()和pwrite()

    # include <unistd.h>
    
    ssize_t pread(int fd, void *buf, size_t nbytes, off_t offset);
    ssize_t pwrite(int fd, const void *buf, size_t nbytes, off_t offset);
    
这两个函数相当于先调用lseek之后调用read和write,不同之处在于两个动作之间无法中断。

## 11.3 创建一个文件

在open创建文件时，加入O_CREAT和O_EXCL选项。

# 12. dup()和dup2()

dup提供了一种复制文件描述符的方法，使我们能供通过两个或更多不同的文件描述符来访问同一个文件。

    #include <unistd.h>
    
    int dup(int fd);
    int dup2(int fd, int fd2);

## 返回值

- 成功，返回新文件描述符
- 失败，返回-1

注意：

- dup返回当前可用文件描述符的最小值
- dup2返回指定的文件描述符(fd2)


    dup(fd);

相当于：

    fcntl(fd, F_DUPFD, 0);
    
而

    dup2(fd, fd2);
    
相当于：

    close(fd2);
    fcntl(fd, F_DUPFD, fd2);
    
不过，dup2()是原子操作。

# 13. sync(),fsync()和fdatasync()

    #include <unistd.h>
    
    int fsync(int fd);
    int fdatasync(int fd);
    
成功返回0，失败返回-1

    void sync(void);

- sync 将缓冲区排入写队列，立刻返回
- fsync只对某个文件起作用，写磁盘动作完成后返回
- fdatasync类似于fsync，只更新文件数据部分，不更新属性部分。

# 14. fcntl()

对底层文件描述符提供了更多的操作方法。

    #include <fcntl.h>
    
    int fcntl(int fd, int cmd);
    int fcntl(int fd, int cmd, long arg);
    
对打开的文件描述符进程5种操作:

![3-9](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/3-9.png)

常用方法：

- fcntl(fildes, F_DUPFD, newfd)
返回一个新的文件描述符，其数值大于等于整数newfd。新文件描述符是fildes的一个副本。

- fcntl(fildes, F_GETFD)
返回文件描述符标志

- fcntl(fildes, F_SETFD, flags)
设置文件描述符标志

- fcntl(fildes, F_GETFL) | fcntl(fildes, F_SETFL, flags)
获取和设置文件状态标志和访问模式。

### ref : P65

# 15. ioctl()

    #include <unistd.h>
    
    int ioctl(int fd, int cmd, ...);
    
### ref : P70

# 16. /dev/fd

打开/dev/fd/n相当于复制描述符n.

    fd = open("/dev/fd/0", mode);
    
相当于：

    fd = dup(0);
    
