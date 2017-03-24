# 临时文件和目录

# 1. 临时文件

需要临时的文件保存一些临时信息时使用。

- tmpnam

返回一个不与任何文件同名的有效路径名字符串。每次调用都产生一个不同的路径名。最多调用次数是TMP_MAX

    #include <stdio.h>
    char *tmpnam(char* s);
    
- tmpfile

创建一个临时二进制文件。在关闭文件或程序结束时将自动删除这个文件。给文件命名的同时打开它。

    #include <stdio.h>
    FILE *tmpfile(void);
    

## 实例

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/stdio/tempfiles.c>

# 2. 临时目录

另外两个函数：

    #include <stdlib.h>
    char *mkdtemp(char *template);  // 创建一个目录，名字唯一
    int mkstemp(char *template);    // 创建一个文件，名字唯一
    
## 实例

<https://github.com/breakerthb/LinuxPrograming/tree/master/SRC_AP/stdio/mkstemp.c>