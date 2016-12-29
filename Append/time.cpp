#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <signal.h>

using namespace std;  
void timefunc(int signo)  
{  
    cout<<"signo:"<<signo<<endl;  
    //signal(SIGALRM, timefunc);  
    //alarm(1);  
}  

int gather_time()  
{  
    //时间定时器  
    struct timeval tv;  
    tv.tv_sec = 2;  //tv_sec代表的是秒  
    tv.tv_usec = 0; //tv_usec代表的是微秒（百万分之一秒）精度  
    select(0, NULL, NULL, NULL, &tv);  
  
    //另一种时间定时器  
    /*pthread_mutex_t log_mutex;  
    pthread_cond_t log_check_cond;  
    pthread_mutex_init(&log_mutex, NULL);  
    pthread_cond_init(&log_check_cond, NULL);  
  
    pthread_mutex_lock(&log_mutex);  
    struct timespec timeout;  
    struct timeval tv_start, tv_end;  
    gettimeofday(&tv_start, NULL);  
    timeout.tv_sec = tv_start.tv_sec + 1;    //1s的检测时间  
    timeout.tv_nsec = tv_start.tv_usec * 1000;  
    pthread_cond_timedwait(&log_check_cond, &log_mutex, &timeout);  
    gettimeofday(&tv_end, NULL);  
    cout<<"time(秒):"<<(double)(1000000*(tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec))/1000000<<endl;  
    pthread_mutex_unlock(&log_mutex);     //取队列大小  
    */  
  
    //运行时间间隔  
    double timeuse = 0.0;  
    struct timeval start;  
    struct timeval end;  
    gettimeofday( &start, NULL );  
    sleep(1);             //秒  
    usleep(1000000);      //微秒（百万分之一秒）  
    gettimeofday( &end, NULL );  
    timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;  
    timeuse = (double)timeuse / 1000000;     //单位为秒  
    cout << "timeuse:" << timeuse << endl;  
  
    //时间格式  
    char format[32] = {0};  
    char formatadd[32] = {0};  
    time_t t_now;  
    struct tm tm_time;
    t_now = time(NULL);  
    localtime_r(&t_now, &tm_time);  
    strftime(format, sizeof(format), "%Y%m%d%H%M%S", &tm_time); //日志的时间  
    strftime(formatadd, sizeof(formatadd), "%Y-%m-%d %H:%M:%S", &tm_time);  
    cout << "one:" << format << endl;  
    cout << "two:" << formatadd << endl;  
  
    /*  
    struct timeval {  
      time_t tv_sec;       //秒 [long int]  
      suseconds_t tv_usec; //微秒 [long int]  
    };  
    1秒＝1000毫秒，  
    1毫秒＝1000微秒，  
    1微妙＝1000纳秒，  
    1纳秒＝1000皮秒。  
    秒用s表现,毫秒用ms,微秒用μs表示，纳秒用ns表示，皮秒用ps表示。  
    */  
    struct timeval tp;  
    struct tm tm_t;  
    char time_date[30] = {0};  
    gettimeofday(&tp, NULL);  
    localtime_r(&tp.tv_sec, &tm_time);              //另一种时间计算方法  
    cout << tm_time.tm_year + 1900 << "-" << tm_time.tm_mon + 1 << "-" << tm_time.tm_mday << " " << tm_time.tm_hour << ":" << tm_time.tm_min << ":" << tm_time.tm_sec << endl;  
  
    strftime(time_date, 30, "%Y-%m-%d %H:%M:%S:", &tm_time);  
    sprintf(time_date + 20, "%d", tp.tv_usec);     //带有微妙的时间  
    cout << time_date << endl;  
  
    //计算时间之差  
    time_t first, last;  
    time(&first);  
    sleep(1);  
    time(&last);  
    cout << difftime(last, first) << endl;//返回两个time_t型变量之间的时间间隔  
  
    struct timespec tv_t;  
    tv_t.tv_sec = 0;  
    tv_t.tv_nsec = 1000;    //tv_nsec以纳秒为单位  
    nanosleep(&tv_t, NULL);  
  
    //定时器  
    //signal(SIGALRM, timefunc);  
    //alarm(1);  
  
    //精度较高的定时功能  
    //it_interval指定间隔时间，it_value指定初始定时时间  
    //tv_sec提供秒级精度，tv_usec提供微秒级精度  
    struct itimerval value;  
    value.it_value.tv_sec = 1;  
    value.it_value.tv_usec = 500000;  
    value.it_interval.tv_sec = 1;  
    value.it_interval.tv_usec = 500000;  
    //ITIMER_PROF: 以该进程在用户态下和内核态下所费的时间来计算，它送出SIGPROF信号  
    signal(SIGALRM, timefunc);  
    assert(setitimer(ITIMER_REAL, &value, NULL) == 0); 
    
    while(1)
        sleep(1);  
        
    return 0;  
}  
  
int main()  
{  
    gather_time();  
    return 0;  
}  