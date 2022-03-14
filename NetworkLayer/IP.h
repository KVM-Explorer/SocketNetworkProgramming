#pragma once
#include <netinet/ip.h>
#include "../DataLinkLayer/Ethernet.h"
class IP {
private:
    int raw_socket;
    ifreq ifreq_ip_;
    ifreq ifreq_index_;
    uint32_t target_address_;
    uint32_t source_address_;
protected:
    struct iphdr iphdr_;
    Ethernet ethernet_;
    uint8_t buffer_[65536];

public:
    IP(std::string device);
    ~IP();

    void SendMessage(uint8_t *message,int length);
    void Listen();
    uint16_t CheckSum(uint8_t* data,int length);
    void SetTarget(std::string address);
    uint32_t GetSourceAddress();
};


