#include <sstream>
#include <vector>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include "ArpReply.h"

ArpReply::ArpReply(std::string device) : Arp(device)
{
    auto tmp_socket = socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
    memset(buffer_,0,sizeof (buffer_));
    ifreq ifreq_mac,ifreq_ip;
    memset(&ifreq_mac,0,sizeof(ifreq_mac));
    memset(&ifreq_ip,0,sizeof (ifreq_ip));
    printf("Device Name: %s\n",device.c_str());
    strncpy(ifreq_ip.ifr_name,device.c_str(),IFNAMSIZ-1);
    strncpy(ifreq_mac.ifr_name,device.c_str(),IFNAMSIZ -1);

    if(ioctl(tmp_socket,SIOCGIFHWADDR,&ifreq_mac)<0)
    {
        printf("error in index ioctl reading\n");
    }
    if(ioctl(tmp_socket,SIOCGIFADDR,&ifreq_ip)<0)
    {
        printf("error in IP ioctl reading\n");
    }
    memcpy(arp_content_.target_mac_, ifreq_mac.ifr_hwaddr.sa_data, ETH_ALEN);
    auto ip_address = (((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr);
    memcpy(arp_content_.target_ip_,&ip_address,sizeof(ip_address));

    this->SetAttribute(ARPType::Reply);
}

ArpReply::~ArpReply() {}

void ArpReply::SetIp2Mac(std::string target_address, std::string target_mac, std::string source_address,
                         std::string source_mac)
{
    ethernet_.SetMac(target_mac);

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
        for (int i = 0; i < 4; i++) arp_content_.target_ip_[i] = source_vector[i];
    }
    if(source_mac!="default")
    {
        auto source_vector = address_array(source_mac,'-');
        for (int i = 0; i < 6; i++) arp_content_.target_mac_[i] = source_vector[i];
    }
    auto target_vector = address_array(target_mac,':');
    for (int i = 0; i < 6; i++) arp_content_.source_mac_[i] = target_vector[i];

    target_vector = address_array(target_address,'.');
    for (int i = 0; i < 4; i++) arp_content_.source_ip_[i] = target_vector[i];
}

void ArpReply::Init() {
    SetAttribute(ARPType::Reply);
}

