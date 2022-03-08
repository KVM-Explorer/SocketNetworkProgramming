#include <sstream>
#include <vector>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Arp.h"


Arp::Arp(std::string device): ethernet_(device)
{

    ethernet_.SetType(EthernetType::ARP);

}

void Arp::SetAttribute(ARPType type)
{
    arp_header_.ar_hrd  = htons(ARPHRD_ETHER);
    arp_header_.ar_pro = htons(ETH_P_IP);
    arp_header_.ar_hln = ETH_ALEN;
    arp_header_.ar_pln = 4;
    switch (type) {
        case ARPType::Request:
            arp_header_.ar_op = htons(ARPOP_REQUEST);
            break;
        case ARPType::Reply:
            arp_header_.ar_op = htons(ARPOP_REPLY);
            break;
        default:
            arp_header_.ar_op = htons(ARPOP_REQUEST);
            break;
    }


}

void Arp::GenerateFrame() {
    int total_length = 0;
    memcpy(buffer_,&arp_header_,sizeof (arp_header_));
    total_length+=sizeof (arp_header_);
    memcpy(buffer_+total_length,&arp_content_,sizeof(arp_con));
    total_length+=sizeof (arp_con);
    for(int i=0;i<total_length;i++)
    {
        if(i%10==0)printf("\n");
        printf("0x%02x ",buffer_[i]);
    }

    ethernet_.GenerateFrame(buffer_,total_length);
    ethernet_.SendFrame();

}


Arp::~Arp() {

}

void Arp::Init() {

}



