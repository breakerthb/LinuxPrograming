# 进程间通信：管道

# 1. 进程管道

    #include <stdio.h>
    FILE* popen(const char* command, const char* open_mode);
    int pclose(FILE* stream_to_close);
    
## popen()

一个进程将另一个进程作为一个新进程启动，并可以传递数据给它或通过它接受数据。

- command : 需要运行的程序名
- open_mode : “r” 或 “w”

"r"模式打开时，通过fread读入数据，"w"模式打开时，通过fwrite写数据。

每个popen()只能执行读或者写中的一个操作，不能同时读写。如果需要读和写两个功能，需要创建两个管道。

## pclose()

关闭之前文件流。返回另一个进程的退出码。

*注意：*在pclose之前，不能使用wait函数，否则得不到退出码。

## Demo

从另一个进程读入数据。

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_popen1.c>

# 2. 将输出送往popen

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_popen2.c>

相当于:

    $ echo "Once upon a time, there was ..." | od -c
    
## 2.1 数据分多块传递

通过多次调用fread从另一个进程多次读入数据。

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_popen3.c>

程序中每次读取"BUFSIZ / 10"，因为BUFSIZ太大，一次就能读入全部数据。

## 2.2 popen函数评价

popen在启动另一个进程时，通过启动一个新shell解析command命令来完成。

- 优势

通过Shell执行新进程方便，比exec方便

- 缺点

启动Shell的方式不够经济

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_popen4.c>

这个Demo执行效率不如：

    $ cat demo*.c | wc -l
    
# 3. pipe调用

通过这个函数在两个进程间传递数据不需要启动一个新的Shell。

    #include <unistd>
    int pipe(int file_descriptor[2]);
    
参数是两个整数文件描述符组成的数组。成功返回0，失败返回-1，并设置errno。
常见错误：

- EMFILE : 进程使用文件描述符过多
- ENFILE : 系统文件表已满
- EFAULT : 文件描述符无效

写到file_descriptor[1]的所有数据都可以从file_descriptor[0]读回来。依据FIFO原则。

![Pipe](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/pipe.png)

*注意：*这里使用的文件描述符不是文件流，因此不能使用fread和fwrite处理数据，需要使用底层的read和write方法。

## Demo

利用pipe传递数据

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_pipe1.c>

*注意：*pipe传递数据的方向只能是file_descriptor[1]到file_descriptor[0],反之会出现各种意想不到的错误。

## 进程间数据传递

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_pipe2.c>

# 4. 父进程和子进程

在子进程中运行一个和父进程完全不同的程序。通过fork和exec联合使用完成。

*重点：*替换子进程的新进程需要拿到管道文件描述符，这个通过exec参数传递完成。

- demo_pipe3.c 数据生产者

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_pipe3.c>



- demo_pipe4.c 数据消费者

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_pipe4.c>

## 4.1 管道关闭后的读操作

父进程文件描述符关闭

## 4.2 把管道用作标准输入输出

打开一个新的文件描述符。

    #include <unistd.h>
    
    int dup(int file_descriptor)；
    int dup2(int file_descriptor_one, int file_descriptor_two);
    
一般标准输入时0，标准输出是1。

dup调用创建的新文件描述符与作为参数的那个已有文件描述符指向同一个文件（或管道）。

    文件描述符 0    标准输入        
    文件描述符 1    标准输出
    文件描述符 2    标准错误输出
    文件描述符 3    管道文件描述符
    
<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_pipe5.c>

demo_pipe5.c流程：

- 子进程close(0)关闭标准输入
- 子进程调用dup(file_pipes[0]),把读取端关联的文描述符复制为文件描述符0
- 子进程关闭file_pipes[0]
- 子进程关闭file_pipes[1]
- 子进程exec启动新程序

- 父进程先关闭file_pipes[0]
- 向管道写入数据
- 写完数据后关闭file_pipes[1]并退出
    

# 5. 命名管道：FIFO

命令行创建命名管道：

    $ mkfifo filename
    
程序中调用：

    #include <sys/types.h>
    #include <sys/stat.h>
    
    int mkfifo(const char* filename, mode_t mode);
    int mknod(const char* filename, mode_t mode | S_IFIFO, (dev_t) 0);
    
## Demo 

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    
    int main()
    {
        int res = mkfifo("/tmp/my_fifo", 0777);
        if (res == 0)
            printf("FIFO created\n");
            
        exit(EXIT_SUCCESS);
    }

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_fifo1.c>

执行上面的程序，之后可以在命令行查找FIFO文件：

    $ ls -lF /tmp/my_fifo
    prwxr-xr-x 1 ubuntu ubuntu 0 Dec 13 03:21 /tmp/my_fifo|
    
第一个字符p和最后一个字符|表示这是一个管道

## 5.1 访问FIFO文件

命令行

    $ cat < /tmp/my_fifo
    
另一个shell中执行：

    $ echo "Hello World" > /tmp/my_fifo
    
### 使用open打开FIFO文件

- open(const char* path, O_RDONLY);

open会被阻塞，只有当另一个程序写方式打开FIFO文件为止。

- open(const char* path, O_RDONLY | O_NONBLOCK);

即使没有其他写方式打开，也能成功返回

- open(const char* path, O_WRONLY);

open会被阻塞，直到另一个程序读方式打开FIFO文件为止。

- open(const char* path, O_WRONLY | O_NONBLOCK);

立刻返回，如果没有读方式打开，则返回一个错误。

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_fifo2.c>

## 5.2 对FIFO进行读写操作

不建议使用

## 5.3 使用FIFO进间通信

- 生产者程序 demo_fifo3.c

创建管道，向管道写入数据

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_fifo3.c>

- 消费者程序 demo_fifo4.c

从管道读取数据并丢弃

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/demo_fifo4.c>

先执行程序demo_fifo3,再执行程序demo_fifo4.

# 6. 使用FIFO的客户/服务器应用

<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/server.c>
<https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe/client.c>

执行：

    $ make
    $ ./server &
    $ for i in 1 2 3 4 5
    do
    ./client &
    done
    $
    
# 7. CD数据库应用程序




    

