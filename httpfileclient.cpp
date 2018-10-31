#include "httpfileclient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>

HttpFileClient::HttpFileClient()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("Error during socket creation");
    }
}

HttpFileClient::~HttpFileClient()
{
    close(sock);
}

bool HttpFileClient::connectToServer(string address, uint16_t port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());

    cout<<"Try to connect to server: "<<address<<":"<<port<<endl;
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error during connection");
        return false;
    }

    return true;

}

string HttpFileClient::getFileFromServer(string fileName)
{
    string message = "GET /"+fileName+" HTTP/1.1"
                     "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:63.0) Gecko/20100101 Firefox/63.0"
                     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
                     "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3"
                     "Accept-Encoding: gzip, deflate"
                     "Connection: keep-alive"
                     "Upgrade-Insecure-Requests: 1";

    cout<<"Try to send"<<endl;
    send(sock, message.c_str(), message.length(), 0);

    char* reciveBuffer = new char[4096];

    stringstream incomeMessage;
    while (1){
    auto recievedBytes = recv(sock, reciveBuffer, 4096, 0);
    if (recievedBytes==0) break;
    //cout<<"Get from server:"<<recievedBytes<<endl;
    incomeMessage<<string(reciveBuffer, recievedBytes);
    }
    auto pos = incomeMessage.str().find_first_of(string("\r\n\r\n"));
    string file = incomeMessage.str().substr(pos+2, incomeMessage.str().length() - pos - 2);

    return file;
}
