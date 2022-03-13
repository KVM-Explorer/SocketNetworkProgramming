#pragma once
#include <netinet/ip_icmp.h>
#include "../DataLinkLayer/Ethernet.h"
class ICMP {

protected:
    struct iphdr ip_header_;
    struct icmphdr icmp_message_;
    Ethernet ethernet_;
    uint8_t buffer_[65536];
    uint16_t total_length_;
    uint16_t icmp_length_;

public:
    ICMP(std::string device);
    ~ICMP();

    void Ping(std::string ip);
    void Tracert(std::string ip);
    void ICMPEcho(int ttl=64);
    void Listen();
    int ExtractFrame(uint8_t *data, int length);
    uint16_t CheckSum(uint8_t* data, int len);
private:
    ifreq ifreq_ip_;
    ifreq ifreq_index_;
    uint32_t source_address_;
    uint32_t target_address_;

};



