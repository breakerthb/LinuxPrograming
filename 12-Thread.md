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

