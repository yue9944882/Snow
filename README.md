#项目目标 Major Functionality#

开发环境： 	CentOS7-Qt3.3



实现一个基于LINUX的多线程下载器，功能上仿造迅雷，主要有如下几个功能：

---


	1.（仅限HTTP协议）多线程下载远程资源
	2.（暂停/继续功能）断点续传
	3.后端日志系统
	4.系统网络速度可视化
	5.可移植安装（需要qt3.3.x及以上版本）


---

	
	-- 主要技术 --
	
	POSIX线程及其协作

	TCP协议套接字编程

	Qt界面实现

	Qt 信号槽机制SIGNAL/SLOT

	Linux信号处理





---


#项目架构#


#####整体架构 如下图所示：#####

![](http://files.cnblogs.com/files/guguli/SNOW-ARCH.gif)


贯穿整个使用过程的Qt的主界面对全局的若干个队列进行操作，从而实现系统协作，其过程中使用若个同步锁调配线程之间的协作与竞争。

**为什么这么设计？**

这么设计最大的原因是为了实现POSIX线程与Qt封装类之间的互动，POSIX线程是基于LINUX下的C语言实现的，其调用创建的入口必须是C-style声明的函数，如果直接将这些函数声明为Qt控件继承类内静态函数会破坏其封装性（实践证明，这么做也是不可行的）。Qt主界面窗口使用Compositor模式嵌套其他类，在类方法里通过一些POSIX全局锁与全局变量互动虽然会损失一些效率，但是换来了稳定性。


**为什么使用POSIX线程？**

Qt是可移植的项目环境，如果使用linux下独有的FORK/VFORK系列函数，会局限程序运行环境，POSIX标准下的线程更通用，更广泛。




#####网络及动态显示控件部分架构 如下图所示:#####

![](http://files.cnblogs.com/files/guguli/SNOW-ARCH-NET.gif)


---
#####POSIX锁类架构 如下图所示：#####
![](http://files.cnblogs.com/files/guguli/SNOW-MUTEX.gif)


#####代码结构:#####

	--- Declaration ------  global.h     ：extern声明外部全局变量
	|            |
	|             ---  global_t.h   ：C风格结构体定义，主要用于POSIX线程传参
	|            |
	|             ---  global_f.h   ：extern声明外部全局函数
	|            |
	|             ---  missionbar.h  missioncheck.h   ：C++风格声明动态控件
	|            |
	|             ---  mainwindow.h  newdialog.h   ：C++风格声明静态控件
	|
	|
	|
	--- Defination  -----  global.cpp   ：全局变量定义
	|            |
	|             ---  global_f.cpp  ：POSIX线程及日志系统- C风格函数定义
	|            |
	|             ---  main.cpp    ：程序入口，程序环境初始化..
	|            |
	|             ---  *.cpp       ：Qt库继承类定义
	|
	|
	--- Makefile   ------  SnowLINUX.pro   ：QMake脚本
             |
              ---  Makefile         ：自动化编译脚本


---


#####代码实现####


######网络下载部分######


- 1. TCP套接字：

由于多线程下载器需要保证文件的完整性，我们选择TCP协议下的套接字进行下载，每次下载首先发送一个HTTP-HEAD请求得到文件的长度和完整名称并且声明不使用gzip格式压缩，否则无法多线程写入！得到了文件完整的长度之后，根据线程数量为文件划分段落，然后由若干个POSIX线程使用随机上长的端口号和服务器80号端口进行TCP连接，再使用Linux内核提供的接口进行下载，每个任务维护一个更新写入进程锁，以更新当前进度/速度的实时信息，并竞争全局锁刷新全局统计变量。


- 2. 无锁数据结构：

使用Linux的原子文件读/写操作而不是标准文件操作，以保证多线程写入的原子性和完整性，pwrite/pread函数是我们的最终选择，由于读写原子性，文件描述符不需要锁类保护同步性。


- 3. 日志系统

日志系统原本的设计是通过Linux-signal库进行定时的任务日志更新，但是这样给CPU带来了太多额外的任务消耗，下载的速度也会造成不同程度减少，更重要的是这样实现会破坏Qt继承类的封装性，因为signal_handler风格的函数必须是全局函数。我们采用的是”单次”日志，在用户有需要的时候触发日志记录系统，相关函数见 global_f.cpp中的：

Init_log(),write_log(),read_log()系列函数，日志格式暂不赘述，为纯ASCii编码文件。



Qt类与全局锁部分

- 1. 全局竞争锁：

时间( timeMutex )锁是为了主界面定时调用函数在获取任务队列中不断异步更新进度的任务的执行时间，时间锁是为了保证主界面和动态控件之间数据的同步，其中动态控件的数据由动态控件封装的锁保证其内部POSIX线程刷新数据的同步性。

表锁(tableMutex)是为了保证我们在进行删除/重启任务而导致全局表修改的同时，程序不会因为定时刷新曲线而错误访问任务表导致的程序崩溃。

状态锁( finishMutex )是为了防止动态控件和任务表之间相互通过记录对方索引而相互访问时表内容修改导致的程序崩溃。

- 2. 全局协作锁：

在启动新任务的时候弹出的小QDialog窗口是个独立的窗口，其内部空间的SIGNAL分别绑定到了主界面的槽（SLOT）和动态控件的槽中，然而两边的槽默认是同时进行调用的，而我们必须要求其顺序，不然程序会崩溃。例如，在我们选择文件路径后开始下载任务，我们需要先调用主界面的槽来更新表做预备工作，然后才能创建动态控件及下载任务.


###程序操作###


由于时钟的异步及CPU时钟计算原理，曲线图和任务栏的速度会有差别（大多数情况下是慢了1~2 sec）
