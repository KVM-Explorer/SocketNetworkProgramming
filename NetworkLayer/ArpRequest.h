#pragma once
#include "Arp.h"

class ArpRequest: public Arp{
public:
    ArpRequest(std::string device);
    ~ArpRequest();
    void SetIp2Mac(std::string target_address,std::string target_mac,
                   std::string source_address = "default",
                   std::string source_mac = "default");
    void Init();
};


