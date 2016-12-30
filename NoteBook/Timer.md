# 定时器的实现方法

# 1. select

通过select的超时功能实现定时器。有效防止忙等待。

ref : <https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket.md>

    struct timeval tv;  
    tv.tv_sec = 2;  //tv_sec代表的是秒  
    tv.tv_usec = 0; //tv_usec代表的是微秒（百万分之一秒）精度  
    select(0, NULL, NULL, NULL, &tv);

## Demo

<https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket/select_timer.c>

   
# 2. pthread_cond_timedwait

与1中类似，利用线程等待的超时方法也可以实现。