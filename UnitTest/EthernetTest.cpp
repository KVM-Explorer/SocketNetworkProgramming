//
// Created by geek on 2022/4/1.
//

#include "../DataLinkLayer/Ethernet.h"

int main()
{
    Ethernet p = Ethernet("wlo1");
    while (true)
    {
        p.Listen();

    }

}