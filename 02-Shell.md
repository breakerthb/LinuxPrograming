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

[Shell Script](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/poll.md)

# 4. 常用Script
    
## 4.1 打印当前目录

```cpp
for i in `ls -a`
do
      echo $i
done
```

## 4.2 Every 100 seconds run a time

```cpp
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
```

## 4.3 打印时间

```cpp
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
```

## 4.4. 在所有子目录的Makefile中替换字串

```cpp
sed -i 's/XX = g++/XX = g++ -pg/g' `find | grep "Makefile$"` 
sed -i 's/XX = g++/XX = g++ -DMULTI_STREAM/g' Makefile 
```

## 4.5 等待程序执行完再继续

```
while(`pgrep -f prepare1 | wc -l`)
do
    sleep 1
done
```

等待prepare1执行完后再继续后面的代码

## 4.6. echo不换行

```cpp
echo -e "please input a value:\c"
```