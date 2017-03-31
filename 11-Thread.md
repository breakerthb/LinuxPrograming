# POSIX线程

进程和线程的区别：

- 进程

fork之后，将创建出该进程的副本。这个新进程拥有自己的变量和自己的PID，时间调度独立。几乎完全独立于父进程。

- 线程

新线程拥有自己的栈和局部变量，但与它的创建者共享全局变量、文件描述符、信号处理函数和当前目录状态。

# 1. 第一个线程程序

- 可重入

在程序所有include之前定义宏：

    #define _REENTRANT
    
- 编译选项

编译过程加上：

    -lpthread
    
## 1.1 pthread_create()

创建线程函数。

    #include <pthread.h>
    
    int pthread_create(pthread_t* thread, pthread_attr_t* attr, 
                    void *(*start_routine)(void*), void* arg);
                    
### 参数

- thread, 线程指针；
- attr, 线程属性，常用NULL；
- start_routine, 线程主体函数指针；
- arg, start_routine函数参数。

## 1.2 pthread_exit()

终止线程函数。

    #include <pthread.h>
    
    void pthread_exit(void* retval);
    
终止调用这个函数的线程，同时用retval返回变量。retval的值可以在pthread_join的第二个参数中得到。注意：retval返回的不能是局部变量，因为线程结束后所有局部变量都被销毁。

## 1.3 pthread_join()

相当于进程中的wait函数。

    #include <pthread.h>
    
    int pthread_join(pthread_t th, void **thread_return);
    
### 参数

- th, 等待线程的指针
- thread_return, 带回线程返回值

## Demo

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/12_Thread/demo_thread1.c>


编译：

    $cc -D_REENTRANT demo_thread1.c -o run_thread1 -lpthread
    
# 2. 同步

两种基本方法：信号量和互斥量

## 2.1 信号量

信号量是一个特殊的变量，它可以被增加减少，但对它的关键访问保持原子操作。

分类：

- 二进制信号量
- 计数信号量

### 创建信号量

    #include <semaphore.h>
    
    int sem_init(sem_t *sem, int pshared, unsigned int value);
    
- sem

初始化sem指向的信号量

- pshared

信号量类型，目前Linux只支持0

### 控制信号量的值

    #include <semaphore.h>
    
    int sem_wait(sem_t* sem);   // 原子方式减1
    int sem_post(sem_t* sem);   // 原子方式加1

当信号量的值为0时执行sem_wait，线程会等待。

### 清理信号量

    #include <semaphore.h>
    
    int sem_destroy(sem_t* sem);
    
如果信号量正被等待，则返回错误。

### Demo - thread3.c

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/12_Thread/demo_thread3.c>

信号量常用作主线程控制子线程流程。


## 2.2 互斥量



# 3. 线程属性


# 4. 取消一个线程


# 5. 多线程




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


# 5. 用户信息

    #include <sys/types.h>
    #include <unistd.h>
    
    uid_t getuid(void); // 返回UID
    char *getlogin(void); // 返回登录名
    
更有效的用户信息接口

    #include <sys/types.h>
    #include <pwd.h>
    
    struct passwd *getpwuid(uid_t uid);
    struct passwd *getpwnam(const char *name);
    
# 6. 主机信息

- gethostname
- uname
- gethostid

# 7. 日志

    #include <syslog.h>
    
    void syslog(int priority, const char *message, arguments ...);
    

