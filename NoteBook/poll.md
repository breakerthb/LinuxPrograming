# poll()

poll()的功能和select()相似。

两者主要区别在于我们要如何指定待检查文件描述符。在select()中，我们提供了三个集合，而在poll()中，我们提供一系列文件描述符。

# 函数定义

    #include <poll.h>
    
    int poll(struct pollfd fds[], nfds_t nfds, int timeout); // Returns number of ready file descriptors, 0 on tinmeout, or -1 on error
    
## 参数

- fds

列出需要poll()来检查的文件描述符。

    struct pollfd{
        int fd;     // file descriptor
        short events;   // Requested events bit mask
        short revents;  // Returned events bit mask
    }
    
events和revents都是位掩码值。结合下列常数值(宏)指定即将唤醒的事件或调查已结束的 poll() 函数被唤醒的原因，这些宏常数如下：

    POLLIN  events中使用该宏常数，能够在折本文件的可读情况下，结束 poll() 函数。相反，revents 上使用该宏常数，在检查 poll() 函数结束后，可依此判断设备文件是否处于可读状态(即使消息长度是 0)。
    POLLPRI events 域中使用该宏常数，能够在设备文件的高优先级数据读取状态下，结束 poll() 函数。相反，revents 上使用该宏常数，在检查 poll() 函数结束后，可依此判断设备文件是否处于可读高优先级数据的状态(即使消息长度是 0)。该宏常数用于处理网络信息包(packet) 的数据传递。
    POLLOUT events 域中使用该宏常数，能够在设备文件的写入状态下，结束 poll() 函数。相反，revents 域上使用该宏常数，在检查 poll() 结束后，可依此判断设备文件是否处于可写状态。
    POLLERR events 域中使用该宏常数，能够在设备文件上发生错误时，结束 poll()　函数。相反，revents　域上使用该宏函数，在检查 poll()　函数结束后，可依此判断设备文件是否出错。
    POLLHUP events　域中使用该宏常数，能够在设备文件中发生 hungup　时，结束 poll() 函数 。相反，在检查 poll() 结束后，可依此判断设备文件是否发生 hungup 。
    POLLNVAL    events 域中使用该宏函数，能够在文件描述符的值无效时，结束 poll() 。相反，在 revents 域上使用该宏函数时，在检查 poll() 函数后，文件描述符是否有效。可用于处理网络信息时，检查 socket handler 是否已经无效。
    
如果队某个特定的文件描述符上的事件不感兴趣，可一个将events设为0。
另外，给fd字段指定一个负值将导致对应的events字段被忽略，且revents字段总是返回0。

- nfds

fds[]中元素个数

- timeout


    -1 : 一直阻塞到有一个文件描述符就绪
    0 : 不阻塞，执行一次检查一次
    正整数 : 最多阻塞timeout
    
## 返回值

    -1 : 有错误发生。
    0 : 在任意一个文件描述符就绪前超时。
    正整数 : n个描述符就绪。
    
# Demo



# Ref

Linux系统编程手册 - chap 63 - page 1097
    