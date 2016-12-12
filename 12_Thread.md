# POSIX线程

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

## 2.1 信号量


## 2.2 互斥量

# 3. 线程属性

# 4. 取消一个线程

# 5. 多线程

