//
// Created by geek on 2022/3/14.
//

#include "ChatServer.h"
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

void ChatServer::Init() {

    sockaddr_in serve_address;
    serve_address.sin_family = AF_INET;
    serve_address.sin_addr.s_addr = htonl(INADDR_ANY);
    serve_address.sin_port = htons(port);

    if(bind(socket_,(struct sockaddr*)&serve_address,sizeof(serve_address))<0)
    {
        printf("Error in bind port\n");
    }
}

ChatServer::ChatServer() {
    socket_ = socket(PF_INET,SOCK_DGRAM,0);
    memset(buffer_,0,sizeof(buffer_));
}

ChatServer::~ChatServer() {

}

void ChatServer::Login(std::tuple<uint32_t, uint16_t> client) {
    using namespace std;
    ip2user[client] = string(buffer_);
    clients.push_back(client);
    string state = "Success";
    sendto(socket_,state.c_str(),state.length(),0,
           (struct sockaddr*)&client_address_,sizeof(client_address_));
}

void ChatServer::Logout() {

}

void ChatServer::Retransmit(std::string message, std::tuple <uint32_t, uint16_t> src) {

    using namespace std;
    cout<<ip2user[src]<<": "<<buffer_<<endl;
    message = ip2user[src]+": "+message;
    for (auto iter:clients)
    {
        if(iter == src) continue;
        sockaddr_in client;
        client.sin_family = AF_INET;
        client.sin_addr.s_addr = get<0>(iter);
        client.sin_port = get<1>(iter);
        int length =
                sendto(socket_,message.c_str(),message.length(),0,
                       (struct sockaddr*)&client,sizeof(client));

    }
}

void ChatServer::Listen() {
    using namespace std;
    memset(&client_address_,0,sizeof(client_address_));
    socklen_t address_length = sizeof(sockaddr_in);
    int length =
            recvfrom(socket_,buffer_,sizeof(buffer_),
                     0,(struct sockaddr*)&client_address_,
                             &address_length);

    buffer_[length] = 0;

    cout<<"IP address:"<<inet_ntoa(client_address_.sin_addr)<<' ';

    cout<<buffer_<<" Port: "<<ntohs(client_address_.sin_port)<<endl;

    auto address = client_address_.sin_addr.s_addr;
    auto port = client_address_.sin_port;
    auto client = tuple<uint32_t ,uint16_t>(address,port);
    if(ip2user.count(client)==0) Login(client);
    else    Retransmit(buffer_, client);
}
