#pragma once
#include<net/if_arp.h>
#include <string>
#include <linux/if_ether.h>
#include <cstring>
#include <net/if.h>
#include "../DataLinkLayer/Ethernet.h"
class Arp {
public:
    enum class ARPType
    {
        Request,
        Reply,
    };
protected:
    uint8_t buffer_[28];
    arphdr arp_header_;
    Ethernet ethernet_;
    struct arp_con
    {
        uint8_t source_mac_[ETH_ALEN];
        uint8_t source_ip_[4];
        uint8_t target_mac_[ETH_ALEN];
        uint8_t target_ip_[4];
    }arp_content_;
public:
    Arp(std::string );

    ~Arp();
    void Init();

    void SetAttribute(ARPType type);

    void GenerateFrame();
};


