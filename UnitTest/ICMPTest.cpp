//
// Created by geek on 2022/4/8.
//
#include "../NetworkLayer/ICMP.h"

void ping()
{
    ICMP icmp("wlo1");
    icmp.Ping("39.156.66.14");
}

void tracert()
{
    ICMP icmp("wlo1");
    icmp.Tracert("39.156.66.14");
}

int main()
{
    int signal = 1;
    switch (signal) {
        case 0:
            ping();
            break;
        case 1:
            tracert();
            break;
        default:
            break;
    }
}