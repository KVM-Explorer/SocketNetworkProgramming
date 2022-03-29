//
// Created by geek on 2022/3/14.
//

#include <csignal>
#include <iostream>
#include <thread>
#include "../Chat/ChatClient.h"

int main()
{
    using namespace std;
    string name;
    ChatClient client;
    client.SetAddress("172.24.75.188");
    client.Init();
    cout<<"Hello,welcome to Chat client,Please set your username"<<endl;
    cin>>name;
    client.Login(name);
    auto watchdog = std::thread(
            [&](){
                while (true)
                {
                    client.Listen();
                }

            }
            );

    while (true)
    {
        string message;
//        char message[1024];
        cin>>message;
        client.SendMessage(message);
    }
    client.Logout();

}