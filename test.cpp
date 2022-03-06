#include "DataLinkLayer/Ethernet.h"

int main()
{
    char *buffer ="Hello World";
    Ethernet ethernet("wlo1");
    ethernet.SetMac("08:71:90:01:F8:E2");
    ethernet.SetType(EthernetType::InternetProtocol);
    ethernet.GenerateFrame((uint8_t *)buffer, 11);
    ethernet.SendFrame();
}
