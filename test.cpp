#include "DataLinkLayer/Ethernet.h"
#include "DataLinkLayer/CMSA2CD.h"
#include "NetworkLayer/ArpRequest.h"
int main()
{
    //=======================================
    // Ethernet
    //=======================================
//    char *buffer ="Hello World";
//    Ethernet ethernet("wlo1");
//    ethernet.SetMac("08:71:90:01:F8:E2");
//    ethernet.SetType(EthernetType::InternetProtocol);
//    ethernet.GenerateFrame((uint8_t *)buffer, 11);
//    ethernet.SendFrame();

    //========================================
    // CSMA/CD 算法模拟
    //========================================
    //Todo 碰撞展示不明显需要进一步改进
//    int host_num = 20;
//    std::vector<CMSA2CD> hosts;
//    for(int i=0;i<host_num;i++)
//    {
//        hosts.push_back(CMSA2CD("Hello World",1,i));
//    }
//    for(auto iter = hosts.begin();iter!=hosts.end();iter++)
//    {
//        (*iter).CreateTask();
//    }
//    for(auto iter = hosts.begin();iter!=hosts.end();iter++)
//    {
//        (*iter).EndTask();
//    }

    //=========================================
    // ARP Request
    //=========================================
    ArpRequest request("wlo1");
    request.SetIp2Mac("192.168.1.1","0:0:0:0:0:0");

    request.GenerateFrame();



}
