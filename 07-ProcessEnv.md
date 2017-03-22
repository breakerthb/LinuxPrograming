# 进程环境

# 1. 内容

# 2. main函数

原型：

    int main(int argc, char *argv[]);
    
# 3. 进程终止

[进程终止的种类](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/exit.md)

# 4. 命令行参数

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
    
## 4.1 getopt

支持需要关联值和不需要关联值的选项。

    #inclue <unistd.h>
    
    int getopt(int argc, char *const argv[], const char *optstring);
    extern char *optarg;
    extern int optind, opterr, optopt;
    
### Demo : argopt.c

## 4.2 getopt_log

它接受以双划线（--）开始的长参数

### Demo : 

# 5. 环境表

每个程序都接收到一张环境表。

    extern char **environ;
    
每个指针以'\0'结尾，

![7-5](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/7-5.png)

常用getenv和putenv来管理，如果要整个环境变量表，需要访问environ指针。

### Demo

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_LP/04_LinuxEnv/showenv.c>

# 6. C程序的存储空间布局

# 7. 共享库

# 8. 存储空间分配

    #include <stdlib.h>
    void *malloc(size_t size);
    void *calloc(size_t nobj, size_t size);
    void *realloc(void *ptr, size_t newsize);
    // All three return: non-null pointer if OK, NULL on error
    void free(void *ptr);

# 9. 环境变量

    #include <stdlib.h>
    
    char *getenv(const char *name);
    int putenv(cont char *string);
    int setenv(const char *name, const char *value, int rewrite);
    int unsetenv(const char *name);
    
获取和修改环境变量。

### Demo

<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_LP/04_LinuxEnv/environ.c>

# 10. setjmp() longjmp()

    #include <setjmp.h>
    int setjmp(jmp_buf env);
    // Returns: 0 if called directly, non
    void longjmp(jmp_buf env, int val);

### Ref : P170

# 11. getrlimit() setrlimit()

    #include <sys/resource.h>
    int getrlimit(int resource, struct rlimit *rlptr);
    int setrlimit(int resource, const struct rlimit *rlptr);
    // Both return: 0 if OK, −1 on erro


    