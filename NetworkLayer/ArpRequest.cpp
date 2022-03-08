#include <vector>
#include <sstream>
#include "ArpRequest.h"

ArpRequest::ArpRequest(std::string device) : Arp(device)
{
    this->SetAttribute();
}

ArpRequest::~ArpRequest() {

}

/**
 * Arp请求地址字段设置默认默认自动填写本地mac和ip地址，也可手动指定
 * @param target_address
 * @param target_mac
 * @param source_address
 * @param source_mac
 */
void ArpRequest::SetIp2Mac(std::string target_address, std::string target_mac, std::string source_address,
                           std::string source_mac) {
    auto address_array = ([](std::string x,char ch) {
        std::stringstream splitor, sstream;
        std::string word;
        int tmp;
        std::vector<uint8_t> result;
        result.clear();
        splitor << x;
        while (getline(splitor, word, ch)) {
            sstream.clear();
            if(ch==':') sstream << std::hex << word;
            else sstream<<word;
            sstream >> tmp;
            result.push_back(tmp);
        }

        return result;
    });
    if(source_address!="default")
    {
        auto source_vector = address_array(source_address,'.');
        for (int i = 0; i < 4; i++) arp_content_.source_ip_[i] = source_vector[i];
    }
    if(source_mac!="default")
    {
        auto source_vector = address_array(source_mac,'-');
        for (int i = 0; i < 6; i++) arp_content_.source_mac_[i] = source_vector[i];
    }
    auto target_vector = address_array(target_mac,':');
    for (int i = 0; i < 6; i++) arp_content_.target_mac_[i] = target_vector[i];

    target_vector = address_array(target_address,'.');
    for (int i = 0; i < 4; i++) arp_content_.target_ip_[i] = target_vector[i];

}
