//
// Created by geek on 2022/3/14.
//

#include "IP.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/ioctl.h>

IP::IP(std::string device) : ethernet_(device)
{
    raw_socket = socket(AF_PACKET,SOCK_RAW, htons(ETH_P_ALL));
    memset(buffer_,0,sizeof(buffer_));

    memset(&ifreq_ip_, 0, sizeof(ifreq_ip_));
    memset(&ifreq_index_, 0, sizeof(ifreq_index_));

    strncpy(ifreq_index_.ifr_name, device.c_str(), IFNAMSIZ - 1);
    if(ioctl(raw_socket,SIOCGIFINDEX,&ifreq_index_) < 0)
        printf("error in index ioctl reading\n");

    strncpy(ifreq_ip_.ifr_name,device.c_str(),IFNAMSIZ-1);
    if(ioctl(raw_socket,SIOCGIFADDR,&ifreq_ip_) < 0)
        printf("error in ip address reading\n");

    // 获取ip地址
    struct sockaddr_in* sockaddress_in = (struct sockaddr_in*)&ifreq_ip_.ifr_addr;
    source_address_ = sockaddress_in->sin_addr.s_addr;
    memset(&iphdr_,0,sizeof(iphdr_));
}

void IP::SetTarget(std::string address) {
    target_address_ = htonl(inet_addr(address.c_str()));
}

uint16_t IP::CheckSum(uint8_t *data, int length) {
    int p = length%2;
    uint16_t sum = 0;
    for(int i=0;i<length;i+=2)
        sum+=data[i]+(data[i+1]<<8);
    if (p==1) sum+=data[length-1];
    while (sum>>16)
    {
        sum = (sum&0xffff)+(sum>>16);
    }

    return ~sum;
}

void IP::Listen() {
    //Todo 过滤监听IP

}

void IP::SendMessage(uint8_t *message, int length) {
    int tot_length = 0;
    iphdr_.version = 4;
    iphdr_.ihl = 5;
    iphdr_.tos = IPTOS_LOWDELAY;
    iphdr_.tot_len = sizeof(iphdr)+length;
    iphdr_.id = htons(10201);
    iphdr_.ttl = 64;
    iphdr_.protocol = IPPROTO_ICMP;
    iphdr_.saddr = source_address_;
    iphdr_.daddr = target_address_;
    memcpy(buffer_,&iphdr_,sizeof(struct iphdr));

    tot_length += sizeof(struct iphdr);
    memcpy(buffer_+tot_length,message,length);

    ethernet_.SetMac("08:71:90:01:F8:E2");
    ethernet_.SetType(EthernetType::InternetProtocol);
    ethernet_.GenerateFrame(buffer_,tot_length);
    ethernet_.SendFrame();
}

uint32_t IP::GetSourceAddress() {
    return source_address_;
}

IP::~IP() {}