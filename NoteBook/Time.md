# C/C++获取时间方法

### Ref : P151

# 概念

UTC时间 - 格林尼治时间

北京时区是东8区，领先UTC 8个小时

    北京时间 = UTC时间 + 8

# 1. time()

函数获取当前时间距1970年1月1日的秒数，以秒计数单位，存于rawtime 中。

    #include <time.h>
    time_t time(time_t *rawtime);
    
### 返回值

- 成功：返回时间值
- 失败：返回-1

如果rawtime是空指针，直接返回当前时间。如果t不是空指针，返回当前时间的同时，将返回值赋予t指向的内存空间。

时钟类型标识符：

![6-8](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/6-8.png)
    
### Demo

    #include "time.h"
    
    void main ()
    {
        time_t rawtime;
        
        struct tm * timeinfo;
        time ( &rawtime );
        
        timeinfo = localtime ( &rawtime );
        
        printf ( "\007The current date/time is: %s", asctime (timeinfo) );
        
        exit(0);
    }

clock_gettime()函数可用于获取指定时钟的时间。

    #include <sys/time.h>
    int clock_gettime(clockid_t clock_id, struct timespec *tsp);
    
### 返回值

- 成功：返回0
- 失败：返回 −1

# 2. gmtime() | localtime()
	
## 头文件	

    #include <time.h>
	
## 定义函数

    struct tm* gmtime(const time_t* timep);  // UTC时间
    struct tm* localtime(const time_t* timep);  // 当地时间
	
## 函数说明	

gmtime()将参数timep所指的time_t结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果由结构tm返回。

## 结构tm 的定义为 

    struct tm{
        int tm_sec;  //代表目前秒数, 正常范围为0-59, 但允许至61 秒
        int tm_min;  //代表目前分数, 范围0-59
        int tm_hour;  //从午夜算起的时数, 范围为0-23
        int tm_mday;  //目前月份的日数, 范围01-31
        int tm_mon;  //代表目前月份, 从一月算起, 范围从0-11
        int tm_year;  //从1900年算起至今的年数
        int tm_wday;  //一星期的日数, 从星期一算起, 范围为0-6
        int tm_yday;  //从今年1 月1 日算起至今的天数, 范围为0-365
        int tm_isdst;  //日光节约时间的旗标
    };
	
## Demo 
	
### gmtime()

    #include <stdio.h>
    #include <time.h>
    #include <string>
    
    using namespace std;
    
    int main()
    {
        string wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    
        time_t timep;
        time(&timep);
    
        struct tm *p = gmtime(&timep);
    
        printf("%d-%d-%d\n", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
        printf("%s - %d:%d:%d\n", wday[p->tm_wday].c_str(), (p->tm_hour + 8), p->tm_min, p->tm_sec);
    }

执行结果：
	
	2016-12-29
    Thu - 16:31:57

### localtime()

    #include <stdio.h>
    #include <time.h>
    #include <string>
    
    using namespace std;
    
    int main()
    {
        string wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    
        time_t timep;
        time(&timep);
    
        struct tm *p = localtime(&timep); //取得当地时间
    
        printf ("%d-%d-%d\n", (1900+p->tm_year), (1 + p->tm_mon), p->tm_mday);
        printf("%s - %d:%d:%d\n", wday[p->tm_wday].c_str(), p->tm_hour, p->tm_min, p->tm_sec);
    }
	
执行结果：
	
	2016-12-29
    Thu - 8:35:37
	
# 3. ctime()

## 头文件	

	#include <time.h>
	
## 定义函数	

    char *ctime(const time_t *timep);
	
## 函数说明	

将参数timep 所指的time_t 结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果以字符串形态返回。此函数已经由时区转换成当地时间，字符串格式为"Wed Jun 30 21 :49 :08 1993\n"。

## 注意	

若再调用相关的时间日期函数，此字符串可能会被破坏。

## 返回值	

返回一字符串表示目前当地的时间日期。

    #include <stdio.h>
    #include <time.h>

    int main()
    {
        time_t timep;
        time(&timep);
        printf("%s", ctime(&timep));
        return 0;
    }
	
执行：

    Thu Dec 29 08:37:22 2016
	
# 4. asctime

（将时间和日期以字符串格式表示） 

## 头文件	

	#include <time.h>
	
## 定义函数	

	char* asctime(const struct tm* timeptr);
	
## 函数说明	

asctime()将参数timeptr所指的tm结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果以字符串形态返回。此函数已经由时区转换成当地时间，字符串格式为:“Wed Jun 30 21:49:08 1993\n”

## 返回值	

若再调用相关的时间日期函数，此字符串可能会被破坏。此函数与ctime不同处在于传入的参数是不同的结构。

## 附加说明	

返回一字符串表示目前当地的时间日期。

## 范例

    #include <stdio.h>
    #include <time.h>

    int main()
    {
        time_t timep;
        time (&timep);

        struct tm *p = gmtime(&timep);

        printf("%s", asctime(p));
    }
	
执行结果
	
    Thu Dec 29 08:39:14 2016

# 5. settimeofday

## 头文件	

    #include <sys/time.h>
    #include <unistd.h>
	
## 定义函数	

    int settimeofday(const struct timeval *tv, const struct timezone *tz);
	
## 函数说明	

settimeofday()会把目前时间设成由tv 所指的结构信息，当地时区信息则设成tz 所指的结构。详细的说明请参考gettimeofday()。

## 注意	

在Linux下，只有root 权限才能使用此函数修改时间。

## 返回值	

    成功则返回0，失败返回－1，错误代码存于errno。
	
## 错误代码	

    EPERM  并非由root 权限调用settimeofday()，权限不够。
    EINVAL  时区或某个数据是不正确的，无法正确设置时间。
	
# 6. gettimeofday()

（取得目前的时间）

## 表头文件	

    #include <sys/time.h>
    #include <unistd.h>
	
## 定义函数	

    int gettimeofday (struct timeval * tv , struct timezone * tz)
	
## 函数说明	

gettimeofday()会把目前的时间有tv所指的结构返回，当地时区的信息则放到tz所指的结构中。

timeval结构定义为:

    struct timeval{
    	long tv_sec; /*秒*/
    	long tv_usec; /*微秒*/
    };
	
timezone 结构定义为:

    struct timezone{
    	int tz_minuteswest; /*和Greenwich 时间差了多少分钟*/
    	int tz_dsttime; /*日光节约时间的状态*/
    };

上述两个结构都定义在/usr/include/sys/time.h。tz_dsttime 所代表的状态如下

    DST_NONE /*不使用*/
    DST_USA /*美国*/
    DST_AUST /*澳洲*/
    DST_WET /*西欧*/
    DST_MET /*中欧*/
    DST_EET /*东欧*/
    DST_CAN /*加拿大*/
    DST_GB /*大不列颠*/
    DST_RUM /*罗马尼亚*/
    DST_TUR /*土耳其*/
    DST_AUSTALT /*澳洲（1986年以后）*/

## 返回值
	
成功则返回0，失败返回－1，错误代码存于errno。附加说明EFAULT指针tv和tz所指的内存空间超出存取权限。

## 范例

    #include <sys/time.h>
    #include <unistd.h>
    #include <stdio.h>
    
    int main()
    {
        struct timeval tv;
        struct timezone tz;
    
        gettimeofday(&tv , &tz);
    
        printf("tv_sec: %d\n", (int)tv.tv_sec);
        printf("tv_usec: %d\n", (int)tv.tv_usec);
        printf("tz_minuteswest: %d\n", tz.tz_minuteswest);
        printf("tz_dsttime: %d\n",tz.tz_dsttime);
    
        return 0;
    }
	
执行

    tv_sec: 1483000994
    tv_usec: 900691
    tz_minuteswest: 0
    tz_dsttime: 0

# 7. mktime()

（将时间结构数据转换成经过的秒数）

## 表头文件

    #include <time.h>
	
## 定义函数	

    time_t mktime(strcut tm * timeptr);
	
## 函数说明	

mktime()用来将参数timeptr所指的tm结构数据转换成从公元1970年1月1日0时0分0 秒算起至今的UTC时间所经过的秒数。

## 返回值

返回经过的秒数。

## 范例
	
    /* 用time()取得时间（秒数），利用localtime() 转换成struct tm 再利用mktine() 将struct tm转换成原来的秒数*/
    #include <time.h>
    #include <stdio.h>
    int main()
    {
        time_t timep;
        time(&timep);
    
        printf("time() : %d \n", (int)timep);
    
        struct tm *p = localtime(&timep);
        timep = mktime(p);
    
        printf("time()->localtime()->mktime():%d\n", (int)timep);
    
        return 0;
    }

执行
	
	time() : 1483002954 
    time()->localtime()->mktime():1483002954

源文档 <http://www.cppblog.com/deane/articles/118718.html> 

# 8. strftime() strptime()

## 8.1 产生时间字符串

    #include <time.h>
    size_t strftime(char *restrict buf, size_t maxsize, 
                    const char *restrict format, const struct tm *restrict tmptr);
    size_t strftime_l(char *restrict buf, size_t maxsize, 
                    const char *restrict format, const struct tm *restrict tmptr, locale_t locale);

### 返回值

- 成功：存入数组的字符数
- 失败：返回0

### Demo

获取时间字符串

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/datafiles/strftime.c>

## 8.2 字符串时间转换成分解时间

    #include <time.h>
    char *strptime(const char *restrict buf, const char *restrict format,
                    struct tm *restrict tmptr);


# 9. 获取函数执行时间

    // 微妙级
    #include <unistd.h>
    #include <sys/time.h>

    class TimeCal {
    public:
    	inline void Start()
    	{
    		gettimeofday( &start, NULL );
    	}
    
    	inline void End()
    	{
    		gettimeofday( &end, NULL );
    	}
    
    	inline int64_t TimeUsed()
    	{
    		return (1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec);
    	}
    	
    private:
        struct timeval start;
    	struct timeval end;
    };