//
// Created by geek on 2022/3/29.
//
#include "../TransportLayer/UDP.h"

int main()
{
    UDP udp("wlo1",10308);
    udp.SetTarget("39.156.66.255",80);
    udp.SendMessage("Hello");
}