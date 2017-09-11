# 1. 变量

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
    
## 1.1 引号

- 单引号中$原样输出
- 双引号中$被认为是变量前缀

test.sh

```cpp
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
```

执行结果：

![Pipe](https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC/0201.png)

## 1.2 环境变量

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

## 1.3 参数变量

|参数变量|说明|
|:--:|:--:|
|$1,$2,...|脚本程序的参数|
|$*|所有参数|
|$@|带分隔符的$*|

# 2. 条件

`test`或`[`命令,用来进行布尔判断

注意：

- `[`后面要留出空格，和`test`命令一样

```cpp
# 检查文件是否存在
if test -f fred.c
then
...
fi
```
- `then`一定要写在下一行，否则要加;

```cpp
# 检查文件是否存在
if [ -f fred.c ]
then
..
fi
```

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

## 参数判断

- 1.判断参数为空

`$1`为第一个参数

```cpp
para=$1

if [ ! $para ]; then
    echo "IS NULL"
else
    echo "NOT NULL"
fi
```

或
    
```cpp  
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
```

- 2.判断参数个数

`$#`参数个数

```cpp
if [ $# == 3 ] ; then
    echo "para number is 3"
else
    echo "para number is not 3"
fi
```

- 3.判断参数相等

字符串比较

```cpp
    if [ "$1" ==  "ABC" ];then
    	# if [ "$1"x =  "ABC"x ];then
        echo "Parameter is ABC"
    else
        echo "Parameter is not ABC"
    fi
```

数字比较

```cpp
if [ $1 = 1 ];then
    echo "Parameter is 1"
else
    echo "Parameter is not 1"
fi
```

# 3. 控制结构

## 3.1 if语句

```cpp
if condition
then
    ...
else
    ...
fi
····

可以加入elif语句

```cpp
if condition; then
    ...
elif
···

### Demo

一个与变量有关的问题

```cpp
if [ $timeofday = "yes" ]
```

这种写法有问题，如果变量为空，则这句话就变成了这样：

```cpp
if [ = "yes"]
```

这样就会报错。正确写法是：

```cpp
if [ "$timeofday" = "yes" ]
```

## 3.2 for语句

```cpp
for variable in values
do
    statements
done
```

固定字符串循环

```cpp
for foo in bar fud 43
do
    echo $foo
done
```

通配符扩展

```cpp
for file in $(ls f*.sh)
do
    lpr $file
done
```

### 循环Demo

- 打印1~9

方法1：

```cpp
for (( i=1; i<10; i++); do
    echo $i
done
```

方法2：

```cpp
for i in {1...9}; do
    echo $i
done
```

方法3：

```cpp
for i in `seq 100`; do
    echo $i
done
```    

## 3.3 while

```cpp
while condition
do
    ...
done
```   

## 3.4 until

```cpp
until condition
do
    ...
done
```

## 3.5 case

```cpp
case variable in
    pattern ) ...;;
    pattern ) ...;;
esac
```

### Demo

```cpp
case "$timeofday" in
    yes) echo "XXX";;
    no)  echo "XXX";;
esac
```    

## 3.6 命令列表

- AND列表（&&）
- OR列表（||）

- 语句块

    {
        ...
    }
    
# 4. 函数

```cpp
function_name () {
    statements
}
```

函数调用

    function_name

这里不用`()`

# 5. 命令

[Shell命令](https://www.zybuluo.com/breakerthb/note/432660)   

[正则表达式](https://www.zybuluo.com/breakerthb/note/428325)

# 6. 命令的执行

    `` 方法
    $(...) 方法
    
建议使用后者

### i. 算术扩展

比expr命令效率更高的方法

    x=0
    x=$(($x+1))

### ii. 参数扩展

    for i in 1 2; do
        process $i_tmp
    done
    
这样写会报错，因为$i_tmp被认为是一个变量，并不存在。正确写法：

    for i in 1 2; do
        process ${i}_tmp
    done
    
变量i的值在脚本中会替换${i},从而产生正确的参数。


常见的参数扩展方法：

|参数扩展|说明|
|:--:|:--:|
|${param:-default}|如果param为空，设为default|
|${#param}|param的长度|
|${param%word}|从param的尾部开始删除与word匹配的最小部分，返回剩余部分|
|${param%%word}|从param的尾部开始删除与word匹配的最长部分，返回剩余部分|
|${param#word}|从param的头部开始删除与word匹配的最小部分，返回剩余部分|
|${param##word}|从param的头部开始删除与word匹配的最长部分，返回剩余部分|


Demo：

```cpp
#!/bin/sh

unset foo
echo ${foo:-bar}

foo=fud
echo ${foo:-bar}

foo=/usr/bin/X11/startx
echo ${foo#*/}
echo ${foo##*/}

bar=/usr/local/etc/local/networks
echo ${bar%local*}
echo ${bar%%local*}
```  

## 3.7 here文档

## 3.8 调试脚本程序

## 3.9 dialog工具