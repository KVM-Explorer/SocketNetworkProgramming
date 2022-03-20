
#include "ChatClient.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

ChatClient::ChatClient() {
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    memset(buffer_, 0, sizeof(buffer_));
    memset(&serve_address_, 0, sizeof(serve_address_));
    username_ = "guest";

}

void ChatClient::SetAddress(std::string address) {
    target_address_ = inet_addr(address.c_str());
}

bool ChatClient::Login(std::string name) {
    using namespace std;
    username_ = name;
    int length = sendto(socket_, name.c_str(), name.length(), 0,
                        (struct sockaddr *) &serve_address_, sizeof(serve_address_));
    if (length < 0) std::cout << "Set username failed" << std::endl;
    length = recvfrom(socket_, buffer_, sizeof(buffer_), 0, NULL, NULL);
    if (length < 0) std::cout << "Set username failed" << std::endl;
    buffer_[length] = 0;
    std::string tmp_str = std::string(buffer_);
    cout<<tmp_str<<endl;
    if(tmp_str=="Success")
    {
        cout<<"Login Successful"<<endl;
        return true;
    }
    return false;
}

void ChatClient::Init() {

    serve_address_.sin_family = AF_INET;
    serve_address_.sin_addr.s_addr = target_address_;
    serve_address_.sin_port = htons(target_port_);
}

void ChatClient::SendMessage(std::string message) {
    int length = sendto(socket_, message.c_str(), message.length(), 0,
                        (struct sockaddr *) &serve_address_, sizeof(serve_address_));
    if (length <= 0) {
        printf("Send failed\n");
    } else {
//        printf("Send success: Length:%d\n", length);
    }
}


void ChatClient::Listen() {
    int length = recvfrom(socket_, buffer_, sizeof(buffer_), 0, NULL, NULL);
//    printf("Message: %d\n", length);
    buffer_[length] = 0;
    printf("%s\n",buffer_);
}

bool ChatClient::Logout() {
    using namespace std;
    string state = "Logout";
    int length = sendto(socket_, state.c_str(), state.length(), 0,
                        (struct sockaddr *) &serve_address_, sizeof(serve_address_));
    if (length < 0) std::cout << "Logout failed" << std::endl;
    length = recvfrom(socket_, buffer_, sizeof(buffer_), 0, NULL, NULL);
    if (length < 0) std::cout << "Logout failed" << std::endl;
    buffer_[length] = 0;
    std::string tmp_str = std::string(buffer_);
    cout<<tmp_str<<endl;
    if(tmp_str=="Success")
    {
        cout<<"Logout Successful"<<endl;
        return true;
    }
    return false;
}

ChatClient::~ChatClient() {
    close(socket_);
}

std::string ChatClient::GetName()
{
    return username_;
}
