//
// Created by geek on 2022/3/9.
//

#include <sstream>
#include <vector>
#include "IPJudger.h"
#include <algorithm>

/**
 * @param ip
 * @return
 * @details
 *      1. 合法字符
 *      2. 格式正确
 *      3. 范围正确
 */

bool IPJudger::IsValid(std::string ip)
{
    /// 特殊字符监测
    const std::string std_str = "0123456789.";
    for(int i=0; i < ip.length(); i++)
    {
        int flag = false;
        for(auto std_char:std_str)  std_char==ip[i]?flag=true:flag;
        if(!flag)
        {
            State=IpResult::error;
            return false;
        }
    }

    /// 格式监测
    int point_num = 0;
    for(int i=0; i < ip.length(); i++) ip[i] == '.' ? point_num++ : point_num;
    if (point_num!=3)
    {
        State = IpResult::error;
        return false;
    }

    /// 范围正确
    auto ip_address = Split(ip);
    for(int i=0;i<4;i++)
    {
        if(int(ip_address[i])<0||int(ip_address[i])>255)
        {
            State = IpResult::error;
            return false;
        }
    }

    return true;
}

std::vector<uint8_t> IPJudger::Split(std::string x) {

    char ch = '.';
    std::stringstream splitor, sstream;
    std::string word;
    int tmp;
    std::vector<uint8_t> result;
    result.clear();
    splitor << x;
    while (getline(splitor, word, ch)) {
        sstream.clear();
        sstream<<word;
        sstream >> tmp;
        result.push_back(tmp);
    }
    return result;
}

/**
 * @param ip_address
 * @details
 *      1. 监测范围正确
 *      2. 监测所属IP分类
 */
void IPJudger::Judge(std::string ip_address)
{
    if(!IsValid(ip_address)) {printf("IP Address Is IsValid!\n"); return;}

    auto ip_vector = Split(ip_address);
    if(ip_vector[0]>=1 && ip_vector[0]<=127)
    {
        State = IpResult::AType;
        printf("IP Address is A Type\n");
        return;
    }
    if(ip_vector[0]>=128&&ip_vector[0]<=191)
    {
        State = IpResult::BType;
        printf("IP Address is B Type\n");
        return;
    }
    if(ip_vector[0]>=192&&ip_vector[0]<=223)
    {
        State = IpResult::CType;
        printf("IP Address is C Type\n");
        return;
    }
    if(ip_vector[0]>=224&&ip_vector[0]<=239)
    {
        State = IpResult::DType;
        printf("IP Address is D Type\n");
        return;
    }
    printf("IP Address is not belong to A,B,C,D basic type\n");

}
