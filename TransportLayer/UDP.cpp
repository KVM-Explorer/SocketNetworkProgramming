//
// Created by geek on 2022/3/14.
//

#include "UDP.h"
#include <cstring>
#include <netinet/in.h>


UDP::UDP(std::string device, uint16_t source_port): ip_(device) {
    memset(buffer_,0,sizeof(buffer_));
    memset(&udp_header_,0,sizeof(udp_header_));
    source_port_ = source_port;
}

void UDP::SetTarget(std::string target_address, uint16_t target_port)
{
    // I meet some problems while I try to bind the program with a specific
    // port, maybe bind is right,however it needs to use a single raw socket
    // between different layer, i plan to solve this in the future.
    ip_.SetTarget(target_address);
    target_port_ = target_port;
}

void UDP::Listen()
{
    // Todo 带完成
}

void UDP::SendMessage(std::string message)
{
    int length = 0;
    udp_header_.source = htons(source_port_);
    udp_header_.dest = htons(target_port_);



    ip_.SendMessage(buffer_,length);
}

uint16_t UDP::CheckSum(uint8_t *data, int length)
{
 //todo check udp
    return 0;
}

UDP::~UDP() {}