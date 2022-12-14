# 一个在裸机上运行的俄罗斯方块

这个项目是很大程度上是自制操作系统的皮毛，主要目的是在裸机上运行俄罗斯方块，涉及到操作系统与硬件的基本交互，但是不包含操作系统的核心技术比如进程和内存管理。

本项目按任务分为以下阶段：

- 引导扇区的创建，打印hello world
- 由实模式进入保护模式
- 在裸机运行C语言程序
- 在VGA模式下显示内容
- 实现中断的支持
- Tetris

# 引导扇区的创建，打印hello world

计算机启动后，主板上内置的BIOS程序首先运行，BIOS进行一系列自检后从外部存储设置中检查可引导扇区，如果存在可引导扇区那么将引导扇区加载到内存地址`0x7c00`处，并跳转到该内存地址开始执行。

本节内容需要的知识：
- Intel或者AT&T汇编语言
- 汇编语言编译器和链接器工具的使用
- 理解并使用BIOS中断
- Bochs基本使用
- Makefile的基本使用

## 可引导扇区的结构

可引导扇区主要特征如下：
- 外部存储设备的第一个扇区，也就是0号扇区
- 一个扇区大小为512字节，并且以`0xaa55`为结尾（以0号字节为起始，即510字节为0x55，511字节为0xaa）

## 使用BIOS中断清屏以及打印hello world

使用`int 0x10`来调用BIOS中断打印字符，其参数和功能通过寄存器来传递，详情参见代码文件`boot.asm`。

## Makefile自动化构建以及Bochs调试

Makefile提供构建脚本，注意`boot.asm`汇编的链接参数，`-Ttext=0x7c00`表示程序装在到内存的位置，`--oformat binary`表示生成数据的原始二进制Blob，而不是elf格式。

在Bochs中则指定了BIOS和VGABIOS，并且设置机器的物理内存大小和机器的外接存储设备。

# 由实模式进入保护模式

在上一节我们打印Hello World是在实模式下实现的，由于实模式下可访问内存受限，因此我们要切换到保护模式。有关于实模式、保护模式以及64位时代的长模式的介绍请自行搜索理解。

进入保护模式需要执行以下几个操作：

- 加载GDT
- 打开A20地址线
- 修改CR0寄存器

接下来逐一介绍。

## 加载GDT

GDT（全局描述符表）是指导CPU如何进行分段数据结构，有关什么是分段请参考操作系统内存管理章节。GDT位于内存中，有着固定的数据结构格式，CPU通过GDTR寄存器来获取GDT表的内存位置。

有关GDT的详细格式可以参考《Linux内核完全解析 5.0》一书中的介绍。

在这部分我们要做的是
1. 设置GDT表
2. 设置GDTR寄存器

具体内容`boot.asm`参考代码中`gdt`关键词的部分。

## 打开A20地址线

关于A20地址线的历史情况，在《Linux内核完全解析 5.0》的 6.3.34 介绍的非常详细。

A20地址线的开启有两种代码，一种是传统的方法，一种则是FAST A20。关于两种方法的操作请参考[https://wiki.osdev.org/A20#Enabling](https://wiki.osdev.org/A20#Enabling)。这里采取的是FAST A20的做法。

## 修改CR0寄存器

通过修改CR0开启保护模式，主要是针对PE位做修改，当为1时即开启保护模式。在设置完毕CR0后需要使用JMP指令刷新指令队列。

## 测试功能

在开启保护模式之后，程序可以访问到多于1MB的空间。通过对VRAM显存的设置来在屏幕上打印字符，从而测试保护模式是否成功运行。或者，我们可以通过Bochs的调试命令，查看CPU当前的模式。

## 参考资料

- 《Linux内核完全解析 5.0》
- [https://wiki.osdev.org/A20](https://wiki.osdev.org/A20)
- [https://en.wikipedia.org/wiki/Global_Descriptor_Table#GDT_example](https://en.wikipedia.org/wiki/Global_Descriptor_Table#GDT_example)

# 在裸机运行C语言程序

在裸机上运行比汇编更高级的语言是开发的需要，固然汇编能够完成，但是没有必要那么难为自己。我们在裸机上运行C语言需要解决的问题有以下几个。

- C语言要编译
- 要把程序从硬盘读到内存中
- 要把执行权交给C语言程序

## C语言程序编写

在日常编写用户程序时，会引用很多的“外部”函数，但是在裸机上，由于这些函数没有被实现，所以不能使用。一定要使用这些函数的情况下，这些函数需要手动实现，可能会存在这C语言如何调用汇编代码的问题，这里需要学习。

具体编译命令可以查看项目中的Makefile代码。

## 把程序从硬盘读到内存中

关于如何把程序装在到“硬盘”上，可以依赖`dd`命令来实现。在加载C语言之前，只能依赖汇编编写程序读取硬盘内容。这里的代码摘自
[https://www.cnblogs.com/flashsun/p/12235761.html](https://www.cnblogs.com/flashsun/p/12235761.html)。

## 把执行权交给C语言程序

这部分就要利用长跳转函数来实现，在上面保护模式的内容中有提及，这里要注意，我们在跳转C语言程序之前，一定要把对应的段寄存器设置好，比如说ES、DS、SS。还有就是确定栈的起始位置，因为我们当前执行的代码位置在0x7c00靠后的部分（根据代码内容有所不同），由于在C语言模型中，栈是从内存高地址向低地址生长的，在没有设置合适的栈位置时很容易会把前面引导扇区在内存中的内容覆盖掉，尤其严重的是覆盖掉GDT。

关于要跳转的位置，可以采用ida64等软件查看入口函数的偏移位置来计算得到，这里的入口可以是`__start`函数，或者通过编译选项设置的入口。

## 总结

本章的代码量很少，只是基础的引入C语言环境，后续会以C语言为主，汇编为辅来实现具体的俄罗斯方块功能。关于后续的业务代码，不做过多的讲解，主要讲述业务代码中涉及到与机器交互的部分，以以下几个内容为主

- 图像显示
- 控制游戏速度的sleep函数以及键盘按键的检测

# 在VGA模式下显示内容

本章主要简单说明一下如何在裸机上显示图像，开发者可以根据实际需要自行选择VGA或者VESA或者其他的显示模式，在这里使用VGA模式来讲解。

接下来要分别实现：

- VGA模式下图像的显示
- VGA模式下文字的显示

## VGA模式下图像的显示

VGA模式有两个工作环境，一个是图像模式，一个是文字模式，进入图像模式需要在boot汇编代码中通过BIOS中断来实现。图像模式整个屏幕的显示情况是与内存中0x0A0000其实的地址做映射，VGA模式下支持256色 320*200的画面，通过对该段内存的写入和修改就能实现对应图像的显示。

## VGA模式下文字的显示

在VGA进入图像模式后，打印文字的方法就是将文字作为图画画出来。这需要设置ASCII字符点阵字库，参考这篇文章[ASCII字符点阵字库的制作和使用](https://shaocheng.li/posts/2011/06/08/)。

## 实战

这部分的代码在`lib.c`里实现，主要有打印字符、打印字符串以及画方块三个函数组成。具体的ASCII字符在ascii_font.h里定义。开启VGA图像模式在`boot.asm`里实现。

这部分主要是C语言编程，代码编写难度不大，重点在理解图像显示。

# 实现中断的支持

关于实现定时控制游戏速度的sleep函数以及键盘按键的检测就需要编写中断的处理函数，在计算机组成原理这门课程中应该有提及中断的问题。

这章的内容有点多，主要内容有以下：

- 中断功能的实现
- sleep函数的实现
- 键盘按键监测的实现

在该阶段的实现过程中，遇到问题

## 中断功能的实现

中断功能的实现主要分两部分，一部分是向机器注册中断服务程序，另一部分是开中断。

### 向机器注册中断服务程序

注册中断服务程序的实现与分段的实现很类似。中断依赖IDT存储每个中断对应的中断服务程序ISR的情况，与GDT十分类似，具体的IDT结构可以查看[https://wiki.osdev.org/IDT](https://wiki.osdev.org/IDT)。在设置完IDT后，需要设置idtr寄存器来向CPU表明对应的IDT位置，这点与GDT相同。

关于IDT和idtr寄存器的内容在代码里体现，不多做赘述。一个难点在与如何编写ISR，不同于函数在返回时调用`ret`指令，ISR需要调用`iret`指令，并且有保护现场的需求。同时值得注意的是，针对与中断服务来说，在中断服务的具体工作执行完毕后，需要发送消息表明中断已经被接收，否则中断不会被继续发送。有关于ISR的编写技术参考这里[https://wiki.osdev.org/Interrupt_Service_Routines#x86](https://wiki.osdev.org/Interrupt_Service_Routines#x86)。

### 开中断

开中断则需要对PIC芯片进行编程，当然现代计算机也会有APIC等芯片作为替代，这里以PIC为主。在开启保护模式的阶段中，我们调用了`cli`指令关中断，对应的，我们需要调用`sti`来开中断。PIC芯片（Programmable Interrupt Controller）作为中断的控制器，要对其进行设置，关于这里代码摘抄自[https://www.cnblogs.com/flashsun/p/12334973.html](https://www.cnblogs.com/flashsun/p/12334973.html)。具体PIC的功能和端口请参考[https://wiki.osdev.org/PIC](https://wiki.osdev.org/PIC)。

关于PIC的编程需要理解PIC的的具体操作方法，这里简化为仅对0到7号中断进行允许，有关于更多的PIC编程内容，后面有机会单独了解。

另外，有关于PIC编程利用了在`io.asm`中定义的`write_to_port`和`read_from_port`两个函数，这里的技术是如何让C语言调用汇编函数，根据架构不同，其参数和返回值的位置也不仅相同，这里采用的是i386的方法，也就是参数和返回值被放入栈中。具体内容可以参见代码以及Makefile文件。

**更正**

**代码中这里存在问题**
```c
    // 设置PIC使其允许该类型中断
    uint8_t mask = read_from_port(PIC_M_DATA);
    mask = mask | (0x1 << irq_code);
    write_to_port(PIC_M_DATA, mask);
```
**应该改为**
```c
    // 设置PIC使其允许该类型中断
    uint8_t mask = read_from_port(PIC_M_DATA);
    mask = mask & ~(0x1 << irq_code);
    write_to_port(PIC_M_DATA, mask);
```

## sleep函数的实现

sleep函数需要时钟中断，在上一部分中已经设置好了中断系统，现在要定时产生时钟中断，需要对中断源PIT进行编程。相关资料可以参考这两个网页内容：
- [https://0cch.com/2013/08/04/e4bdbfe794a8e58fafe7bc96e7a88be997b4e99a94e5ae9ae697b6e599a8programmable-interval-timere7bc96e58699e7b3bbe7bb9fe697b6e9929f/](https://0cch.com/2013/08/04/e4bdbfe794a8e58fafe7bc96e7a88be997b4e99a94e5ae9ae697b6e599a8programmable-interval-timere7bc96e58699e7b3bbe7bb9fe697b6e9929f/) 
- [https://wiki.osdev.org/Pit#I.2FO_Ports](https://wiki.osdev.org/Pit#I.2FO_Ports)。

sleep函数的大致思想如下
- sleep函数设置一个全局变量，时钟中断按固定频率定时发生，每次时钟中断发生后就对全局变量进行减少
- sleep函数在设置完全局变量后就进入循环，一直等到全局变量变为0的时候退出循环

关于sleep的循环，这里采用的是简单的手段，也就是while空循环，不调用hlt指令是因为在调用htl指令后游戏运行速度有明显问题，具体原因不知道，hlt会让CPU停止512个周期好像。

具体的代码实现，放在`lib.c`中。

## 键盘按键监测的实现

键盘按键监测也是以中断的形式实现的，键盘的中断编号是1。针对键盘中断服务程序来说，每次中断要从键盘的端口把对应的值读出来，然后根据读出的值分析按键的情况。

一般键盘按键的编码都采用扫描码的形式，我的程序Bochs返回的是扫描码集set 1。通常来说键盘要与队列结合在一起，每次读取数据写入到队列，然后程序再从队列里读出数据从而的值按键情况。

在这里做个简化，只检测四个按键是否被按下，分别是WASD。数据结构采用简单的循环队列的形式，具体实现参考数据结构这门课程。

# Tetris

到目前位置，基本的函数已经就位了，剩下的就是俄罗斯方块本身业务的代码问题，在这里不打算细讲，虽然俄罗斯方块是本项目的目的，但是却不是本项目的重点（有点反常啊）。

