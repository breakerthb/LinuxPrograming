# 套接字

# 1. 套接字连接

## Demo

- 一个简单的本地客户

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/15_Socket/client1.c>

- 一个简单的服务器

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/15_Socket/server1.c>

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

