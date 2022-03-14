
#include "ChatClient.h"
#include <cstring>
#include <arpa/inet.h>

ChatClient::ChatClient() {
    socket_ = socket(PF_INET,SOCK_DGRAM,0);
    memset(buffer_,0,sizeof(buffer_));
    memset(&serve_address_,0,sizeof(serve_address_));
    username_ = "guest";

}

void ChatClient::SetAddress(std::string address)
{
    target_address_ = inet_addr(address.c_str());
}

bool ChatClient::Login(std::string name)
{
    username_ = name;

}

void ChatClient::Init() {

    serve_address_.sin_family = AF_INET;
    serve_address_.sin_addr.s_addr = target_address_;
    serve_address_.sin_port = htons(target_port_);

//    if(bind(socket_,(struct sockaddr*)&serve_address_,sizeof(serve_address_))<0)
//    {
//        printf("Error in bind port\n");
//    }
}

void ChatClient::SendMessage(std::string message)
{
    int length = sendto(socket_,message.c_str(),message.length(),0,
           (struct sockaddr*)&serve_address_,sizeof(serve_address_));
    if(length<=0)
    {
        printf("Send failed\n");
    }else
    {
        printf("Send success: Length:%d\n",length);
    }
}


void ChatClient::Listen()
{
    int length = recvfrom(socket_,buffer_,sizeof(buffer_),0,NULL,NULL);
    printf("Message: %d\n",length);
}

bool ChatClient::Logout() {
    return false;
}

ChatClient::~ChatClient() {

}
