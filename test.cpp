#include "DataLinkLayer/Ethernet.h"
#include "DataLinkLayer/CMSA2CD.h"
int main()
{
    /// Ethernet
//    char *buffer ="Hello World";
//    Ethernet ethernet("wlo1");
//    ethernet.SetMac("08:71:90:01:F8:E2");
//    ethernet.SetType(EthernetType::InternetProtocol);
//    ethernet.GenerateFrame((uint8_t *)buffer, 11);
//    ethernet.SendFrame();

    /// CSMA/CD
    CMSA2CD m1("This is Machine 1#",10,1);
    CMSA2CD m2("This is Machine 2#",10,2);
    CMSA2CD m3("This is Machine 3#",10,3);

    m1.CreateTask();
    m2.CreateTask();
    m3.CreateTask();

    m1.EndTask();
    m2.EndTask();
    m3.EndTask();
    int host_num = 20;
    std::vector<CMSA2CD> hosts;
    for(int i=0;i<host_num;i++)
    {
        hosts.push_back(CMSA2CD("Hello World",1,i));
    }
    for(auto iter = hosts.begin();iter!=hosts.end();iter++)
    {
        (*iter).CreateTask();
    }
    for(auto iter = hosts.begin();iter!=hosts.end();iter++)
    {
        (*iter).EndTask();
    }


}
