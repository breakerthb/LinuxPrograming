# Linux程序设计

|分类|UNIX环境高级编程|||
|:--:|:--:|:--:|:--:|
|UNIX知识体系|第1章|[UNIX基础知识](https://github.com/breakerthb/LinuxPrograming/blob/master/01-Basic.md)||
|[Shell](https://github.com/breakerthb/LinuxPrograming/blob/master/02-Shell.md)|||Shell程序设计| 
|I/O|第3章|[文件I/O](https://github.com/breakerthb/LinuxPrograming/blob/master/03-FileIO.md)|不带缓存的I/O|
||第4章|[文件和目录](https://github.com/breakerthb/LinuxPrograming/blob/master/04-File&Dir.md)|文件、目录|
||第5章|[标准I/O库](https://github.com/breakerthb/LinuxPrograming/blob/master/05-StandardIO.md)||
||第6章|[系统数据文件和信息](https://github.com/breakerthb/LinuxPrograming/blob/master/06-SysDataFile.md)|标准数据文件|
|进程|第7章|[进程环境](https://github.com/breakerthb/LinuxPrograming/blob/master/07-ProcessEnv.md)||
||第8章|[进程控制](https://github.com/breakerthb/LinuxPrograming/blob/master/08-ProcessCtrl.md)||
||第9章|[进程关系](https://github.com/breakerthb/LinuxPrograming/blob/master/09-ProcessRelation.md)||
||第10章|[信号](https://github.com/breakerthb/LinuxPrograming/blob/master/10-Signal.md)||
|线程|第11章|[线程](https://github.com/breakerthb/LinuxPrograming/blob/master/11-Thread.md)||
||第12章|[线程控制](https://github.com/breakerthb/LinuxPrograming/blob/master/12-ThreadCtrl.md)||
||第13章|[守护进程](https://github.com/breakerthb/LinuxPrograming/blob/master/13-Daemon.md)||
||第14章|[高级I/O]()||
||第15章|[进程间通信]()||
||第16章|[网络IPC:套接字]()||
||第17章|[高级进程间通信]()||
||第18章|[终端I/O]()||
||第19章|[伪终端]()||
||第20章|[数据库函数库]()||
|出错处理||||
|用户标识||||

|系统调用和库函数||||
|||Linux环境||  
|||终端||  
|||curses||  
|||数据管理||  
|||MySQL||  
|||||  
|||调试||  
|||[进程和信号](https://github.com/breakerthb/LinuxPrograming/blob/master/11_ProcessAndSignal.md)|system/exec/fork/wait/waitpid/signal/kill/pause/sigaction|  
|||[POSIX线程](https://github.com/breakerthb/LinuxPrograming/blob/master/12_Thread.md)|多线程编程|    
|||[管道](https://github.com/breakerthb/LinuxPrograming/blob/master/13_Pipe.md)||  
|||[进程间通信](https://github.com/breakerthb/LinuxPrograming/blob/master/14_ShareBuf.md)|信号量/共享内存/消息队列|  
|||[套接字](https://github.com/breakerthb/LinuxPrograming/blob/master/15_Socket.md)|Socket|  
|||用GTK+进行GNOME编程||  
|||用Qt进行KDE编程||  
|||Linux标准||  

[GCC使用](https://github.com/breakerthb/LinuxPrograming/blob/master/GCC.md)

[Makefile](https://github.com/breakerthb/LinuxPrograming/blob/master/Makefile.md)

----

# 1. C/C++ 标准库

## 1.1 字符串操作

[strtok()|strtok_s()](https://github.com/breakerthb/LinuxPrograming/blob/master/CStandard/strtok.md) - 字符串分割


# 2. Linux 库

[fork()与vfork()](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/fork.md)  
[wait()与waitpid()](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/wait.md)  
[exec()](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/exec.md)  
[system()](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/system.md)  

[select()](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/select.md)  
[poll()](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/poll.md)  
[epoll()](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/epoll.md)   

[Effective C++](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/EffectiveC.md)   

# 3. Special Subject

[临时文件](https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook/TempFile.md)  
[时间方法](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/Time.md)  
[定时器](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/Timer.md)  

[驱动安装](https://github.com/breakerthb/LinuxPrograming/tree/master/NoteBook/DriverInstall.md)  

## 3.1 Log模块

[Log from unvme](https://github.com/breakerthb/unvme/tree/log)  
[Log from ssdb](https://github.com/breakerthb/CXXLog)  
[boost log](https://github.com/breakerthb/BoostLogUsage)  

## 3.2 参数解析

[gflags参数解析](https://www.zybuluo.com/breakerthb/note/609329)

## 3.3 C++ Server

[Tinyhttpd](https://github.com/breakerthb/Tinyhttpd)  
[Tinyhttpd框架](https://github.com/breakerthb/Tinyhttpd/tree/reuse)

----

# Ref 

## 《Linux/Unix系统编程手册》

[源码](http://www.man7.org/tlpi/code/online/all_files_by_chapter.html)

## 引用路径

|||
|:-:|:-:|
|文章|<https://github.com/breakerthb/LinuxPrograming/blob/master>|  
|Note|<https://github.com/breakerthb/LinuxPrograming/blob/master/NoteBook>|
|Pic|<https://raw.githubusercontent.com/breakerthb/LinuxPrograming/master/PIC>|
|《Linux Programing》源码|<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_LP>|
|《Advanced Programing in the UNIX Environment》源码|<https://github.com/breakerthb/LinuxPrograming/blob/master/SRC_AP>|





