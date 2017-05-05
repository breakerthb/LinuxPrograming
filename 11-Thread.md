# POSIX线程

进程和线程的区别：

- 进程

fork之后，将创建出该进程的副本。这个新进程拥有自己的变量和自己的PID，时间调度独立。几乎完全独立于父进程。

- 线程

新线程拥有自己的栈和局部变量，但与它的创建者共享全局变量、文件描述符、信号处理函数和当前目录状态。

# 1. 线程标识

线程ID是唯一标识：`pthread_t`

## 1.1 比较线程ID

    #include <pthread.h>

    int pthread_equal(pthread_t tid1, pthead_t tid2);

### 返回值

- 相等，非0
- 不相等，0

## 1.2 获取自身线程ID

    #include <pthread.h>

    pthread_t pthread_self(void);
    //Returns: the thread ID of the calling thread 

# 2. 线程创建和终止

- 可重入

在程序所有include之前定义宏：

    #define _REENTRANT
    
- 编译选项

编译过程加上：

    -lpthread
    
## 2.1 pthread_create()

创建线程函数。

    #include <pthread.h>
    
    int pthread_create(pthread_t* thread, pthread_attr_t* attr, 
                    void *(*start_routine)(void*), void* arg);
                    
### 参数

- thread, 线程指针；
- attr, 线程属性，常用NULL；
- start_routine, 线程主体函数指针；
- arg, start_routine函数参数。

### Demo : 打印线程ID

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/threads/threadid.c>

## 2.2 pthread_exit()

线程终止的三种情况：

- 线程正常结束或执行`exit`、`_Exit`或`_exit`，返回值是线程的退出码
- 被统一进程的其他线程取消
- 调用pthread_exit函数

```cpp
#include <pthread.h>    
void pthread_exit(void* retval);
```    

终止调用这个函数的线程，同时用retval返回变量。retval的值可以在pthread_join的第二个参数中得到。注意：retval返回的不能是局部变量，因为线程结束后所有局部变量都被销毁。

## 2.3 pthread_join()

相当于进程中的wait函数。

    #include <pthread.h>
    
    int pthread_join(pthread_t th, void **thread_return);
    
执行后当前线程会一直被阻塞，直到目标线程执行了`pthread_exit`

### 参数

- th, 等待线程的指针
- thread_return, 带回线程返回值

## Demo

- 线程创建与删除

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_LP/12_Thread/demo_thread1.c>

编译：

    $cc -D_REENTRANT demo_thread1.c -o run_thread1 -lpthread

- 获取终止线程的退出码

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/threads/exitstatus.c>

# 3. 其他方法

## 3.1 pthread_cancel()

```cpp
#include <pthread.h>
int pthread_cancel(pthread_t tid);
Returns: 0 if OK, error number on failure
```

## 3.3 pthread_cleanup

```cpp
#include <pthread.h>
void pthread_cleanup_push(void (*rtn)(void *), void *arg);
void pthread_cleanup_pop(int execute);
```

# 4. 线程和进程的关系

![11-6](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/11-6.png)

# 5. 线程同步

两种基本方法：信号量和互斥量

## 5.1 信号量

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


## 5.2 互斥量

```cpp
#include <pthread.h>
int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
//Both return: 0 if OK, error number on failure
```

```cpp
#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
// All return: 0 if OK, error number on failure
```
### Ref:P320

## 5.3 避免死锁

### Ref:P322

## 5.4 pthread_mutex_timedlock

```cpp
#include <pthread.h>
#include <time.h>
int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict tsptr);
//Returns: 0 if OK, error number on failure
```

## 5.5 读写锁

## 5.6 带有超时的读写锁

## 5.7 条件变量

## 5.8 自旋锁

## 5.9 屏障

