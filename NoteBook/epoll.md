# epoll()

功能和select和poll类似，只是性能更好。

# epoll_create()

创建epoll实例

    #include <sys/epoll.h>
    
    int epoll_create(int size);
    
- size

要检查的文件描述符个数。

- 返回值


    成功 ：epoll实例的文件描述符。不用时需要close掉。
    失败 ：-1 errorcode
    
# epoll_ctl()

修改epoll的兴趣列表

    #include <sys/epoll.h>
    
    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);
    
- fd

要修改的文件描述符

- op

要执行的操作

    EPOLL_CTL_ADD   将fd添加到epfd的兴趣列表中
    EPOLL_CTL_MOD   修改fd上设定的事件
    EPOLL_CTL_DEL   将fd从epfd的兴趣列表中移除

- ev

epoll_event指:

    struct epoll_event {
        uint32_t events; /* epoll events (bit mask) */
        epoll_data_t data; /* User data */
    };
    
epoll_data

    typedef union epoll_data {
        void *ptr; /* Pointer to user-defined data */
        int fd; /* File descriptor */
        uint32_t u32; /* 32-bit integer */
        uint64_t u64; /* 64-bit integer */
    } epoll_data_t;
    
## Demo

    int epfd;
    struct epoll_event ev;
    
    epfd = epoll_create(5);
    if (epfd == -1)
        errExit("epoll_create");
        
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev) == -1)
        errExit("epoll_ctl");
        
# epoll_wait()

事件等待。返回epoll实例中处于就绪状态的文件描述符信息。

    #include <sys/epoll.h>
    int epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout); // Returns number of ready file descriptors, 0 on timeout, or –1 on error
    
# epoll事件

    Table 63-8: Bit-mask values for the epoll events field
    Bit         Input to epoll_ctl()?    Returned by epoll_wait()?    Description
    EPOLLIN         •                           •                       Data other than high-priority data can be read
    EPOLLPRI        •                           •                       High-priority data can be read
    EPOLLRDHUP      •                           •                       Shutdown on peer socket (since Linux 2.6.17)
    EPOLLOUT        •                           •                       Normal data can be written
    EPOLLET         •                                                   Employ edge-triggered event notification
    EPOLLONESHOT    •                                                   Disable monitoring after event notification
    EPOLLERR                                    •                       An error has occurred
    EPOLLHUP                                    •                       A hangup has occurred
    
# Demo

