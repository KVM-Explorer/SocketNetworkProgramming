//
// Created by geek on 2022/4/8.
//

#include "../NetworkLayer/IPJudger.h"

int main()
{
    IPJudger judger;
    std::string ip = "127.0.0.10";
    judger.Judge(ip);

    return 0;
}