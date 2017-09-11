# Makefile调试方法

# 1. 增加调试信息

使用info/warning/error增加调试信息。

下面的命令可以放在`makefile`中的任何地方，执行到该函数时，会将`string`信息输出，方便定位make执行到哪个位置。

## 1.1 info

    $(info, "here add the debug info")

但是这个不能打印出.mk的行号


## 1.2 warning 

    $(warning, "here add the debug info")

例如：

```cpp
$(warning A top-level warning)

FOO := $(warning Right-hand side of a simple variable)bar
BAZ = $(warning Right-hand side of a recursive variable)boo

$(warning A target)target: $(warning In a prerequisite list)makefile
$(BAZ)
$(warning In a command script)
ls
$(BAZ):
```

这会产生如下的输出：

```cpp
$ make
makefile:1: A top-level warning
makefile:2: Right-hand side of a simple variable
makefile:5: A target
makefile:5: In a prerequisite list
makefile:5: Right-hand side of a recursive variable
makefile:8: Right-hand side of a recursive variable
makefile:6: In a command script
ls
makefile
```

## 1.3 error

    $(error "error: this will stop the compile")

这个可以停止当前makefile的编译


## 1.4 打印变量的值

    $(info, $(TARGET_DEVICE) )

![](http://img.blog.csdn.net/20150319162516652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2xxaW5nd2Vp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 1.5 echo

使用echo增加调试信息。

*注意：*echo只能在target：后面的语句中使用，且前面是个TAB

- 方法1： 

    @echo "start the compilexxxxxxxxxxxxxxxxxxxxxxx"


- 方法2: 

    @echo $(files)

# 2.命令行选项

有时候，我们不想让我们的makefile中的规则执行起来，我们只想检查一下我们的命令，或是执行的序列。于是我们可以使用make命令的下述参数：

> “-n” “--just-print” “--dry-run” “--recon” 不执行参数，这些参数只是打印命令，不管目标是否更新，把规则和连带规则下的命令打印出来，但不执行，这些参数对于我们调试makefile很有用处。
> 
> “-t” “--touch” 这个参数的意思就是把目标文件的时间更新，但不更改目标文件。也就是说，make假装编译目标，但不是真正的编译目标，只是把目标变成已编译过的状态。
> 
> “-q” “--question” 这个参数的行为是找目标的意思，也就是说，如果目标存在，那么其什么也不会输出，当然也不会执行编译，如果目标不存在，其会打印出一条出错信息。
> 
> “-W <file>;” “--what-if=<file>;” “--assume-new=<file>;” “--new-file=<file>;” 这个参数需要指定一个文件。一般是是源文件（或依赖文件），Make会根据规则推导来运行依赖于这个文件的命令，一般来说，可以和“-n”参数一同使用，来查看这个依赖文件所发生的规则命令。

三个最适合用来调试的命令行选项：

    --just-print（-n）
    --print-database（-p）
    --warn-undefined-variables

## 2.1 --just-print (-n)

查看Makefile展开之后是什么样的，并不真正执行。

在一个新的`makefile`工作目标上，我所做的第一个测试就是以`--just-print（-n）`选项来调用make。这会使得make读进makefile并且输出它更新工作目标时将会执行的命令，但是不会真的执行它们。GNU make有一个方便的功能，就是允许你为将被输出的命令标上安静模式修饰符（@）。

这个选项被假设可以抑制所有命令的执行动作，然而这只在特定的状况下为真。实际上，你必须小心以对。尽管make不会运行命令脚本，但是在立即的语境之中，它会对shell函数

调用进行求值动作。例如：
 
image

正如我们之前所见，_MKDIRS 简单变量的目的是触发必要目录的创建动作。如果这个Makefile 是以--just-print 选项的方式运行的，那么当make 读进Makefile 时，shell命令将会一如往常般被执行。然后，make 将会输出（但不会执行）更新$(objects)文件列表所需要进行的每个编译命令。


常用的方法是：

    $ make -n > build.sh
    $ sh build.sh

这样执行就能够看到每行命令的执行过程。

## 2.2 --print-data-base （-p）

--print-data-base（-p）是另一个你常会用到的选项。它会运行Makefile，显示GNU版权信息以及make 所运行的命令，然后输出它的内部数据库。数据库里的数据将会依种类划分成以下几个组：variables、directories、implicit rules、pattern-specific variables、files（explicit rules）以及vpath earch path。如下所示：


## 2.3 --warn-undefined-variables

这个选项会使得make 在未定义的变量被扩展时显示警告信息。因为未定义的变量会被扩展成空字符串，这常见于变量名称打错而且很长一段时间未被发现到。这个选项有个问题，这也是为什么我很少使用这个选项的原因，那就是许多内置规则都会包含未定义的变量以作为用户自定义值的挂钩。所以使用这个选项来运行make必然会产生许多不是错误的警告信息，而且对用户的makefile 没有什么用处。例如：

    $ make --warn-undefined-variables -n

# 3. --debug选项 (-d)




