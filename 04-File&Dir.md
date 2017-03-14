# 1. 引言

文件属性


# 2. stat() fstat() fstatat() lstat()

返回与打开文件描述符相关的文件状态信息。

    #include <unistd.h>
    #include <sys/stat.h>
    #incluce <sys/types.h>
    
    int stat(const char *path, struct stat *buf);
    int fstat(int fd, struct stat *buf);
    int lstat(const char *path, struct stat *buf);
    int fstatat(int fd, const char *path, struct stat *buf, int flag);
    
## 返回值

- 成功，返回0
- 失败，返回-1

![4-1](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/4-1.png)

# 3. 文件类型

- 普通文件(regular file)

- 目录文件(directory file)

- 块特殊文件(block special file)
对设备提供带缓冲区的访问

- 字符特殊文件(character special file)
对设备提供不带缓冲区的访问

- FIFO
管道文件

- 套接字(socket)
socket文件

- 符号链接(symbolic link)
相当于超链接

在stat结构的st_mode成员中。判断方法：

![4-2](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/4-2.png)


## Demo

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/filedir/filetype.c>

    #include "apue.h"
    int main(int argc, char *argv[])
    {
        int i;
        struct stat buf;
        char *ptr;
        for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);
            if (lstat(argv[i], &buf) < 0) {
                err_ret("lstat error");
                continue;
            }
            if (S_ISREG(buf.st_mode))
                ptr = "regular";
            else if (S_ISDIR(buf.st_mode))
                ptr = "directory";
            else if (S_ISCHR(buf.st_mode))
                ptr = "character special";
            else if (S_ISBLK(buf.st_mode))
                ptr = "block special";
            else if (S_ISFIFO(buf.st_mode))
                ptr = "fifo";
            else if (S_ISLNK(buf.st_mode))
                ptr = "symbolic link";
            else if (S_ISSOCK(buf.st_mode))
                ptr = "socket";
            else
                ptr = "** unknown mode **";
                
            printf("%s\n", ptr);
        }
        
        exit(0);
    }

输入参数即可得到类型。

    $ ./filetype /etc/passwd /etc
    
# 4. 设置用户ID和组ID

### Ref : P78

# 5. 文件访问权限

这个也在st_mode中包含。

![4-6](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/4-6.png)

# 6. 新文件和目录的所有权

### Ref : P80

# 7. access() faccessat()

    #include <unistd.h>
    
    int access(const char *path, int mode);
    int faccessat(int fd, const char *path, int mode, int flag);
    
## 返回值

- 成功，返回0
- 出错，返回-1

mode的取值：

- F_OK  测试文件是否存在
- R_OK  测试读权限
- W_OK  测试写权限
- X_OK  测试执行权限

第一个单独使用，后三个可以通过位运算联合使用。

## Demo

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/filedir/access.c>

    #include "apue.h"
    #include <fcntl.h>
    
    int main(int argc, char *argv[])
    {
    	if (argc != 2)
    		err_quit("usage: a.out <pathname>");
    	if (access(argv[1], R_OK) < 0)
    		err_ret("access error for %s", argv[1]);
    	else
    		printf("read access OK\n");
    	if (open(argv[1], O_RDONLY) < 0)
    		err_ret("open error for %s", argv[1]);
    	else
    		printf("open for reading OK\n");
    	exit(0);
    }

# 8. umask()

为进程设置文件模式创建屏蔽字。

    #include <sys/stat.h>
    
    mode_t umask(mode_t cmask);
    
## Demo 

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/filedir/umask.c>
    
### Ref : P84

# 9. chmod() fchmod() fchmodat()

修改现有文件的访问权限

    #include <sys/stat.h>
    
    int chmod(const char *path, mode_t mode);
    int fchmode(int fd, mode_t mode);
    int fchmodat(int fd, const char *path, mode_t mode, int flag);
    
## 返回值

- 成功：返回0
- 失败：返回-1

## Demo 

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/filedir/changemod.c>

# 10. 粘着位

# 11.chown() fchown() fchownat() lchown()

# 12. 文件长度

stat结构中的st_size表示以字节为单位的文件长度。

# 13. 文件截断

    #include <unistd.h>
    
    int truncate(const char *pathname, off_t length);
    int ftruncate(int fd, off_t length);
    
# 14. 文件系统

### Ref : P90

# 15. link() linkat() unlink() unlinkat() remove()

这里说的链接是硬链接。

创建链接方法：

    #include <unistd.h>
    
    int link(const char *existingpath, const char *newpath);
    int linkat(int efd, const char * existingpath, int nfd, const char *newpath, int flag);
    
返回值

- 成功：返回0
- 失败：返回-1

删除现有链接：

    #include <unistd.h>
    
    int unlink(const char *pathname);
    int unlinkat(int fd, const char *pathname, int flag);
    
解除链接：

    #include <unistd.h>
    
    int remove(consst char* pathname);
    
- 对于文件，相当于unlink
- 对于目录，相当于rmdir

# 16. rename() renameat()

重命名

    #include <stdio.h>
    
    int rename(cosnt char* oldname, cosnt char* newname);
    int renameat(int oldfd, cosnt char *oldname, int newfd, const char *newname);
    
# 17. 符号链接

符号链接是对一个文件的间接指针，也叫软链接。

# 18. 创建和读取符号链接

    # include <unistd.h>
    
    int symlink(const char *actualpath, const char *sympath);
    int symlinkat(cosnt char *actualpath, int fd, const char * sympath);
    
打开链接本身：

    #include <unistd.h>
    
    ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsize);
    ssize_t readlinkat(int fd, const char* restrict pathname, char *restrict buf, size_t bufsize);
    
# 19. 文件的时间

# 20. futimens() utimensat() utimes()

# 21. mkdir() mkdirat() rmdir()

创建和删除目录。

    #include <sys/stat.h>
    
    int mkdir(const char *pathname, mode_t mode);
    int mkdirat(int fd, const char *pathname, mode_t mode);
    
    int rmdir(const char *pathname);

# 22. 读目录

# 23. chdir() fchdir() gercwd()

更改当前工作目录。



## 1.2 文件和设备

比较重要的设备文件有三个：

- /dev/console

- /dev/tty

- /dev/null

空设备，所以无用的输出信息都可以重定向到这里



# 3. 库函数

底层函数开销较大，因此要减少使用次数。于是就要是用库函数。


    
### 4.6.2 fstat,stat和lstat系统调用


    
### 4.6.3 dup和dup2系统调用
    
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