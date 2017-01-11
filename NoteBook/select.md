# select()

select系统调用允许程序同时在多个底层文件描述符上等待输入的到达。防止“忙等待”

select对数据结构fd_set进行操作，下面的宏定义处理这些集合。

    #include <sys/types.h>
    #include <sys/time.h>
    
    // 初始化为空
    void FD_ZERO(fd_set *fdset); 
    // 清除fd
    void FD_CLR(int fd, fd_set *fdset);
    // 设置fd
    void FD_SET(int fd, fd_set *fdset);
    // 判断是否已经被设置。如果fd已经被FD_SET设置过了，返回一个非零值。
    int FD_ISSET(int fd, fd_set *fdset);
    
select函数可以用一个超时值来防止无限阻塞。

    struct timeval{
        time_t tv_sec;  // seconds
        long tv_usec;   // microseconds
    };
    
# select函数定义

    #include <sys/types.h>
    #include <sys/time.h>
    
    int select(int nfds, fd_set *readfds, fd_set *writefds,
                fd_set *errorfds, struct timeval *timeout);
                
select用于阻塞程序，直到文件描述符集合中，有一个文件描述符已经处于可读、可写或错误状态。

## 参数

- nfds

需要被测试的文件描述符数，范围：0 ~ nfds-1

- readfds

可读文件描述符集合，可为空

- writefds

可写文件描述符集合，可为空

- errnofds

错误文件描述符集合，可为空

- timeout

超时时间设置，如果NULL则一直阻塞下去。
注意：select每执行一次，timeout内的值会被清零。

## 返回条件

- 1. readfds集合中，有文件描述符可读
- 2. writefds集合中，有文件描述符可写
- 3. errnofds集合中，有文件描述符遇到错误条件
- 4. 如果前面三种情况都没有发生，select将在timeout指定的时间后返回。

返回后需要通过FD_ISSET判断哪个描述符发生了变化。

select会返回发生变化的文件描述符总数，失败时返回-1并设置errno来描述错误。

    EBADF   无效描述符
    EINTR   因中断而返回
    EINVAL  nfds或timeout取值错误

## Demo 1

通过select挂起等待标准输入。

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/select.c>

## Demo 2

我们可以通过select实现一个简单的定时器。

子进程每2.5秒提示主进程一次。

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/select_timer.c>