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

更改当前工作目录：

    #include <unistd.h>
    
    int chdir(const char *pathname);
    int fchdir(int fd);
    
通过路径或描述符设定当前目录。

得到当前目录完整路径名：

    #include <unistd.h>
    
    char *getcwd(char *buf, size_t size);
    
返回值：

- 成功：返回buf
- 失败，返回NULL

# 24. 设备特殊文件

# 25. 文件访问权限总结

![4-26](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/4-26.png)

最后9个常量还可以分成如下三组：

    S_IRWXU = S_IRUSR | S_IWUSR | S_IXUSR
    S_IRWXG = S_IRGRP | S_IWGRP | S_IXGRP
    S_IRWXO = S_IROTH | S_IWOTH | S_IXOTH


