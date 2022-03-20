//
// Created by geek on 2022/3/10.
//

#include "ICMP.h"
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <sstream>
#include <csignal>
#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/filter.h>

ICMP::ICMP(std::string device) : ethernet_(device)
{

    int raw_socket = socket(AF_PACKET,SOCK_RAW, htons(ETH_P_ALL));
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
    memset(&ip_header_,0,sizeof(ip_header_));
    memset(&icmp_message_,0,sizeof(icmp_message_));
    total_length_ = 0;
}

ICMP::~ICMP() {}

// Todo make it length>=64 bytes

void ICMP::ICMPEcho(int ttl)
{
    memset(buffer_,0,sizeof(buffer_));
    int index = 0;
    //Todo total_length_ = buffer+ ethernet_header
    // IP Header
    ip_header_.version = 4;
    ip_header_.ihl = 5;
    ip_header_.tos = IPTOS_LOWDELAY;
    ip_header_.tot_len = total_length_;
    ip_header_.id = htons(10201);
    ip_header_.ttl = ttl;
    ip_header_.protocol = IPPROTO_ICMP;
    ip_header_.saddr = source_address_;
    ip_header_.daddr = target_address_;
    memcpy(buffer_,&ip_header_,sizeof(ip_header_));
    index += sizeof(ip_header_);

    //ICMP
    auto pid = getpid();
    icmp_message_.type = ICMP_ECHO;     //  1bytes
    icmp_message_.code = 0;             //  1bytes
    icmp_message_.un.echo.id = pid;     //  2bytes
    icmp_message_.un.echo.sequence = 2; //  2bytes
    icmp_length_ = sizeof(icmp_message_); // 64 - Ethernet:14 - IP Header:20 = 26
    // Todo 填充icmp报文消息
    icmp_message_.checksum = CheckSum((uint8_t *)&icmp_message_,icmp_length_);
    memcpy(buffer_+index,&icmp_message_,sizeof(icmp_message_));
    index+=sizeof(icmp_message_);

    printf("ICMP Message Length: %d\n",sizeof(icmp_message_));

    ethernet_.SetMac("08:71:90:01:F8:E2");
    ethernet_.SetType(EthernetType::InternetProtocol);
    ethernet_.GenerateFrame(buffer_,index);
    ethernet_.SendFrame();
    printf("=====================Ping Over===============================\n");
}

void ICMP::Listen()
{
    int fd = socket(PF_PACKET,SOCK_RAW, htons(ETH_P_ALL));
    if(fd<0) printf("socket create failed\n");
    struct sock_fprog filter;
    struct sock_filter filter_code[]={
            { 0x28, 0, 0, 0x0000000c },
            { 0x15, 0, 3, 0x00000800 },
            { 0x30, 0, 0, 0x00000017 },
            { 0x15, 0, 1, 0x00000001 },
            { 0x6, 0, 0, 0x00040000 },
            { 0x6, 0, 0, 0x00000000 },

    };

    filter.len = sizeof(filter_code)/sizeof(sock_filter);
    filter.filter = filter_code;

    // 设置过滤规则
    if(setsockopt(fd,SOL_SOCKET,SO_ATTACH_FILTER,&filter,sizeof(filter))<0)
    {
        printf("Socket set fail\n");
    }

    int receive_length = recvfrom(fd, buffer_, sizeof(buffer_), 0, NULL, NULL);
    if(receive_length < 0)printf("Receive none\n");
    for(int i=0; i < receive_length; i++)
    {
        if(i%10!=0)printf("%02x ",buffer_[i]);
        else printf("\n");
    }
    printf("\n");

    ethernet_.ExtractFrame(buffer_);
    ExtractFrame(buffer_+sizeof(ethhdr), receive_length-sizeof(ethhdr));
}

void ICMP::Ping(std::string ip)
{
    target_address_ =inet_addr(ip.c_str());
    ICMPEcho();
    Listen();
}

void ICMP::Tracert(std::string ip)
{
    auto split=[](std::string x)
    {
        std::stringstream splitor, sstream;
        std::string word;
        int index=0;
        uint8_t address[4];
        uint32_t ip;
        splitor << x;
        while (getline(splitor, word, '.')) {
            sstream.clear();
            sstream<<word;
            sstream >>address[index++];
        }
        memcpy(&ip,address,4);
        return ip;
    };
    target_address_ = split(ip);
    for(int i=1;i<32;i++)
    {
        ICMPEcho(i);
        Listen();
// todo if(/*监测到ip*/)break;
    }
}

int ICMP::ExtractFrame(uint8_t *data, int length) {
    struct iphdr *ip = (struct iphdr*)data;
    in_addr address;
    address.s_addr = ip->saddr;
    printf("IP Header-> Source: %s\n", inet_ntoa(address));
    address.s_addr = ip->daddr;
    printf("IP Header-> Target: %s\n", inet_ntoa(address));
    return 0;
}

uint16_t ICMP::CheckSum(uint8_t *data, int len) {

    int p = len%2;
    uint16_t sum = 0;
    for(int i=0;i<len;i+=2)
        sum+=data[i]+(data[i+1]<<8);
    if (p==1) sum+=data[len-1];
    while (sum>>16)
    {
        sum = (sum&0xffff)+(sum>>16);
    }

    return ~sum;
}
