//
// Created by geek on 2022/4/1.
//

#include "../DataLinkLayer/Ethernet.h"
#include <cstring>

void ethernet_listen(Ethernet& p)
{
    while (true)
    {
        p.Listen();
    }
}

void ethernet_send(Ethernet& p)
{
    // Send
    char buffer[] = {"Hello World"};
    int len = std::strlen(buffer);
    p.SetMac("ff:ff:ff:ff:ff:ff");
    p.SetType(EthernetType::InternetProtocol);
    p.GenerateFrame(reinterpret_cast<uint8_t *>(buffer),len);
    p.SendFrame();
}

int main()
{
    int signal = 0;  // 0 listen 1 send
    Ethernet p = Ethernet("wlo1");

    if(!signal)
    {
        ethernet_listen(p);
    }else
    {
        ethernet_send(p);
    }


}