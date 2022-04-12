# Socket Network Programming

> System : Manjaro KDE Linux
>
> Core Version: 5.13.19-2-Manjaro(x64)
>
> Project Sites: https://github.com/KVM-Explorer/SocketNetworkProgramming

## 文档建构

```bash
├── Chat
│   ├── ChatClient.cpp
│   ├── ChatClient.h
│   ├── ChatServer.cpp
│   └── ChatServer.h
├── CMakeLists.txt
├── DataLinkLayer
│   ├── CMSA2CD.cpp
│   ├── CMSA2CD.h
│   ├── Ethernet.cpp
│   └── Ethernet.h
├── NetworkLayer
│   ├── Arp.cpp
│   ├── Arp.h
│   ├── ArpReply.cpp
│   ├── ArpReply.h
│   ├── ArpRequest.cpp
│   ├── ArpRequest.h
│   ├── ICMP.cpp
│   ├── ICMP.h
│   ├── IP.cpp
│   ├── IP.h
│   ├── IPJudger.cpp
│   └── IPJudger.h
├── README.md
├── Sniffer
├── TransportLayer
│   ├── TCP.cpp
│   ├── TCP.h
│   ├── UDP.cpp
│   └── UDP.h
└── UnitTest
    ├── ArpTest.cpp
    ├── BoardcastTest.cpp
    ├── ChatClientTest.cpp
    ├── ChatServerTest.cpp
    ├── CMSACDTest.cpp
    ├── EthernetTest.cpp
    ├── ICMPTest.cpp
    ├── IPTest.cpp
    ├── SnifferTest.cpp
    └── SynFlood.cpp

```



## 实现功能

- 以太帧封装
- CMSA/CS算法模拟
- IP数据帧封装
- ARP协议
  - ARP请求
  - ARP应答
  - ARP欺骗
- IP地址判断
- ICMP协议
  - ping命令
  - tracert命令
- 网络嗅探程序
  - Ethernet帧
  - ARP协议
  - ICMP协议
  - TCP协议
  - UDP协议
  - IP协议
- 基于UDP的简单聊天室程序
- UDP数据发送和接收
- SYN洪泛攻击

## 特点

绝大多数基于Raw socket实现，并对对其进行了封装。~~尽管在后期我自己都看不下去了，设计的太垃圾~~。各种协议基本没有进行自行构建结构体而是采用了Linux系统库提供的封装。

## 使用

1. Linux下打开终端进入项目文件夹
2. 依次执行如下命令构建可执行文件

```bash
mkdir build && cd build && cmake .. 
```

```bash
make -j
```



## Todo

- 优化CMSA/CD模拟算法的50%bug问题
- 重新设计架构
- 修复在其他Linux操作系统下编译错误的bug
- 根据程序运行时参数进行配置
