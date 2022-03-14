//
// Created by geek on 2022/3/14.
//

#include <csignal>
#include "Chat/ChatClient.h"

int main()
{
    ChatClient client;
    client.SetAddress("127.1.12.1");
    client.Init();
    while (true)
    {
        client.SendMessage("Error");
        sleep(2);
    }
}