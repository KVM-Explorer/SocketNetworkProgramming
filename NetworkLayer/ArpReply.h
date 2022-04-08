#pragma once
#include "Arp.h"

class ArpReply: public Arp{
public:
    ArpReply(std::string device);
    ~ArpReply();
    void SetIp2Mac(std::string target_address,std::string target_mac,
                   std::string source_address = "default",
                   std::string source_mac = "default");

    void Init();
};

