#pragma once
#include<string>
#include<cstdio>
#include<net/if.h>
#include<sys/socket.h>

enum class EthernetType{
    InternetProtocol,
};
class Ethernet {
private:
    uint16_t type_;
    uint8_t buffer_[1024];
    uint8_t source_mac_[6];
    uint8_t target_mac_[6];
    int buffer_length_;

protected:
    struct ifreq ifreq_index_;
    struct ifreq ifreq_mac_;
    int raw_socket_;


public:
    Ethernet(std::string device);
    ~Ethernet();
    void SetMac(std::string target,std::string source = "none");
    void SetType(EthernetType type);
    void GenerateFrame(uint8_t *data, int length);
    void SendFrame();
public:
    static std::string CalculateCrc(std::string text);

public:

};


