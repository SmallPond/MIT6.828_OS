
# MIT6.828 Operating System Engineering
花了两个月多的时间，终于把 MIT6.828 的课程跟完了。刚接触这个课程时，就有一种感觉：哇，好神奇！ 这个课程真的是循序渐进，一步一步在你的脑海里建立起 OS 的大厦框架。从 Lab1 的手把手教学到 Lab6 的放任你自由，很好地对学生的学习进行了引导。学完这个课程后，你也就拥有了一个属于你自己的完整的内核雏形！

# 1. 简介
课程评价：**神级课程——要是早遇到，我还会是这种 five（废物） 系列** 

课程网址：[6.828: Operating System Engineering](https://pdos.csail.mit.edu/6.828/2018/schedule.html)，一直跟着其 schedule 走就可以啦。

xv6 讲义：[a simple, Unix-like teaching operating system](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)，讲义中会将每个部分的实现讲得十分详细，在代码编写时遇到不太清晰的概念时，可以多参考讲义。

我的实现环境：
- VMware Workstation虚拟机
- Ubuntu16.04
- qemu，最好使用 MIT 给的 patch 版本（Lab6 用到 pacth 版本的qemu 较多）。安装方法也可参考[Tools Used in 6.828](https://pdos.csail.mit.edu/6.828/2018/tools.html) 链接
- 工具链 [Tools Used in 6.828](https://pdos.csail.mit.edu/6.828/2018/tools.html)

环境搭建网络上教程很多，这里我就不再赘述，大家可自行百度（搜索以下关键词），包括：

- VMware 或 VirtualBox 安装
- Ubuntu虚拟机
- qemu安装：英文教程参考上文给出[官网的工具链的安装](https://pdos.csail.mit.edu/6.828/2018/tools.html)， 中文教程可参考网络上的博文（[QEMU模拟器安装](https://www.cnblogs.com/gatsby123/p/9746193.html)）

# 2. Xv6
学习6.828时，你会发现经常遇到 xv6 和 JOS这两个名词，不明白它们两者之间的关系，在完成实验时思路就会不是很清晰。xv6 是一个类Unix的教学操作系统，而 JOS 是在xv6的基础上改写，让我们能在其上进行实验的 OS。 所以实际上，当我们遇到不会实现的问题时，可以去参考 xv6 相应部分的源码。

Homework 实现：完整实现代码。[Github_SmallPond/MIT6.828_OS](https://github.com/SmallPond/MIT6.828_OS/tree/master/xv6-public)

- [MIT6.828_Homework_Shell_MIT_6.828](https://blog.csdn.net/Small_Pond/article/details/90544379)
- [MIT6.828_HW2_Boot_xv6_MIT6.828](https://blog.csdn.net/Small_Pond/article/details/90665444)
- [MIT6.828_HW3_XV6 System calls](https://blog.csdn.net/Small_Pond/article/details/91345372)
- [ MIT6.828_HW4_xv6 lazy page allocation](https://blog.csdn.net/Small_Pond/article/details/91346550)
- [ MIT6.828_HW5_xv6 CPU alarm](https://blog.csdn.net/Small_Pond/article/details/92838818)
- [MIT6.828_HW6_Threads and Locking](https://blog.csdn.net/Small_Pond/article/details/92838852)
- [MIT6.828_HW7_xv6 locking](https://blog.csdn.net/Small_Pond/article/details/93200120)
- [MIT6.828_HW8_User-level threads](https://blog.csdn.net/Small_Pond/article/details/94600772)
- [MIT6.828_HW9_barriers](https://blog.csdn.net/Small_Pond/article/details/94968225)
- [MIT6.828_HW10_Bigger file for xv6](https://blog.csdn.net/Small_Pond/article/details/95009224)
- [MIT6.828_HW11_xv6 log](https://blog.csdn.net/Small_Pond/article/details/95210975)

# 3. JOS
以下是我实验过程中记下的笔记，包含我的实现思路以及代码。 不过有些重复的细节，我就没有记录，可以参考我的完整实现代码。[Github_SmallPond/MIT6.828_OS](https://github.com/SmallPond/MIT6.828_OS)

- Lab1 Booting a PC
    - [ LAB_1_Part1_PC Bootstrap and Part2 The Boot loader](https://www.dingmos.com/index.php/archives/3/)
    - [LAB1_Part3_The Kernel](https://www.dingmos.com/index.php/archives/4/)
- Lab2 Memory management
    - [LAB2_Part1_Physical Page Management](https://www.dingmos.com/index.php/archives/5/)
    - [LAB2_Part2_Virtual Memory](https://www.dingmos.com/index.php/archives/6/)
    - [ LAB2_Part3_Kernel Address Space(内核地址空间)](https://www.dingmos.com/index.php/archives/7/)
- Lab3 User-Level Environments
    - [LAB3_User-Level Environments_PartA_User Environments and Exception Handling](https://www.dingmos.com/index.php/archives/8/)
    - [LAB3_User-Level Environments_PartB Page Faults, Breakpoints Exceptions, and System Calls](https://www.dingmos.com/index.php/archives/9/)

- Lab4 Preemptive Multitasking_
    - [LAB4_Preemptive Multitasking_PartA Multiprocessor Support and Cooperative Multitasking](https://www.dingmos.com/index.php/archives/10/)
    - [LAB4_Preemptive Multitasking_PartB Copy-on-Write Fork](https://www.dingmos.com/index.php/archives/11/)
    - [LAB4_Preemptive Multitasking_PartC Preemptive Multitasking and IPC](https://www.dingmos.com/index.php/archives/12/)
- Lab5 File system, Spawn and Shell
    - [Lab5_File system, Spawn and Shell](https://www.dingmos.com/index.php/archives/13/)
- Lab6 Network Driver
    - [Lab6_Network Driver](https://www.dingmos.com/index.php/archives/14/)


# 4. 参考文献
1. [MIT 6.828 JOS 操作系统学习笔记/fatsheep9146](https://www.cnblogs.com/fatsheep9146/category/769143.html)，刚入门时参考，包括环境搭建。博文写得十分详细，可惜貌似只写到了 Lab2。
2.  [clpsz/mit-jos-2014](https://github.com/clpsz/mit-jos-2014)，此大神放出了自己到 Lab4 的代码，其文档提及了一些细节，很有帮助。
3. [Unknown Unknown](https://buweilv.github.io/categories/OS/)，过程较详细（相当于对官方文档做了一遍翻译），英语不好可以参考这边，但我还是建议以官方文档为主，毕竟英语还是要学好呀。这位大神做到了LAB5，但不包括HW。
4. [bysui的博客](https://blog.csdn.net/bysui/article/category/6232831), 这位大神完成了全部的实验，但是我在后面才发现这么好的资源！

我的课程能顺利完成，少不了各位大佬记录下的实验过程，由衷感谢以上各位大神。同时希望我也能帮到后来的学习者~

# 5. FAQ
1. 零基础可以做这个课程吗？

不管做什么事情总会有个从零到一的过程的。当然这个课程需要一些基础，如汇编语言、操作系统、计算机组成原理以及体系结构等。毕竟操作系统是一门比较底层且考察综合性计算机知识的课程。可能零基础做这个课程会相对困难一点，但如果坚持下来了至少会学到一些知识（基础不够的情况下，不要求完全独立完成整个课程，可参考其他博客等资料）。并且我一直认为，以边学边做的方式来学习一门课程是最好的方式之一。种一棵树最好的时机是十年前或现在！加油~

2. 如何开始这个课程？

如果你是完全零基础，那么从熟悉我上文提到的几个名词开始（Vmware、Ubuntu虚拟机等），然后开始搭建实验环境（网络上教程很多，自己得培养出搜索并甄别资料是否优质的能力，不能拿来主义）。然后参考官网的 scheduler 做Lab 和 homework（理想路线）。若不知道如何继续的时候，可适当参考本文的给出的各个博客链接或自行搜索更优质的内容。

3. 6.828 和 6.s081 有什么区别

2018 之前只有 6.828，后来出现了6.s081。简单来说，2018之后的6.828面向研究生，重点是操作系统研究；6.s081 面向本科生，有些类似2018年6.828的低难度版？（我没有做）。以下是官方介绍。

> 6.828 and 6.S081 will be offered as two separate classes. 6.S081 (Introduction to Operating Systems) will be taught as a stand-alone AUS subject for undergraduates, and will provide an introduction to operating systems. 6.828 will be offered as a graduate-level seminar-style class focused on research in operating systems. 6.828 will assume you have taken 6.S081 or an equivalent class. 
