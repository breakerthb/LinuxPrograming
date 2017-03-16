# Linux环境

# 1. 程序参数

一个简单的参数处理函数，Demo:args.c

    #include <stdio.h>
    #include <stdlib.h>
    
    int main(int argc, char *argv[])
    {
        int arg;
    
        for(arg = 0; arg < argc; arg++) 
        {
            if(argv[arg][0] == '-')
                printf("option: %s\n", argv[arg]+1);
            else
                printf("argument %d: %s\n", arg, argv[arg]);
        }
        exit(0);
    }
    
## 1.1 getopt

支持需要关联值和不需要关联值的选项。

    #inclue <unistd.h>
    
    int getopt(int argc, char *const argv[], const char *optstring);
    extern char *optarg;
    extern int optind, opterr, optopt;
    
### Demo : argopt.c

## 1.2 getopt_log

它接受以双划线（--）开始的长参数

### Demo : 

# 2. 环境变量

    #include <stdlib.h>
    
    char *getenv(const char *name);
    int putenv(cont char *string);
    
获取和修改环境变量。

### Demo : environ.c

## 2.1 环境变量的用途

## 2.2 environ变量

程序的环境由一组格式为“名字=值”的字符串组成，程序可以通过environ变量直接访问。

    #include <stdlib.h>
    
    extern char **environ;
    
### Demo : showenv.c

# 3. 时间和日期

# 4. 临时文件

    #include <stdio.h>
    
    char *tmpnam(char* s);
    
返回一个不与任何文件同名的有效文件名

    #include <stdio.h>
    
    FILE *tmpfile(void);
    
给文件命名的同时打开它。

### Demo：tmpam.c

# 5. 用户信息

    #include <sys/types.h>
    #include <unistd.h>
    
    uid_t getuid(void); // 返回UID
    char *getlogin(void); // 返回登录名
    
更有效的用户信息接口

    #include <sys/types.h>
    #include <pwd.h>
    
    struct passwd *getpwuid(uid_t uid);
    struct passwd *getpwnam(const char *name);
    
# 6. 主机信息

- gethostname
- uname
- gethostid

# 7. 日志

    #include <syslog.h>
    
    void syslog(int priority, const char *message, arguments ...);
    
# 8. 资源和限制

# 9. 时间和日期

Ref : <https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/Time.md>
    

    