# 信号量、共享内存和消息队列

# IPC机制

IPC(Inter-Process Communicate 进程间通信)机制：

- 信号量
- 共享内存
- 消息队列

# 1. 信号量

它是一个特殊的变量，只允许对他进行等待(wait)和发送信号(signal)这两种操作。用下面的符号表示这两种操作：

- P(信号量变量)：用于等待
- V(信号量变量)：用于发送信号

## 1.1 信号量的定义

    定义一个信号变量sv
    P(sv)   如果sv的值大于0，就给它减去1;如果它的值等于0，就挂起该进程的执行
    V(sv)   如果有其它进程因等待sv而被挂起，就让它恢复运行;如果没有进程因等待sv而挂起，就给它加1
    
使用：

    P(sv)
    // Do something
    V(sv)
    
- 当临界区可用时，sv的值是true或1
- P(sv)操作将sv减1变成false表示临界区正在使用
- 进程离开时，V(sv)将sv加1，使临界区变为可用

注意：用一个普通变量来操作是不行的，因为C/C++中没有判断变量的原子操作。

## 1.2 Linux的信号量机制

信号量函数的定义：

    #include <sys/sem.h>
    
    int semctl(int sem_id, int sem_num, int command, ...);
    int semget(key_t key, int num_sems, int sem_flags);
    int semop(int sem_id, struct sembuf *sem_ops, size_t num_sem_ops);
    
### semget函数

创建一个新的信号量或取得一个已有信号量的键：

    int semget(key_t key, int num_sems, int sem_flags);
    
- key

整数值，信号量的唯一标识符。不相关的进程可以通过它访问同一个信号量。创建时需要一个唯一的非0整数。

- num_sems

指定需要信号量的数目。几乎总是取1。

- sem_flags

信号量标志。

一般用IPC_CREAT和IPC_EXCL来确保创建出一个新的、唯一的信号量。如果信号量已经存在，返回一个错误。

- 返回值

成功时返回一个正整数，既标识符。错误时返回-1。

### semop函数

用于改变信号量的值。

    int semop(int sem_id, struct sembuf *sem_ops, size_t num_sem_ops);

- sem_id

信号量标识符。

- sem_ops

    struct sembuf{
        short sem_num;  // 信号量编号，取值一般为0
        short sem_op;   // 信号量在一次操作中需要改变的数值。通常用到两个值：-1，P操作，+1，V操作。
        short sem_flg;  // 通常设置为SEM_UNDO
    }

### semctl函数

直接控制信号量信息

    int semctl(int sem_id, int sem_num, int command, ...);
    
- sem_id

信号量标识符

- sem_num

信号量编号，需要成组的信号量时用到这个标志。一般为0。

- command

将要采取的行动。
两个常用值：

    SETVAL      把信号量初始化为一个已知的值
    IPC_RMID    用于删除一个已经无需继续使用的信号量标识符

- 第四个参数

如果有第四个参数，一定是union结构

    union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    }

## 1.3 使用信号量

### Demo - sem1.c

# 2. 共享内存

允许两个相关进程访问同一块逻辑内存。

函数定义：

    #include <sys/shm.h>
    
    void *shmat(int shm_id, const void *shm_addr, int shmflg);
    int shmctl(int shm_id, int cmd, struct shmid_ds *buf);
    int shmdt(const void *shm_addr);
    int shmget(key_t key, size_t size, int shmflg);
    
## 2.1 shmget函数

    int shmget(key_t key, size_t size, int shmflg);
    
- key

共享内存唯一标识符

- size

共享内存空间大小，以字节为单位

- shmflg

IPC_CREAT

- 返回值

成功，返回一个非负值，失败，返回-1

## 2.2 shmat函数

创建了共享内存时，它不能被任何进程访问。需要访问，必须链接到某个进程的地址空间中。

    void *shmat(int shm_id, const void *shm_addr, int shmflg);
  
- shm_id

共享内存段标识符

- shm_addr

共享内存链接到当前进程的地址空间。它通常是一个空指针。

- shmflg

一组位标准。

- 返回值

成功，返回指针；失败，返回-1

## 2.3 shmdt函数

将共享内存从当前进程中分离。

    int shmdt(const void *shm_addr);

## 2.4 shmctl函数

共享内存控制函数。

    int shmctl(int shm_id, int cmd, struct shmid_ds *buf);

    struct shmid_ds{
        uid_t shm_perm.uid;
        uid_t shm_perm.gid;
        mode_t shm_perm.mode;
    }

- shm_id

共享内存段标志

- cmd

要采取的动作。

    IPC_STAT    把shmid_ds结构中的数据设置为共享内存的当前关联值
    IPC_SET     如果进程由足够的权限，就把共享内存的当前关联值设置为shmid_ds结构中的值
    IPC_RMID    删除共享内存段

- buf

指向包含共享内存模式和访问权限的结构的指针。

- 返回值

成功返回0，失败返回-1.

### Demo - shm1.c shm2.c


# 3. 消息队列

消息队列与命名管道有许多相似之处，但少了在打开关闭管道方面的复杂性。

函数定义：

    #include <sys/msg.h>
    
    int msgctl(int msqid, int cmd, struct msqid_ds *buf);
    int msgget(key_t key, int msgflg);
    int msgrcv(int msqid, void *msg_ptr, size_t msg_sz, long int msgtype, int msgflg);
    int msgsnd(int msqid, const void *msg_ptr, size_t msg_sz, int msgflg);
    
## 3.1 msgget函数

创建和访问一个消息队列。

    int msgget(key_t key, int msgflg);
    
## 3.2 msgsnd函数

把消息添加到消息队列。

    int msgsnd(int msqid, const void *msg_ptr, size_t msg_sz, int msgflg);
    
消息结构受到两方面的限制：

- 长度必须小于系统规定的上限
- 它必须以一个长整形变量开始

最好把消息定义为下面的样子：

    struct my_message{
        long int message_type;
        /*The data you wish to transfer*/
    }

- msgid

消息队列唯一标识

- msg_ptr

消息结构体指针

- msg_sz

消息结构体长度

- msgflg

消息队列满或达到系统上限时的操作。

- 返回值

成功返回0，失败返回-1. 

## 3.3 msgrcv函数

从消息队列中获取消息

    int msgrcv(int msqid, void *msg_ptr, size_t msg_sz, long int msgtype, int msgflg);
    
- msqid

消息队列唯一标识符

- msg_ptr

指向准备接收消息的指针

- msg_sz

接收消息长度

- msgtype

接收优先级

- msgflg

没有相应类型的消息时发生的事情。

## 3.4 msgctl函数

消息队列控制函数

    int msgctl(int msqid, int cmd, struct msqid_ds *buf);
    
    struct msqid_ds{
        uid_t msg_perm.uid;
        uid_t msg_perm.gid;
        mode_t msg_perm.mode;
    }
    
- msqid

消息队列唯一标识符。

- command

要执行的动作

    IPC_STAT    把msqid_ds结构中的数据设置为消息队列的当前关联值
    IPC_SET     如果进程有足够的权限，就把消息队列的当前关联值设置为msqid_ds结构中给出的值
    IPC_RMID    删除消息队列

- 返回值

成功时返回0，失败时返回-1.

### Demo - msg1.c msg2.c

# 4. IPC状态命令

## 4.1 显示信号量状态

    $ ./ipcs -s
    
删除信号量

    $ ./ipcrm -s <id>
    
## 4.2 显示共享内存状态

    $ ipcs -m
    
删除共享内存

    $ ipcrm -m <id>

## 4.3 显示消息队列状态

    $ ipcs -q
    
删除

    $ ipcrm -q <id>
    
    
