#include "httpfileclient.h"

#include <iostream>

int main()
{
    HttpFileClient client;
    client.connectToServer("127.0.0.1", 8080);
    string message = client.getFileFromServer("test.mp3");

    std::cout<<message;

    return 0;
}
