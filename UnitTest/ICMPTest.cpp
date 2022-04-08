//
// Created by geek on 2022/4/8.
//
#include "../NetworkLayer/ICMP.h"

void ping()
{
    ICMP icmp("wlo1");
    icmp.Ping("172.24.255.254");
}

void tracert()
{

}

int main()
{
    ping();
}