# 标准I/O库

# 1. 流和FILE对象

流定向：区别处理宽窄字节的字符。

## 1.1 设置流定向

    #include <stdio.h>
    #include <wchar.h>
    
    int fwide(FILE *fp, int mode);
    
### 返回值

- 若流是宽定向的，返回正值
- 若流是字节定向的，返回负值
- 若流未定向，返回0

### mode

- 正值，指定流是宽定向
- 负值，指定流是字节定向
- 0，不设置定向。返回当前的定向。

*注意：*fwide并不改变已定向的流的定向。

## 1.2 FILE对象

标准库中，通过FILE对象管理流的相关属性。

# 2. 标准输入、输出和错误

三个标准IO流通过预定义文件指针：stdin,stdout,stderr加以引用。

头文件：stdio.h

# 3. 缓冲

## 3.1 缓冲类型

三种类型：

- 全缓冲

填满缓冲区才执行相应操作。

可以通过执行flush强行冲洗缓冲区（未满的时候）。

*注意：*flush有两种意思：标准I/O中将缓冲区写入磁盘；终端驱动中将缓冲区丢弃。

- 行缓冲

当遇到换行符时执行I/O操作。

- 不带缓冲

标准I/O库不对字符进行缓冲。标准错误流stderr不带缓冲。

## 3.2 更改缓冲类型

    #include <stdio.h>
    
    void setbuf(FILE *restrict fp, char *restrict buf);
    int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
    
### 返回值

- 成功，返回0
- 失败，返回非0

设置之后，通常是全缓冲。如果与终端关联，是行缓冲。
取消缓冲，buf设为NULL

### mode

缓冲类型。

    _IOFBF  全缓冲
    _IOLBF  行缓冲
    _IONBF  不带缓冲
    
![5-1](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/5-1.png)

## 3.3 强制冲洗

    #include <stdio.h>
    
    int fflush(FILE* fp);
    
### 返回值

- 成功：返回0
- 失败：返回EOF

# 4. 打开/关闭流

    #include <stdio.h>
    FILE *fopen(const char *restrict pathname, const char *restrict type);
    FILE *freopen(const char *restrict pathname, const char *restrict type, FILE *restrict fp);
    FILE *fdopen(int fd, const char *type); // 获取一个已有文件的描述符
    
### 返回值

- 成功：file pointer
- 失败：NULL

type指定IO流的读写方式：

![5-2](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/5-2.png)

    #include <stdio.h>
    
    int fclose(FILE* fp);
    
### 返回值

- 成功：0
- 失败：EOF

# 5. 读写流

三种非格式化IO：

- 每次一个字符I/O
- 每次一行I/O
- 直接I/O

## 5.1 输入函数

    #include <stdio.h>
    int getc(FILE *fp);
    int fgetc(FILE *fp);
    int getchar(void);
    
### 返回值

- 成功：下一个字符
- 失败或到达尾部：EOF

## 5.2 判断错误

    #include <stdio.h>
    int ferror(FILE *fp);
    int feof(FILE *fp);
    
### 返回

- 真，返回非0
- 假，返回0
 

清除这两个标志：

    void clearerr(FILE *fp);

压如缓冲区一个字符：

    #include <stdio.h>
    int ungetc(int c, FILE* fp);
    
### 返回值

- 成功，c
- 失败，EOF

## 5.3 输出函数

    #include <stdio.h>
    int putc(int c, FILE *fp);
    int fputc(int c, FILE *fp);
    int putchar(int c);

### 返回值

- 成功，返回c
- 失败，返回EOF

# 6. 每次一行I/O

    #include <stdio.h>
    char *fgets(char *restrict buf, int n, FILE *restrict fp);
    char *gets(char *buf );

    #include <stdio.h>
    int fputs(const char *restrict str, FILE *restrict fp);
    int puts(const char *str);
    
### Ref : P122

# 7. 标准I/O效率

### Ref : P123

# 8. 二进制I/O

    #include <stdio.h>
    size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
    
# 9. 定位流

    #include <stdio.h>
    long ftell(FILE *fp);
    int fseek(FILE *fp, long offset, int whence);
    void rewind(FILE *fp);

    #include <stdio.h>
    off_t ftello(FILE *fp);
    int fseeko(FILE *fp, off_t offset, int whence);

    #include <stdio.h>
    int fgetpos(FILE *restrict fp, fpos_t *restrict pos);
    int fsetpos(FILE *fp, const fpos_t *pos);
    
### Ref : P126

# 10. 格式化I/O

## 10.1 格式化输出

    #include <stdio.h>
    int printf(const char *restrict format, ...);
    int fprintf(FILE *restrict fp, const char *restrict format, ...);
    int dprintf(int fd, const char *restrict format, ...);

### 返回值

- 成功：返回字符串
- 失败：返回负值


    int sprintf(char *restrict buf, const char *restrict format, ...);

### 返回值

- 成功：返回存入数组的字符数
- 失败：返回负值


    int snprintf(char *restrict buf, size_t n,const char *restrict format, ...);
    
### 返回值

- 成功：存入数组的字符数。（如果buf足够大）
- 失败：返回负值

参数形式变体：

    #include <stdarg.h>
    #include <stdio.h>
    int vprintf(const char *restrict format, va_list arg);
    int vfprintf(FILE *restrict fp, const char *restrict format, va_list arg);
    int vdprintf(int fd, const char *restrict format, va_list arg);
    //All three return: number of characters output if OK, negative value if output error

    int vsprintf(char *restrict buf, const char *restrict format, va_list arg);
    // Returns: number of characters stored in array if OK, negative value if encoding error

    int vsnprintf(char *restrict buf, size_t n, const char *restrict format, va_list arg);
    // Returns: number of characters that would have been stored in array if buffer was large enough, negative value if encoding erro

## 10.2 格式化输入

    #include <stdio.h>
    int scanf(const char *restrict format, ...);
    int fscanf(FILE *restrict fp, const char *restrict format, ...);
    int sscanf(const char *restrict buf, const char *restrict format, ...);
    
### 返回值

- 成功：输入项
- 失败：EOF

一组变体：

    #include <stdarg.h>
    #include <stdio.h>
    int vscanf(const char *restrict format, va_list arg);
    int vfscanf(FILE *restrict fp, const char *restrict format, va_list arg);
    int vsscanf(const char *restrict buf, const char *restrict format, va_list arg);
    
# 11. 实现细节

获得FILE的文件描述符。不是ISO C标准库。

    #include <stdio.h>
    int fileno(FILE *fp);

### 返回值

文件描述符

### Demo

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/stdio/buf.c>

# 12. 临时文件

[临时文件](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/TempFile.md)
    
# 13. 内存流

用管理文件流的方式管理内存。

    #include <stdio.h>
    FILE *fmemopen(void *restrict buf, size_t size, const char *restrict type);

### 返回值

- 成功：返回指针
- 失败：返回NULL

### Demo 

<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/SRC_AP/stdio/memstr.c>

其他两个函数：

    #include <stdio.h>
    FILE *open_memstream(char **bufp, size_t *sizep);   // 面向字节
    #include <wchar.h>
    FILE *open_wmemstream(wchar_t **bufp, size_t *sizep); // 面向宽字节

# 14. 标准I/O的替代软件


