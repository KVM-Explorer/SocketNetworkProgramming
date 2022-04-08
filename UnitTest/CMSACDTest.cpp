//
// Created by geek on 2022/4/8.
//

#include "../DataLinkLayer/CMSA2CD.h"

int main()
{
    //========================================
    // CSMA/CD 算法模拟
    //========================================
    auto channel = std::shared_ptr<char[]>(new char[100]);
    srand((unsigned int)time(NULL));
    channel[0] = 1;
    CMSA2CD clientA('A',0,channel);
    CMSA2CD clientB('B',1,channel);

    std::thread p1 (&CMSA2CD::Stimulate,&clientA);
    std::thread p2 (&CMSA2CD::Stimulate,&clientB);
    p1.join();
    p2.join();
}
