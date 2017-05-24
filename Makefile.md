# Makefile

# 1. Makefile的规则

```cpp
target ... :<空格/Tab>prerequisites ...
<Tab>command
...
...
```

## target

目标文件(Object File / 执行文件 / Label)(伪目标)

## prerequisites

要生成那个target所需要的文件或目标

```cpp
myapp: main.o 2.o 3.o
main.o: main.c a.h
2.o: 2.c a.h b.h
3.o: 3.c b.h c.h
```

## command

make需要执行的命令

PS : prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行。

一次创建多个文件，利用伪目标all

    all: myapp, myapp1
    
如果未指定一个all目标，则make只创建找到的第一个目标。

注意：

- 规则必须以Tab开头，空格不行
- makefile每行结尾不能有空格
    
### Demo - Makefile1

```cpp
myapp: main.o 2.o 3.o
    gcc -o myapp main.o 2.o 3.o

main.o: main.c a.h
    gcc -c main.c

2.o: 2.c a.h b.h
    gcc -c 2.c

3.o: 3.c b.h c.h
    gcc -c 3.c
```
# 2. 注释

以\#开头的一行

# 3.宏

定义宏：

    MACRONAME=value
    
引用宏：

    $(MACRONAME) 或 ${MACRONAME}
    

    
- 特殊宏定义

|||
|:-:|:-:|
|$?|当前目标所依赖的文件列表中比当前目标文件还要新的文件|
|$@|当前目标的名字|
|$<|当前依赖文件的名字|
|$*|不包括后缀名的当前依赖文件的名字|
|||    
|-:|忽略所有错误|
|@:|在执行某条命令前不要将该命令显示在标准输出上|

# 4. 通配符

	wildcard	扩展通配符
	notdir		去除路径通配符
	patsubst	替换通配符

EG：
建立一个目录Test，子目录sub，结构如下：

![](http://i.imgur.com/UOejCoE.png)

Makefile：

	src=$(wildcard *.c ./sub/*.c)
	dir=$(notdir $(src))
	obj=$(patsubst %.c,%.o,$(dir) )

	all:
		@echo $(src)
		@echo $(dir)
		@echo $(obj)
		@echo "end"

![](http://i.imgur.com/bTNhugJ.png)

执行结果分析：

![](http://i.imgur.com/FGOIb3R.png)

	src=$(wildcard *.c  ./sub/*.c)	指定目录中全部.c文件
	dir=$(notdir $(src))	把src中的文件去掉路径，只留文件名
	obj=$(patsubst %.c,%.o,$(dir) )	把$(dir)中符合%.c的文件替换成%.o
									相当于：
									obj=$(dir:%.c=%.o)

	path=$(addprefix src/,foo bar)	foo和bar前加上src/
									结果：src/foo src/bar

这里用到makefile里的替换引用规则，即用您指定的变量替换另一个变量。
它的标准格式是

	$(var:a=b) 或 ${var:a=b}

它的含义是把变量var中的每一个值结尾用b替换掉a

# 5. 常用符合

关于$^、$<和$@的用法说明:

	$@	规则中的目标名（也就是规则名）目标的集合，“$@”依次取出目标，并执于命令。
	$^	规则中所有的依赖项目。以空格分隔。如果在依赖目标中有多个重复的，那个这个变量会去除重复的依赖目标，只保留一份。
	$<	规则中的依赖项目,只代表规则所有依赖项目中的第一项
	$? 	规则中时间新于目标的依赖项目

EG:

	Module1_1.o Module1_2.o : Module1_1.cpp Module1_2.cpp
	则
	$@ -- Module1_1.o Module1_2.o
	$^ -- Module1_1.cpp Module1_2.cpp
	$< -- Module1_1.cpp


	= 是最基本的赋值
	:= 是覆盖之前的值
	?= 是如果没有被赋值过就赋予等号后面的值
	+= 是添加等号后面的值
    
# 4. 多个目标

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

# 5. 内置规则

### Demo - Makefile4

# 6. 后缀名和模式规则

    %.o: %.cpp
        $(CC) -xc++ $(CFLAGS) -I$(INCLUDE) -c $<
        
# 7. 用make管理函数库

    .c.a:
        $(CC) -c $(CFLAGS) $<
        $(AR) $(ARFLAGS) $@ $*.o
        
- 第一条规则告诉make必须编译源文件以生成目标文件
- 第二条规则告诉make用ar命令将新的目标文件添加到函数库中

### Demo - makefile5

# 8. Makefile文件和子目录

----

### Demo - MAIN

- Dynamic Lib

<https://github.com/breakerthb/JsonCPPDemo/blob/linux-makefile/lib_json/Makefile>

- Static Lib

- Process File

<https://github.com/breakerthb/JsonCPPDemo/blob/linux-makefile/Makefile>

----

extend

# 4.make是如何工作的

在默认的方式下，也就是我们只输入make命令。

- 1、make会在当前目录下找名字叫“Makefile”或“makefile”的文件。
- 2、如果找到，它会找文件中的第一个目标文件（target），在上面的例子中，他会找到“edit”这个文件，并把这个文件作为最终的目标文件。
- 3、如果edit文件不存在，或是edit所依赖的后面的 .o 文件的文件修改时间要比edit这个文件新，那么，他就会执行后面所定义的命令来生成edit这个文件。
- 4、如果edit所依赖的.o文件也不存在，那么make会在当前文件中找目标为.o文件的依赖性，如果找到则再根据那一个规则生成.o文件。（这有点像一个堆栈的过程）
- 5、当然，你的C文件和H文件是存在的，于是make会生成 .o 文件，然后再用 .o 文件生命make的终极任务，也就是执行文件edit了。

# 5.makefile中使用变量

在上面的例子中，先让我们看看edit的规则：

	edit : main.o kbd.o command.o display.o insert.o search.o files.o utils.o
	cc -o edit main.o kbd.o command.o display.o insert.o search.o files.o utils.o

makefile的变量也就是一个字符串，理解成C语言中的宏可能会更好。

	objects = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

于是，我们就可以很方便地在我们的makefile中以“$(objects)”的方式来使用这个变量了，于是我们的改良版makefile就变成下面这个样子：

	objects = main.o kbd.o command.o display.o insert.o search.o files.o utils.o

	edit : $(objects)
		cc -o edit $(objects)
	main.o : main.c defs.h
		cc -c main.c
	kbd.o : kbd.c defs.h command.h
		cc -c kbd.c
	command.o : command.c defs.h command.h
		cc -c command.c
	display.o : display.c defs.h buffer.h
		cc -c display.c
	insert.o : insert.c defs.h buffer.h
		cc -c insert.c
	search.o : search.c defs.h buffer.h
		cc -c search.c
	files.o : files.c defs.h buffer.h command.h
		cc -c files.c
	utils.o : utils.c defs.h
		cc -c utils.c

	clean :
		rm edit $(objects)

# 6.让make自动推导

只要make看到一个[.o]文件，它就会自动的把[.c]文件加在依赖关系中，如果make找到一个whatever.o，那么whatever.c，就会是whatever.o的依赖文件。并且 cc -c whatever.c 也会被推导出来，于是，我们的makefile再也不用写得这么复杂。

	objects = main.o kbd.o command.o display.o insert.o search.o files.o utils.o
	
	edit : $(objects)
		cc -o edit $(objects)
	
	main.o : defs.h
	kbd.o : defs.h command.h
	command.o : defs.h command.h
	display.o : defs.h buffer.h
	insert.o : defs.h buffer.h
	search.o : defs.h buffer.h
	files.o : defs.h buffer.h command.h
	utils.o : defs.h
	
	.PHONY : clean
	clean :
	rm edit $(objects)

这种方法，也就是make的“隐晦规则”。上面文件内容中，“.PHONY”表示，clean是个伪目标文件。

# 7.另类风格的makefile(不推荐)

即然我们的make可以自动推导命令，那么我看到那堆[.o]和[.h]的依赖就有点不爽，那么多的重复的[.h]，能不能把其收拢起来，好吧，没有问题，这个对于make来说很容易，谁叫它提供了自动推导命令和文件的功能呢？来看看最新风格的makefile吧。

	objects = main.o kbd.o command.o display.o insert.o search.o files.o utils.o
	
	edit : $(objects)
	cc -o edit $(objects)
	
	$(objects) : defs.h
	kbd.o command.o files.o : command.h
	display.o insert.o search.o files.o : buffer.h
	
	.PHONY : clean
	clean :
	rm edit $(objects)

这种风格，让我们的makefile变得很简单，但我们的文件依赖关系就显得有点凌乱了。

- 文件的依赖关系看不清楚
- 如果文件一多，要加入几个新的.o文件，那就理不清楚了。

# 8.清空目标文件的规则

每个Makefile中都应该写一个清空目标文件（.o和执行文件）的规则，这不仅便于重编译，也很利于保持文件的清洁。

	clean:
	rm edit $(objects)

更为稳健的做法是：

	.PHONY : clean
	clean :
	-rm edit $(objects)

**("-" 表示出错也认为是成功)**

# 9. C++环境搭建

## 1.工程目录结构

APL (Alex's Programming Library)

	-Make.properties
	-Makefile(1)
	-include //存放头文件
	   -Module1_1.h
	   -Module1_2.h
	   -Module2_1.h
	   -Module2_2.h
	-src    //存放源文件
	   -Makefile(2)
	   -module1
	    -Module1_1.cpp
	    -Module1_2.cpp
	    -Makefile(3)
	   -module2
	    -Module2_1.cpp
	    -Module2_2.cpp
	    -Makefile(3)
	   -...
  
	-lib   //存放该Project依赖的库文件,型如libxxx.a
	-dist   //存放该Project编译连接后的库文件libapl.a
	-examples //存放使用该“半成品”搭建的例子应用的源程序
	   - Makefile(4)
	   -appdemo1
	    -Makefile(5)
	    -src   //存放应用源代码
	    -include
	    -bin   //存放应用可执行程序
	   -appdemo2
	    -Makefile(5)
	    -src   //存放应用源代码
	    -include
	    -bin   //存放应用可执行程序
	   -...

## 2.Makefile的作用

- Makefile(3)

将其module下的.cpp源文件编译为同名.o文件，同时其phony target "clean"负责删除该目录下的所有.o文件;

- Makefile(2)

调用src目录下所有module的Makefile文件。

- Makefile(1)

先调用src中的Makefile生成静态库文件，然后调用examples中的Makefile构建基于该框架的应用。

- Make.properties

定义通用的目录信息变量、编译器参数变量和通用的依赖关系。(也可命名为*.mk)

## 3.Makefile实现

module1下的Makefile如下：

	#
	# Makefile for module1
	#
	all : Module1_1.o Module1_2.o
	Module1_1.o : Module1_1.cpp
	g++ -c $^ -I ../../include
	Module1_2.o : Module1_2.cpp
	g++ -c $^ -I ../../include
	clean :
	rm -f *.o

module2下的Makefile如下：

	#
	# Makefile for module2
	#
	all : Module2_1.o Module2_2.o
	Module2_1.o : Module2_1.cpp
	g++ -c $^ -I ../../include
	Module2_2.o : Module2_2.cpp
	g++ -c $^ -I ../../include
	clean :
	rm -f *.o

make一下，顺利产生相应的.o文件。

===============================================
建立一个公用的文件来存放一些通用的东西

	# 
	# Properties for demo's Makefile 
	# 
	MAKEFILE = Makefile
	BASEDIR = $(HOME)/proj/demo
	####################
	# Directory layout #
	####################
	SRCDIR = $(BASEDIR)/src
	INCLUDEDIR = $(BASEDIR)/include
	LIBDIR = $(BASEDIRE)/lib
	DISTDIR = $(BASEDIR)/dist
	####################
	# Compiler options #
	#    F_ -- FLAG    #
	####################
	CC = g++
	# Compiler search options
	F_INCLUDE = -I$(INCLUDEDIR)
	F_LIB = -L $(LIBDIR)
	CFLAGS = 
	CPPFLAGS = $(CFLAGS) $(F_INCLUDE)

然后修改一下，各个module中的Makefile文件，以module1为例，修改后如下：

	#
	# Makefile for module1
	#
	include ../../Make.properties
	all : Module1_1.o Module1_2.o
	Module1_1.o : Module1_1.cpp
	$(CC) -c $^ $(CPPFLAGS)
	Module1_2.o : Module1_2.cpp
	$(CC) -c $^ $(CPPFLAGS)
	clean :
	rm -f *.o

其实这两个Makefile中还有一个隐含的重复的地方“后缀规则”来定义通用依赖规则，我在Make.properties加入下面的变量定义：

	####################
	# Common depends   #
	####################
	DEPS = .cpp.o

然后还是以module1为例，修改module1的Makefile后如下：

	#
	# Makefile for module1
	#
	include ../../Make.properties
	all : Module1_1.o Module1_2.o
	$(DEPS):
	$(CC) -c $^ $(CPPFLAGS)
	clean :
	rm -f *.o


/*=================================================
Note: 关于后缀规则的说明
后缀规则中所定义的后缀应该是make 所认识的，如果一个后缀是make 所认识的，那么这个规则就是单后缀规则，而如果两个
连在一起的后缀都被make 所认识，那就是双后缀规则。例如：".c"和".o"都是make 所知道。因而，如果你定义了一个规则是
".c.o"那么其就是双后缀规则，意义就是".c"是源文件的后缀，".o"是目标文件的后缀, ".c.o"意为利用.c文件构造同名.o文件。
==============================================================*/
Makefile(2)文件设计如下：

	#
	# Makefile for src directory
	#
	include ../Make.properties
	TARGET = libdemo.a
	####################
	# Subdirs define #
	####################
	MODULE1_PATH = module1
	MODULE2_PATH = module2
	SUBDIRS = $(MODULE1_PATH) $(MODULE2_PATH)
	####################
	# Objects define #
	####################
	MODULE1_OBJS = $(MODULE1_PATH)/Module1_1.o $(MODULE1_PATH)/Module1_2.o
	MODULE2_OBJS = $(MODULE2_PATH)/Module2_1.o $(MODULE2_PATH)/Module2_2.o
	DEMO_OBJS = $(MODULE1_OBJS) $(MODULE2_OBJS)
	all : subdirs $(TARGET)
	cp $(TARGET) $(DISTDIR)
	subdirs:
	@for i in $(SUBDIRS); do \
	   echo    "===>$$i"; \
	   (cd $$i &&$(MAKE) -f $(MAKEFILE)) || exit 1; \
	   echo    "<===$$i"; \
	done
	$(TARGET) : $(DEMO_OBJS)
	ar -r $@ $^
	clean:
	@for i in $(SUBDIRS); do \
	   echo    "===>$$i"; \
	   (cd $$i &&$(MAKE) clean -f $(MAKEFILE)) || exit 1; \
	   echo    "<===$$i"; \
	done
	rm -f $(DISTDIR)/$(TARGET)

Makefile(1):

	#
	# Makefile for whole project
	#
	include Make.properties
	SRC_PATH = src
	EXAMPLES_PATH = examples
	SUBDIRS = $(SRC_PATH) $(EXAMPLES_PATH)
	all : subdirs
	
	subdirs:
	@for i in $(SUBDIRS); do \
	   echo    "===>$$i"; \
	   (cd $$i && $(MAKE) -f $(MAKEFILE)) || exit 1; \
	   echo    "<===$$i"; \
	done
	clean:
	@for i in $(SUBDIRS); do \
	   echo    "===>$$i"; \
	   (cd $$i && $(MAKE) clean -f $(MAKEFILE)) || exit 1; \
	   echo    "<===$$i"; \
	done

运行一下，由于examples目录下的Makefile还是空的，所以没有成功。
Makefile(5)我设计成这样：

	#
	# Makefile for appdemoX
	#
	include ../../Make.properties
	TARGET = appdemoX
	SRC = ./src/appdemoX.cpp
	all : 
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS) -L $(DISTDIR) -ldemo
	mv $(TARGET).exe ./bin
	clean :
	rm -f ./src/*.o ./bin/$(TARGET).exe

而examples目录下的Makefile(4)的样子如下：

	#
	# Makefile for examples directory
	#
	include ../Make.properties
	EXAMPLE1_PATH = appdemo1
	EXAMPLE2_PATH = appdemo2
	SUBDIRS = $(EXAMPLE1_PATH) $(EXAMPLE2_PATH)
	all : subdirs
	
	subdirs:
	@for i in $(SUBDIRS); do \
	   echo    "===>$$i"; \
	   (cd $$i &&$(MAKE) -f $(MAKEFILE)) || exit 1; \
	   echo    "<===$$i"; \
	done
	clean:
	@for i in $(SUBDIRS); do \
	   echo    "===>$$i"; \
	   (cd $$i &&$(MAKE) clean -f $(MAKEFILE)) || exit 1; \
	   echo    "<===$$i"; \
	done

在Makefile(1)中加上install,tar等目标，使用户得到有更多的功能。



## 1、“=”

make会将整个makefile展开后，再决定变量的值。也就是说，变量的值将会是整个makefile中最后被指定的值。看例子：

	x = foo
	y = $(x) bar
      x = xyz

      在上例中，y的值将会是 xyz bar ，而不是 foo bar 。

## 2、“:=”

“:=”表示变量的值决定于它在makefile中的位置，而不是整个makefile展开后的最终值。

	x := foo
	y := $(x) bar
	x := xyz

在上例中，y的值将会是 foo bar ，而不是 xyz bar 了。

