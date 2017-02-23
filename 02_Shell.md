# shell程序设计

# 1. 管道和重定向

## 1.1 重定向

- 重定向输出

|||
|:--:|:--:|
|>|输出内容保存到文件|
|>>|输出内容追加到文件|

标准输出和错误输出同时重定向：

    $ kill -1 1234 > /dev/null 2>&1
    
- 重定向输入


    $ more < killout.txt
    
## 1.2 管道

    $ ps > abc.txt
    $ sort abc.txt > pssort.out
    
用管道可以优化为：

    $ ps | sort > pssort.out
    
# 2. 作为程序设计语言的shell

可以在命令行直接执行，也可以写在脚本文件中执行

    #!/bin/sh
    
# 3. shell语法

## 3.1 变量

    $ abc=Hello
    $ echo $abc
    Hello
    
注意：

- 字符串中有空格必须用引号包起来
- 等号两边不能有空格

可以通过read命令从终端给变量赋值

    $ read abc
    123
    $ echo $abc
    123
    
### a. 引号

- 单引号中$原样输出
- 双引号中$被认为是变量前缀

test.sh

    #!/bin/sh

    myvar="Hi there"
    
    echo $myvar
    echo "$myvar"
    echo '$myvar'
    echo \$myvar
    
    echo Enter some text
    read myvar
    
    echo '$myvar' now equals $myvar
    exit 0

执行结果：

![Pipe](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/0201.png)

### b. 环境变量

|环境变量|说明|
|:--:|:--:|
|$HOME|当前用户根目录|
|$PATH|可执行命令搜索列表|
|$PS1|shell提示符格式|
|$PS2|二级提示符，通常是>|
|$IFS|输入域分隔符，可以是空格、制表符和换行符|
|$0|脚本文件名|
|$#|传递给脚本的参数个数|
|$$|shell脚本进程号|

### c. 参数变量

|参数变量|说明|
|:--:|:--:|
|$1,$2,...|脚本程序的参数|
|$*|所有参数|
|$@|带分隔符的$*|

## 3.2 条件

- test或[命令

用来进行布尔判断

EG ：检查文件是否存在

    if test -f fred.c
    then
    ...
    fi
    
或

    if [ -f fred.c ]
    then
    ..
    fi
    
注意：

- [后面要留出空格，和test命令一样
- then一定要写在下一行，否则要加;

|字符串比较||
|:--:|:--:|
|str1 = str2||
|str2 != str2||
|-n str|字符串不为空结果为真|
|-z str|字符串为null结果为真|
|算术比较||
|a -eq b|相等结果为真|
|a -ne b|不等结果为真|
|a -gt b|相等于a > b|
|a -ge b|相当于a >= b|
|a -lt b|相当于a < b|
|a -le b|相当于a <= b|
|! a|相等于 !a 注意空格|
|文件条件测试||
|-d file|判断目录|
|-e file|判断文件存在|
|-f file|判断是普通文件|
|-g file|判断set-group-id被设置|
|-r file|判断文件可读|
|-s file|判断文件大小不为0|
|-u file|判断set-user-id被设置|
|-w file|判断文件可写|
|-x file|判断文件可执行|

### Demo

- 1.判断参数为空

$1为第一个参数

        para=$1
        
        if [ ! $para ]; then
            echo "IS NULL"
        else
            echo "NOT NULL"
        fi
    
或
    
    
        if [ -z $1 ];then
            echo "-----------------------------------------------"
            echo "If you want to build the sysbench"
            echo "Please input the parameter : sysbench"
            echo "And if you want to build the trxtest"
            echo "Please input the parameter : trxtest"
            echo "Eg:"
            echo "    ./build.sh sysbench or ./build.sh trxtest"
            echo "-----------------------------------------------"
            
            exit 0
        fi


- 2.判断参数个数


    if [ $# == 3 ] ; then
        echo "para number is 3"
    else
        echo "para number is not 3"
    fi

- 3.判断参数相等

字符串比较

    if [ "$1" ==  "ABC" ];then
    	# if [ "$1"x =  "ABC"x ];then
        echo "Parameter is ABC"
    else
        echo "Parameter is not ABC"
    fi

数字比较

    if [ $1 = 1 ];then
        echo "Parameter is 1"
    else
        echo "Parameter is not 1"
    fi

## 3.3 控制结构

### a. if语句

    if condition
    then
        ...
    else
        ...
    fi

### b. elif语句

    if condition; then
        ...
    elif

### c. 一个与变量有关的问题

    if [ $timeofday = "yes" ]
    
这种写法有问题，如果变量为空，则这句话就变成了这样：

    if [ = "yes"]
    
这样就会报错。正确写法是：

    if [ "$timeofday" = "yes" ]
    
### d. for语句

    for variable in values
    do
        statements
    done
    
- 固定字符串循环


    for foo in bar fud 43
    do
        echo $foo
    done
    
- 通配符扩展


    for file in $(ls f*.sh)
    do
        lpr $file
    done
    
- Demo

打印1~9

方法1：

    for (( i=1; i<10; i++); do
        echo $i
    done
    
方法2：

    for i in {1...9}; do
        echo $i
    done
    
方法3：

    for i in `seq 100`; do
        echo $i
    done
    
### e. while

    while condition
    do
        ...
    done
    
### f. until

    until condition
    do
        ...
    done
    
### g. case

    case variable in
        pattern ) ...;;
        pattern ) ...;;
    esac
    
eg

    case "$timeofday" in
        yes) echo "XXX";;
        no)  echo "XXX";;
    esac
    
### h. 命令列表

- AND列表（&&）
- OR列表（||）

### i. 语句块

    {
        ...
    }
    
## 3.4 函数

    function_name () {
        statements
    }
    
## 3.5 命令

[Shell命令](https://www.zybuluo.com/breakerthb/note/432660)   

[正则表达式](https://www.zybuluo.com/breakerthb/note/428325)

## 3.6 命令的执行


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
# 常用Script
    
打印当前目录

    for i in `ls -a`
    do
          echo $i
    done

2. while
常用
1.Every 100 seconds run a time


    time1=`date +%s%N|cut -c1-13`
    while true; do
        time2=`date +%s%N|cut -c1-13`
        time_gap=$((${time2}-${time1}))
        if [ $time_gap -gt 100 ]; then
            time1=$time2
            ...
        else
            ...
        fi
    done

2. 打印时间

  para=$1

  curtime=`date "+%Y-%m-%d %H:%M:%S"`

  if [ ! $para ]; then
	          echo "Current Time : $curtime"
  elif [ "$para" == "start"  ];then
	         echo "Start Time : $curtime"
  elif [ "$para" == "end"  ];then
	          echo "End Time : $curtime"
  else
          echo "Current Time : $curtime"
  fi

3. 在所有子目录的Makefile中替换字串

sed -i 's/XX = g++/XX = g++ -pg/g' `find | grep "Makefile$"` 

sed -i 's/XX = g++/XX = g++ -DMULTI_STREAM/g' Makefile 

4. 等待程序执行完再继续

while(`pgrep -f prepare1 | wc -l`)
do
        sleep 1
done

# 等待prepare1执行完后再继续后面的代码

6. echo不换行

echo -e "please input a value:\c"

7.循环

for i in `seq 10000` #seq 产生1~10000
do

done