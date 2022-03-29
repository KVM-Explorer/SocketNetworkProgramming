//
// Created by geek on 2022/3/14.
//
#include "../Chat/ChatServer.h"

int main()
{
    ChatServer server;
    server.Init();

    while (true)
    {
        server.Listen();
    }


}
