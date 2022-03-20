#include "DataLinkLayer/Ethernet.h"
#include "DataLinkLayer/CMSA2CD.h"
#include "NetworkLayer/ArpRequest.h"
#include "NetworkLayer/ArpReply.h"
#include "NetworkLayer/IPJudger.h"
#include "NetworkLayer/ICMP.h"
#include "NetworkLayer/IP.h"
#include "TransportLayer/UDP.h"
int main()
{


    //=======================================
    // Ethernet
    //=======================================
//    char *buffer ="Hello World";
//    Ethernet ethernet("wlo1");
//    ethernet.SetMac("08:71:90:01:F8:E2");
//    ethernet.SetType(EthernetType::InternetProtocol);
//    ethernet.ICMPEcho((uint8_t *)buffer, 11);
//    ethernet.SendFrame();

    //============================================
    // Ethernet Listen
    //============================================
//    auto length = ethernet.Listen();
//    printf("Ethernet Length: %d\n",length);

    //========================================
    // CSMA/CD 算法模拟
    //========================================
    //Todo 碰撞展示不明显需要进一步改进
    auto channel = std::shared_ptr<char[]>(new char[100]);
    channel[0] = 1;
    CMSA2CD
    //=========================================
    // ARP Request
    //=========================================
//    ArpRequest request("wlo1");
//    request.SetIp2Mac("192.168.1.1","0:0:0:0:0:0");
//    request.ICMPEcho();
//    // ARP 欺骗的原理在于监听ARP请求并发送应答伪造目的IP在此并发送自身mac地址
//    ArpReply reply("wlo1");
//    reply.SetIp2Mac("192.168.1.1","ef:fe:fe:fe:fe:fe");
//    reply.ICMPEcho();

    //========================================
    // IP Judge
    //=========================================
//    IPJudger judger;
//    judger.Judge("0.0.0.0");

    //=============================================
    // ping
    //=============================================
//    ICMP icmp("wlo1");
//    icmp.Ping("39.156.66.18");
//    icmp.Tracert("39.156.66.18");

//    UDP udp("wlo1",6666);


}
