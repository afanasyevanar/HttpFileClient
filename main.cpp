#include "httpfileclient.h"

#include <iostream>


int main()
{
    HttpFileClient client;
    client.connectToServer("127.0.0.1", 8080);
    client.getFileFromServerAndSave("test.mp3","/home/anton/test/recieved.mp3");

    return 0;
}
