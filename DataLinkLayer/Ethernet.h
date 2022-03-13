#pragma once
#include<string>
#include<cstdio>
#include<net/if.h>
#include<sys/socket.h>
#include<linux/if_ether.h>

enum class EthernetType{
    Unknow,
    InternetProtocol,
    ARP,
};
class Ethernet {
private:
    uint16_t type_;
    uint8_t buffer_[65536];
    uint8_t source_mac_[6];
    uint8_t target_mac_[6];
    int buffer_length_;
    int receive_length_;

protected:
    struct ifreq ifreq_index_;
    struct ifreq ifreq_mac_;
    int raw_socket_;
    struct ethhdr ethernet_header_;
    struct sockaddr socket_address_;
    uint16_t receive_type_;



public:
    Ethernet(std::string device);
    ~Ethernet();
    void SetMac(std::string target,std::string source = "none");
    void SetType(EthernetType type);
    void GenerateFrame(uint8_t *data, int length);
    void SendFrame();
    EthernetType GetRecvType();

    int Listen();
    void ExtractFrame(uint8_t *content);
public:
    static uint16_t CrcSum(uint8_t *buffer, int num, uint16_t crc);

public:

};


