#include "Ethernet.h"
#include <netinet/in.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <memory.h>
#include <cstdlib>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sstream>
#include <vector>
#include <iostream>

// tips ioctl 不能重复使用同一个变量,直接重复使用的话会导致获取的历史内容发生变更

Ethernet::Ethernet(std::string device) {
    raw_socket_ = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
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
    printf("Send Buffer Size: %d", message_length);
}
