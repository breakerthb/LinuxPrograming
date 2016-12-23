# 开发工具 Makefile

# 1. make命令和makefile文件

## 1.1 makefile语法

通过规则描述如何通过依赖文件创建目标

make通过读取Makefile文件创建文件。

## 1.2 make的选项和参数

    -k 发现错误仍然进行
    -n 输出操作步骤
    -f 指定makefile文件
    
### 1.2.1 依赖关系

    myapp: main.o 2.o 3.o
    main.o: main.c a.h
    2.o: 2.c a.h b.h
    3.o: 3.c b.h c.h
    
注意格式：

    目标名：<空格/Tab><依赖列表>
    
一次创建多个文件，利用伪目标all

    all: myapp, myapp1
    
如果未指定一个all目标，则make只创建找到的第一个目标。

### 1.2.2 规则

规则定义了目标的创建方式。

注意：

- 规则必须以Tab开头，空格不行
- makefile每行结尾不能有空格

### Demo - Makefile1

    myapp: main.o 2.o 3.o
    	gcc -o myapp main.o 2.o 3.o
    
    main.o: main.c a.h
    	gcc -c main.c
    
    2.o: 2.c a.h b.h
    	gcc -c 2.c
    
    3.o: 3.c b.h c.h
    	gcc -c 3.c

## 1.3 makefile文件注释

以#开头的一行

## 1.4 makefile文件中的宏

定义宏：

    MACRONAME=value
    
引用宏：

    $(MACRONAME)
    
或

    ${MACRONAME}
    
### Demo - Makefile2

    all: myapp
    
    # Which compiler
    CC = gcc
    
    # Where are include files kept
    INCLUDE = .
    
    # Options for development
    CFLAGS = -g -Wall -ansi
    
    # Options for release
    # CFLAGS = -O -Wall -ansi
    
    
    myapp: main.o 2.o 3.o
    	$(CC) -o myapp main.o 2.o 3.o
    
    main.o: main.c a.h
    	$(CC) -I$(INCLUDE) $(CFLAGS) -c main.c
    
    2.o: 2.c a.h b.h
    	$(CC) -I$(INCLUDE) $(CFLAGS) -c 2.c
    
    3.o: 3.c b.h c.h
    	$(CC) -I$(INCLUDE) $(CFLAGS) -c 3.c
    
- 特殊宏定义

    $?  当前目标所依赖的文件列表中比当前目标文件还要新的文件
    $@  当前目标的名字
    $<  当前依赖文件的名字
    $*  不包括后缀名的当前依赖文件的名字
    
    -:  忽略所有错误
    @:  在执行某条命令前不要将该命令显示在标准输出上。
    
## 1.5 多个目标

### Demo - Makefile3

    all: myapp
    
    # Which compiler
    CC = gcc
    
    # Where to install
    INSTDIR = /usr/local/bin
    
    # Where are include files kept
    INCLUDE = .
    
    # Options for development
    CFLAGS = -g -Wall -ansi
    
    # Options for release
    # CFLAGS = -O -Wall -ansi
    
    
    myapp: main.o 2.o 3.o
    	$(CC) -o myapp main.o 2.o 3.o
    
    main.o: main.c a.h
    	$(CC) -I$(INCLUDE) $(CFLAGS) -c main.c
    
    2.o: 2.c a.h b.h
    	$(CC) -I$(INCLUDE) $(CFLAGS) -c 2.c
    
    3.o: 3.c b.h c.h
    	$(CC) -I$(INCLUDE) $(CFLAGS) -c 3.c
    
    
    clean:
    	-rm main.o 2.o 3.o
    
    install: myapp
    	@if [ -d $(INSTDIR) ]; \
    	then \
    		cp myapp $(INSTDIR);\
    		chmod a+x $(INSTDIR)/myapp;\
    		chmod og-w $(INSTDIR)/myapp;\
    		echo "Installed in $(INSTDIR)";\
    	else \
    		echo "Sorry, $(INSTDIR) does not exist";\
    	fi
    
添加了一个install选项和clean选项

注意：

    -rm 表示忽略rm的执行结果
    install依赖myapp，先生成myapp才能安装。
    由于执行规则时启动一个shell，因此需要用反斜杠让它们理论上是一行。

## 1.6 内置规则

### Demo - Makefile4

## 1.7 后缀名和模式规则

    %.o: %.cpp
        $(CC) -xc++ $(CFLAGS) -I$(INCLUDE) -c $<
        
## 1.8 用make管理函数库

    .c.a:
        $(CC) -c $(CFLAGS) $<
        $(AR) $(ARFLAGS) $@ $*.o
        
- 第一条规则告诉make必须编译源文件以生成目标文件
- 第二条规则告诉make用ar命令将新的目标文件添加到函数库中

### Demo - makefile5

## 1.9 Makefile文件和子目录


