#pragma once
#include "../NetworkLayer/IP.h"
#include <linux/udp.h>

class UDP {
private:
    uint16_t source_port_;
    uint16_t target_port_;
protected:
    IP ip_;
    struct udphdr udp_header_;
    uint8_t buffer_[65535];

public:
    UDP(std::string device,uint16_t source_port);
    ~UDP();

    void SetTarget(std::string target_address,uint16_t target_port);
    void Listen();
    uint16_t CheckSum(uint8_t *data,int length);
    void SendMessage(std::string message);




};

