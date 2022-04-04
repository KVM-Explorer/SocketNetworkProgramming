#include "Ethernet.h"
#include <netinet/in.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <netpacket/packet.h>

#define POLY        0x1021

// tips ioctl 不能重复使用同一个变量,直接重复使用的话会导致获取的历史内容发生变更

Ethernet::Ethernet(std::string device) {
    raw_socket_ = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    memset(buffer_,0,sizeof(buffer_));
    printf("Raw Scocket ID:%d\n", raw_socket_);

    memset(&ifreq_index_, 0, sizeof(ifreq_index_));
    strncpy(ifreq_index_.ifr_ifrn.ifrn_name, device.c_str(), IFNAMSIZ - 1);//giving name of Interface
    if (ioctl(raw_socket_, SIOCGIFINDEX, &ifreq_index_) < 0) {
        printf("error in index ioctl reading\n");
    }

    memset(&ifreq_mac_, 0, sizeof(ifreq_mac_));
    strncpy(ifreq_mac_.ifr_ifrn.ifrn_name, device.c_str(), IFNAMSIZ - 1);//giving name of Interface
    if (ioctl(raw_socket_, SIOCGIFHWADDR, &ifreq_mac_) < 0) {
        printf("error in Hardware ioctl reading\n");
    }
    memcpy(source_mac_, ifreq_mac_.ifr_hwaddr.sa_data, 6 * sizeof(uint8_t));

    buffer_length_ = 0;
}

Ethernet::~Ethernet() {

}

void Ethernet::SetMac(std::string target, std::string source) {
    auto mac_array = ([](std::string x) {
        std::stringstream split, sstream;
        std::string word;
        int tmp;
        std::vector<uint8_t> result;
        split << x;
        while (getline(split, word, ':')) {
            sstream.clear();
            sstream << std::hex << word;
            sstream >> tmp;
            result.push_back(tmp);
        }

        return result;
    });

    if (source != "none") {
        auto source_vector = mac_array(source);
        for (int i = 0; i < 6; i++) source_mac_[i] = source_vector[i];
    }
    printf("Source Mac: ");
    for (int i = 0; i < 6; i++) {
        printf("0x%02x ", source_mac_[i]);
    }
    printf("\n");
    auto target_vector = mac_array(target);
    for (int i = 0; i < 6; i++) target_mac_[i] = target_vector[i];

    printf("Target Mac: ");
    for (int i = 0; i < 6; i++) {
        printf("0x%02x ", target_mac_[i]);
    }
    printf("\n");
}

void Ethernet::SetType(EthernetType type) {
    switch (type) {
        case EthernetType::InternetProtocol:
            type_ = htons(ETHERTYPE_IP);
            break;
        case EthernetType::ARP:
            type_ = htons(ETHERTYPE_ARP);
            break;
        default:
            type_ = htons(ETHERTYPE_IP);
            break;
    }
    printf("Ethernet Type: 0x%04x\n", type_);
}


void Ethernet::GenerateFrame(uint8_t *data, int length) {

    /// Ethernet Layer
    struct ethhdr *ethernet_header = (struct ethhdr *) buffer_;
    memcpy(ethernet_header->h_dest, target_mac_, 6 * sizeof(uint8_t));
    memcpy(ethernet_header->h_source, source_mac_, 6 * sizeof(uint8_t));
    ethernet_header->h_proto = type_;
    buffer_length_ += sizeof(struct ethhdr);
    printf("Ethernet Header Lenghth = %d\n", buffer_length_);

    // Network Layer Data
    memcpy(buffer_ + buffer_length_, data, length);
    buffer_length_ += length;
    printf("Buffer Lenghth =%d\n", buffer_length_);
}

void Ethernet::SendFrame() {
    struct sockaddr_ll sender_info;
    sender_info.sll_ifindex = ifreq_index_.ifr_ifindex;
    sender_info.sll_halen = ETH_ALEN;

    memcpy(sender_info.sll_addr, target_mac_, 6 * sizeof(uint8_t));
    for (int i = 0; i < buffer_length_; i++) {
        if (i % 10 == 0) printf("\n");
        printf("0x%x", *(buffer_ + i) >> 4 & 0xf);
        printf("%x ", *(buffer_ + i) & 0xf);
    }
    printf("\n");
    auto message_length = sendto(raw_socket_, buffer_, buffer_length_,
                                 0, (const struct sockaddr *) &sender_info, sizeof(sender_info));
    printf("Send Buffer Size: %d\n", message_length);
}


/**
 * Calculating CRC-16 in 'C'
 * @para buffer, start of data
 * @para num, length of data
 * @para crc, incoming CRC
 */
uint16_t Ethernet::CrcSum(uint8_t *buffer, int num, uint16_t crc) {

    int i;
    for(int i=0;i<num;i++)
    {
        crc = crc ^ (*buffer++ << 8);     /* Fetch byte from memory, XOR into CRC top byte*/
        for (i = 0; i < 8; i++)             /* Prepare to rotate 8 bits */
        {
            if (crc & 0x8000)            /* b15 is set... */
                crc = (crc << 1) ^ POLY;    /* rotate and XOR with polynomic */
            else                          /* b15 is clear... */
                crc <<= 1;                  /* just rotate */
        }                             /* Loop for 8 bits */
        crc &= 0xFFFF;                  /* Ensure CRC remains 16-bit value */
    }                               /* Loop until num=0 */
    return(crc);                    /* Return updated CRC */
}

/**
 * @brief receive packet with raw socket
 * @return
 */
int Ethernet::Listen() {
    int address_length = sizeof(socket_address_);
    memset(&socket_address_,0,sizeof(socket_address_));

    // Receive Net package
    receive_length_ = recvfrom(raw_socket_,buffer_,65535,0,&socket_address_,(socklen_t *)&address_length);

    if(receive_length_<0)
    {
        printf("error in reading recvfrom function\n");
        return -1;
    }

    auto ethernet_header = (struct ethhdr*) buffer_;
    receive_type_ = ntohs(ethernet_header->h_proto);
    printf("Dest MAC Address: %02x:%02x:%02x:%02x:%02x:%02x ",
           ethernet_header->h_source[0],
           ethernet_header->h_source[1],
           ethernet_header->h_source[2],
           ethernet_header->h_source[3],
           ethernet_header->h_source[4],
           ethernet_header->h_source[5]);

    printf("Dest MAC Address: %02x:%02x:%02x:%02x:%02x:%02x ",
           ethernet_header->h_dest[0],
           ethernet_header->h_dest[1],
           ethernet_header->h_dest[2],
           ethernet_header->h_dest[3],
           ethernet_header->h_dest[4],
           ethernet_header->h_dest[5]);

    printf("Protocol : %d\n",ethernet_header->h_proto);
    return receive_length_;
}

/**
 * @brief 对接受的frame进行以太网层的分析和捷报
 * @param content 原始buffer地址
 * @return
 */
void Ethernet::ExtractFrame(uint8_t *content)
{
    struct ethhdr *ethernet_header = (struct ethhdr*)(content);
    printf("Ethernet Source Address: ");
    for(int i=0;i<6;i++)
    {
        if(i!=5)printf("%02x:",ethernet_header->h_source[i]);
        else printf("%02x",ethernet_header->h_source[i]);
    }
    printf("\n");
    printf("Ethernet Destination Address: ");
    for(int i=0;i<6;i++)
    {
        if(i!=5)printf("%02x:",ethernet_header->h_dest[i]);
        else printf("%02x",ethernet_header->h_dest[i]);
    }
    printf("\n");
    printf("Ethernet Protocol: %x\n",ntohs(ethernet_header->h_proto));

    return;

}

EthernetType Ethernet::GetRecvType() {
    switch (receive_type_) {
        case ETHERTYPE_IP:
            return EthernetType::InternetProtocol;
            break;
        case ETHERTYPE_ARP:
            return EthernetType::ARP;
            break;
        default:
            return EthernetType::Unknow;
            break;
    }
}
