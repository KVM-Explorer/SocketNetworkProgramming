#pragma once
#include <sys/socket.h>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <string>
class ChatServer {

private:
    int socket_;
    std::map<uint32_t ,std::string> ip2user;
    std::vector<uint32_t > clients;
    struct sockaddr_in client_address_;
    char buffer_[65535];

    const uint16_t port = 10308;
public:
    ChatServer();
    ~ChatServer();

    void Init();
    void Listen();
    void Login();
    void Logout();
    void Retransmit(std::string message);

};


