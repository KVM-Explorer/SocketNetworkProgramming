//
// Created by geek on 2022/4/1.
//
#include <stdio.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

// F
enum class FrameType{
    IP,
    ARP,
    TCP,
    UDP,
    ICMP,
    TEST
};

// F
void udp_header(uint8_t *buffer,int len)
{
    printf("Protocol UDP\n");
    struct udphdr   *udp;
    udp = (struct udphdr*)buffer;
    printf("Src port:%d ", ntohs(udp->source));
    printf("Dst port:%d\n", ntohs(udp->dest));
}

// F
void tcp_header(uint8_t *buffer,int len)
{
    printf("Protocol TCP\n");
    struct tcphdr *tcp;
    tcp = (struct tcphdr*)buffer;
    printf("Src port: %d ", ntohs(tcp->source));
    printf("Dst port: %d\n", ntohs(tcp->dest));
}

// F
void arp_header(uint8_t *buffer,int len)
{
    printf("Protocol ARP\n");
    struct ether_arp *arp_addr;

    arp_addr = (struct ether_arp*)(buffer);

    printf("Sender mac address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp_addr->arp_sha[0], arp_addr->arp_sha[1], arp_addr->arp_sha[2],
           arp_addr->arp_sha[3], arp_addr->arp_sha[4], arp_addr->arp_sha[5]);
    printf("Sender ip address: %d.%d.%d.%d\n",
           arp_addr->arp_tpa[0],arp_addr->arp_spa[1],arp_addr->arp_spa[2],arp_addr->arp_spa[3]);
    printf("Target ip address: %d.%d.%d.%d\n",
           arp_addr->arp_tpa[0],arp_addr->arp_tpa[1],arp_addr->arp_tpa[2],arp_addr->arp_tpa[3]);
    printf("Target mac address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           arp_addr->arp_tha[0], arp_addr->arp_tha[1], arp_addr->arp_tha[2],
           arp_addr->arp_tha[3], arp_addr->arp_tha[4], arp_addr->arp_tha[5]);
}

// F
void icmp_header(uint8_t* buffer,int len)
{
    printf("Protocol: ICMP\n");
    struct icmphdr *icmp;
    icmp = (struct icmphdr*)buffer;
    printf("ICMP code:%d\n",icmp->code);
    printf("ICMP type:%d\n",icmp->type);
    printf("ICMP checksum%d\n",icmp->checksum);

}

// F
FrameType ethernet_header(uint8_t* buffer,int len)
{
    if(buffer[0]==0) return FrameType::TEST;
    printf("Dst MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
    printf("Src MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           buffer[0+6],buffer[1+6],buffer[2+6],buffer[3+6],buffer[4+6],buffer[5+6]);
    auto proto = (uint16_t*)&buffer[12];
    if(*proto== htons(ETHERTYPE_IP))
    {
        printf("Ethernet Protocol: IP\n");
        return FrameType::IP;
    }
    if(*proto== htons(ETHERTYPE_ARP)){
        printf("Ethernet Protocol: ARP\n");
        return FrameType::ARP;
    }
}

// F
FrameType ip_header(uint8_t* buffer,int len)
{
    struct iphdr *ip;
    ip = (struct iphdr*)buffer;
    struct sockaddr_in source,dest;
    source.sin_addr.s_addr = ip->saddr;
    dest.sin_addr.s_addr = ip->daddr;
    printf("IP protocol: IPv%d\n",ip->version);
    printf("Src IP Address:%s\n",inet_ntoa(source.sin_addr));
    printf("Dst IP Address:%s\n", inet_ntoa(dest.sin_addr));

    switch (ip->protocol)
    {
        case IPPROTO_TCP:
            return FrameType::TCP;
            break;
        case IPPROTO_UDP:
            return FrameType::UDP;
            break;
        case IPPROTO_ICMP:
            return FrameType::ICMP;
            break;
    }



}
int main()
{
    try {
        int raw_socket , n;
        uint8_t buffer[2048];
        struct ifreq ethreq;
        struct sockaddr saddr;
        int saddr_len;
        raw_socket = socket(AF_PACKET,SOCK_RAW, htons((ETH_P_ALL)));
        if(raw_socket<0)    throw "Raw socket create failed";

        strncpy(ethreq.ifr_name,"wlo1",IF_NAMESIZE);
        if(ioctl(raw_socket,SIOCGIFFLAGS,&ethreq)==-1) throw "set mix mode failed";

        ethreq.ifr_flags |= IFF_PROMISC;    // 设置混杂模式
        if(ioctl(raw_socket,SIOCSIFFLAGS,&ethreq)==-1) throw "set sif failed";

        int count = 0;
        while (true)
        {
            n = recvfrom(raw_socket,buffer,2048,0,NULL,NULL);



            if(n<42) throw "Receive incomplete packet";
            auto type = ethernet_header(buffer,14);
            if(type==FrameType::TEST)
            {
//            printf("Loacl Communicate\n");
                continue;
            }

            cout<<"frame length: "<<n<<endl;

            FrameType tmp_type;

            switch (type) {
                case FrameType::ARP:
                    arp_header(buffer+14,n-14);
                    break;
                case FrameType::IP:
                    tmp_type = ip_header(buffer+14,n-14);
                    if(tmp_type==FrameType::UDP) udp_header(buffer+14+sizeof(iphdr),n-14-sizeof(iphdr));
                    if(tmp_type==FrameType::TCP) tcp_header(buffer+14+sizeof(iphdr),n-14-sizeof(iphdr));
                    if(tmp_type==FrameType::ICMP)icmp_header(buffer+14+sizeof(iphdr),n-14-sizeof(iphdr));
                    break;
                default:
                    break;
            }

            count++;
            cout<<"================"<<count<<"================"<<endl;
        }
    }catch (const char * msg)
    {
        cout<<msg;
    }


}