#pragma once
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <string>
class TCP {
private:
    int tcp_socket_;

protected:
    uint8_t buffer_[65535];

public:
    TCP(std::string device,uint16_t source_port);
    ~TCP();

    void SetTarget(std::string target_address,uint16_t target_port);
    void SetSource(std::string source_address,uint16_t source_port);
    void Listen();
    uint16_t CheckSum(uint8_t *data,int length);
    void SendMessage(std::string message);
};


