# 套接字

# 1. 套接字连接

### Demo 1

- 一个简单的本地客户

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/client1.c>

- 一个简单的服务器

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/server1.c>

运行：

    $ ./server1 &

当前目录中出现一个新文件：server_socket。这个文件可以用ls查看：

    $ ls -lF server_socket
    
    $ ./client1

## 1.1 套接字属性

- 域（domain）
指定套接字通信中使用的网络介质。AF_INET:Internet网络。

- 类型（type）
一个套接字域可能有多种不同的通信方式。AF_UNIX域没有这样的问题，它提供了一个可靠的双向通信路径。

AF_INET提供了两种通信机制：流和数据报

流套接字提供的是一个有序、可靠、双向字节流连接。由SOCK_STREAM指定。（TCP协议）

数据报套接字不建立和维持连接。由SOCKET_DGRAM指定。（UDP协议）

- 协议（protocol）
只要底层的传输机制允许不止一个协议，我们就可以选择一个协议。UNIX网络套接字不需要选择协议。

## 1.2 创建套接字

    #include <sys/types.h>
    #include <sys/socket.h>
    
    int socket(int domain, int type, int protocol);
    
创建的套接字是一条通信线路的一个端点。

- domain 协议族
- type 通信类型
- protocol 协议

## 1.3 套接字的地址

- AF_UNIX地址结构

    struct sockaddr_un{
        sa_family_t sun_family;
        char sun_path[];
    }
    
- AF_INET地址结构

    struct sockaddr_in{
        short int sin_family;
        unsigned short int sin_port;
        struct in_addr sin_addr;
    }

- IP地址结构

    struct in_addr {
        unsigned long int s_addr;
    }
    
## 1.4 命名套接字

把未命名的套接字关联到文件描述符，可以被其他进程访问。

    #include <sys/socket.h>
    
    int bind(int socket, const struct sockaddr* address, size_t address_len);
    
成功时返回0，失败时返回-1。并设置errno。

    EBADF   文件描述符无效
    ENOTSOCK    文件描述符对应的不是一个套接字
    EINVAL  文件描述符对应的是一个已经命名的套接字
    EADDRNOTAVAIL   地址不可用
    EADDRINUSE  地址已经绑定了一个套接字
    
## 1.5 创建套接字队列

    #include <sys/socket.h>
    
    int listen(int socket, int backlog);
    
listent队列长度设为backlog,等待处理的个数不能超过这个值。

## 1.6 接受连接

    #include <sys/socket.h>
    
    int accept(int socket, struct sockaddr* address, size_t* address_len)；
    
## 1.7 请求连接

    #include <sys/socket.h>
    
    int connect(int socket, const struct sockaddr* address, size_t address_len);
    
## 1.8 关闭套接字

通过close函数来终止套接字连接。

## 1.9 套接字通信

- 文件套接字：需要创建一个Server和Client都能访问到的文件
- 网络套接字：只需要IP和端口即可

一般推荐使用网络套接字

### Demo2

网络套接字

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/client2.c>

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/server2.c>

*注意：*请不要把这段代码用到你的程序中。

## 1.10 主机字节序和网络字节序

一般用netstat命令查看网络连接状况：

    $ netstat -A inet
    
由于不同计算机的网络字节序不同，我们需要将它们进行统一。

    # include <netinet/in.h>
    
    unsigned long int htonl(unsigned long int hostlong);
    unsigned short int htonl(unsigned short int hostshort);
    unsigned long int ntohl(unsigned long int netlong);
    unsigned short int ntohs(unsigned short int netshort);
    

### Demo3

- 使用网络字节序统一端口号

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/client3.c>

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/server3.c>

- server端

    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);

- client端

    address.sin_port = htons(9734);
    
# 2 网络信息

主机数据库函数

    #include <netdb.h>
    
    struct hostent *gethostbyaddr(const void *addr, size_t len, int type);
    struct hostent *gethostbyname(const char *name);
    
    struct hostent{
        char *h_name;
        char **h_aliases;
        int h_addrtype;
        int h_length;
        char **h_addr_list;
    };
    
如果没有我们要查询的数据，这些信息会返回一个空指针。

与服务及其端口号有关的信息也可以通过一些服务信息函数来获取。

    #include <netdb.h>
    
    struct servent *getservbyname(const char* name, const char* proto);
    struct servent *getservbyport(int port, const char *proto);
    
proto指定用于连接该服务的协议，tcp或udp.

    struct servent{
        char *s_name;
        char **s_aliases;
        int s_port;
        char *s_proto;
    };
    
注意：返回地址列表要转换为正确的地址类型，并用函数inet_ntoa将它从网络字节序转换为可打印字节序。

    #include <arpa/inet.h>
    
    char *inet_ntoa(struct in_addr in);
    
这个函数将一个网络地址转换成点分十进制字符串。

    #include <unistd.h>
    
    int gethostname(char *name, int namelength);
    
### Demo4

获取一台主机的相关信息。

<https://github.com/breakerthb/LinuxPrograming/master/15_Socket/getname.c>

运行：

    $ ./getname localhost

### Demo5

连接到标准服务

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/getdate.c>

运行：

    $ ./getdate localhost
    
可以用它得到任何主机的时间信息。

## 2.1 因特网守护进程


## 2.2 套接字选项

你可以用很多选项来控制套接字行为。

    #include <sys/socket.h>
    
    int setsockopt(int socket, int level, int option_name, const void* option_value, size_t option_len);
    
setsockopt用来控制这些套接字选项。

# 3. 多客户

### Demo 6

可以同时服务多个客户的服务器:

- 服务器fork新的子进程提供服务，保证一直处于监听状态
- 提供服务加入5s延迟

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/server4.c>

## 3.1 select系统调用

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
    
select函数：

    #include <sys/types.h>
    #include <sys/time.h>
    
    int select(int nfds, fd_set *readfds, fd_set *writefds,
                fd_set *errorfds, struct timeval *timeout);
                
select用于阻塞程序，直到文件描述符集合中，有一个文件描述符已经处于可读、可写或错误状态。

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

- 返回条件


    1. readfds集合中，有文件描述符可读
    2. writefds集合中，有文件描述符可写
    3. errnofds集合中，有文件描述符遇到错误条件
    4. 如果前面三种情况都没有发生，select将在timeout指定的时间后返回。

返回后需要通过FD_ISSET判断哪个描述符发生了变化。

select会返回发生变化的文件描述符总数，失败时返回-1并设置errno来描述错误。

    EBADF   无效描述符
    EINTR   因中断而返回
    EINVAL  nfds或timeout取值错误

### Demo 7

通过select挂起等待标准输入。

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/select.c>

### Demo 7.1

我们可以通过select实现一个简单的定时器。

子进程每2.5秒提示主进程一次。

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/select_timer.c>

## 3.2 多客户

### Demo 8

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/server5.c>

运行：

    $ ./server5 &
    $ ./client3 & ./client3 & ./client3 & ps x

# 4. 数据报

UDP方式连接

# Socket通信

参考：<https://github.com/breakerthb/Tinyhttpd/tree/reuse>

从开源项目中提炼出的Socket服务器。