//
// Created by geek on 2022/3/14.
//

#include "UDP.h"
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>


UDP::UDP(std::string device, uint16_t source_port) {
    udp_socket_ = socket(AF_INET,SOCK_DGRAM,0);
}

void UDP::SetTarget(std::string target_address, uint16_t target_port)
{
    // I meet some problems while I try to bind the program with a specific
    // port, maybe bind is right,however it needs to use a single raw socket
    // between different layer, i plan to solve this in the future.
//    ip_.SetTarget(target_address);
    target_address_ .sin_port = htons(target_port);
    target_address_.sin_family = AF_INET;
    target_address_.sin_addr.s_addr = inet_addr(target_address.c_str());
}

void UDP::Listen()
{
    // Todo 带完成
}

void UDP::SendMessage(std::string message)
{
    int length = sendto(udp_socket_,message.c_str(),message.length(),0,
                        (struct sockaddr*)&target_address_,sizeof(target_address_));
    if(length<0) throw "Sned Failed";
}

uint16_t UDP::CheckSum(uint8_t *data, int length)
{
 //todo check udp
    return 0;
}

UDP::~UDP() {}