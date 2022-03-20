#pragma once
#include <sys/socket.h>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <string>
#include <tuple>
class ChatServer {

private:
    int socket_;
    std::map<std::tuple<uint32_t ,uint16_t >,std::string> ip2user;
    std::vector<std::tuple<uint32_t ,uint16_t > > clients;
    struct sockaddr_in client_address_;
    char buffer_[65535];

    const uint16_t port = 10308;
    const uint16_t clients_port_ = 9999;
public:
    ChatServer();
    ~ChatServer();

    void Init();
    void Listen();
    void Login(std::tuple<uint32_t, uint16_t> client);
    void Logout();
    void Retransmit(std::string message, std::tuple <uint32_t, uint16_t> src);

};


