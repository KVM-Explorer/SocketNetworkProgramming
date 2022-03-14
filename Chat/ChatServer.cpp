//
// Created by geek on 2022/3/14.
//

#include "ChatServer.h"
#include <cstring>
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

void ChatServer::Login() {

}

void ChatServer::Logout() {

}

void ChatServer::Retransmit(std::string message) {

}

void ChatServer::Listen() {
    socklen_t address_length = 0;
    int length =
            recvfrom(socket_,buffer_,sizeof(buffer_),
                     0,(struct sockaddr*)&client_address_,
                             &address_length);
    printf("Message: %s\n",buffer_);

}
