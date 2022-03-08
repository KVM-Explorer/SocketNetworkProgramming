#include <sstream>
#include <vector>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Arp.h"


Arp::Arp(std::string device): ethernet_(device)
{
    auto tmp_socket = socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
    memset(buffer_,0,sizeof (buffer_));
    ifreq ifreq_mac,ifreq_ip;
    memset(&ifreq_mac,0,sizeof(ifreq_mac));
    memset(&ifreq_ip,0,sizeof (ifreq_ip));
    printf("Device Name: %s\n",device.c_str());
    strncpy(ifreq_ip.ifr_name,device.c_str(),IFNAMSIZ-1);
    strncpy(ifreq_mac.ifr_name,device.c_str(),IFNAMSIZ -1);

    if(ioctl(tmp_socket,SIOCGIFHWADDR,&ifreq_mac)<0)
    {
        printf("error in index ioctl reading\n");
    }
    if(ioctl(tmp_socket,SIOCGIFADDR,&ifreq_ip)<0)
    {
        printf("error in IP ioctl reading\n");
    }
    memcpy(arp_content_.source_mac_, ifreq_mac.ifr_hwaddr.sa_data, ETH_ALEN);
    memcpy(arp_content_.source_ip_,ifreq_ip.ifr_addr.sa_data,4);
    auto ip_address = (((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr);
    memcpy(arp_content_.source_ip_,&ip_address,sizeof(ip_address));

    ethernet_.SetType(EthernetType::ARP);
    ethernet_.SetMac("ff:ff:ff:ff:ff:ff");
}

void Arp::SetAttribute()
{
    arp_header_.ar_hrd  = htons(ARPHRD_ETHER);
    arp_header_.ar_pro = htons(ETH_P_IP);
    arp_header_.ar_hln = ETH_ALEN;
    arp_header_.ar_pln = 4;
    arp_header_.ar_op = htons(ARPOP_REQUEST);

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



