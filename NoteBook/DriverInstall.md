# Linux驱动安装

# 1. 驱动加载方式

在Linux下可以通过两种方式加载驱动程序：静态加载和动态加载。

- 静态加载

把驱动程序直接编译进内核，系统启动后可以直接调用。静态加载的缺点是调试起来比较麻烦，每次修改一个地方都要重新编译和下载内核，效率较低。若采用静态加载的驱动较多，会导致内核容量很大，浪费存储空间。

- 动态加载

利用了Linux的module特性，可以在系统启动后用`insmod`命令添加模块（.ko），在不需要的时候用`rmmod`命令卸载模块，采用这种动态加载的方式便于驱动程序的调试，同时可以针对产品的功能需求，进行内核的裁剪，将不需要的驱动去除，大大减小了内核的存储容量。

在台式机上，一般采用动态加载的方式；在嵌入式产品里，可以先采用动态加载的方式进行调试，调试成功后再编译进内核。

# 2. 静态加载

## 2.1 解压内核，修改硬件架构和编译器

将内核压缩文件`linux-2.6.8.1-zzm.tar.bz2`解压到`/home/user/`目录下。解压后得到内核源码目录文件`linux-2.6.8.1-zzm`，进入该目录，编辑`Makefile`文件，将`ARCH`改为`arm`，`CROSS_CPMPILE`改为`arm-linux-`，如下图所示：

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113312.png)

保存后退出。

## 2.2 配置内核

在内核源码树目录下，输入`make menuconfig`命令，进入内核配置界面，进入“Load an Alternate Configuration File”选项，载入配置文件`kernel_2410.cfg`，保存退出，过程如下图所示：

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113313.png)

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113316.png)

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113317.png)

再次输入make menuconfig命令，编辑sound选项，将其编译进内核（*），结果如下图所示，最后保存配置，退出。

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113420.png)

## 2.3 编译内核

在源码树目录下输入`make zImage`命令，编译完成后可以在`/arch/arm/boot/`目录下生成zImage镜像文件。

## 2.4 下载内核

将内核镜像文件zImage下载到开发板上，当串口终端显示如下信息时，表示驱动加载成功。

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113423.png)

# 3. 动态加载

## 3.1 解压内核

过程与静态编译时一样

## 3.2 配置内核

前面过程与静态编译时一样，再次输入命令`make menuconfig`，配置sound选项时，将其编译成模块（M），结果如下图所示，最后保存配置，退出；

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113424.png)

这样就将声卡驱动编译成模块，可以动态选择是否加载到内核中。

## 3.3 下载内核

将内核镜像文件zImage下载到开发板上，验证能否驱动声卡的过程如下：

![](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113425.png)

说明：首先，将虚拟机下的/home/目录挂载到开发板上的/tmp/目录下，然后先后加载soundcore.ko和 s3c2410-oss.ko两个模块，最后通过lsmod命令查看是否加载上声卡驱动，结果显示加载成功，这样就可以在应用空间编程，实现音频的录放等操作。

PS：

①采用make menuconfig命令时，选项*代表Y，表示将驱动编译进内核；M表示将驱动编译成模块；空代表N，表示不编译；

②内核文件与模块两者有很多东西必须匹配，编译器版本、源码版本、编译时的配置等，所以当内核文件修改了，譬如修改了驱动的编译选项（Y、M、N），那么就必须重新编译和下载内核，否则会出错。

# 4. 遇到的问题

问题：动态加载过程中，出现下面错误：

![\\](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113426.png)

错误：注册和注销设备的符号未知。

解决方法：寻找依赖关系，查看几个符号的定义，发现在soundcore.c文件中定义了以上几个函数，同时导出了符号，以register_sound_dsp为例，如下图所示：

![\\](http://www.2cto.com/uploadfile/Collfiles/20140623/2014062309113428.png)

所以应该先加载soundcore.ko，后加载s3c2410-oss.ko。

注意：在Kconfig和Makefile文件中定义了依赖关系，也可以查找到问题的原因。



