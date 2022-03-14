#pragma once
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>

class ChatClient {
private:
    std::string username_;
    int socket_;
    struct sockaddr_in serve_address_;
    char buffer_[65535];

    const uint16_t source_port_ = 8888;
    const uint16_t target_port_ = 10308;

    uint32_t target_address_;


public:
    ChatClient();
    ~ChatClient();

    void Init();
    void SetAddress(std::string address);
    bool Login(std::string name);
    void SendMessage(std::string message);
    void Listen();
    bool Logout();


};

