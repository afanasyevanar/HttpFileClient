#include "httpfileclient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <fstream>

HttpFileClient::HttpFileClient()
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_ < 0)
    {
        perror("Error during socket creation");
    }
}

HttpFileClient::~HttpFileClient()
{
    close(socket_);
}

bool HttpFileClient::connectToServer(string address, uint16_t port)
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());

    cout<<"Try to connect to server: "<<address<<":"<<port<<endl;
    if(connect(socket_, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error during connection");
        return false;
    }
    cout<<"Connected successfully"<<endl;

    return true;
}

string HttpFileClient::getFileFromServer(string fileName){
    string message = "GET /"+fileName+" HTTP/1.1";

    send(socket_, message.c_str(), message.length(), 0);

    char* reciveBuffer = new char[BUFFER_SIZE];

    stringstream incomeMessage;
    while (1){
        auto recievedBytes = recv(socket_, reciveBuffer, BUFFER_SIZE, 0);
        if (recievedBytes==0) break;
        //cout<<"Get from server:"<<recievedBytes<<endl;
        incomeMessage<<string(reciveBuffer, recievedBytes);
    }

    auto firstSpace = incomeMessage.str().find_first_of(' ');
    auto secondSpace = incomeMessage.str().find_first_of(' ', firstSpace+1);

    status_ = static_cast<Status>(stoi(incomeMessage.str().substr(firstSpace, secondSpace-firstSpace)));

    auto pos = incomeMessage.str().find("\r\n\r\n", 0); // пустая строка между заголовком и телом

    string file = incomeMessage.str().substr(pos+4, incomeMessage.str().length() - pos - 4);

    return file;
}

bool HttpFileClient::getFileFromServerAndSave(string fileName, string fullPath)
{
    string file = getFileFromServer(fileName);
    ofstream of;

    switch (status_) {
    case Status::OK:

        of.open(fullPath);

        if (of){
            of<<file;
            of.close();

            cout<<"Recieved file has been saved as "<<fullPath<<endl;
            return true;

        }else{
            cout<<"Couldn't write to file: "<<fullPath;
        }

        break;

    case Status::NOTFOUND:
        cout<<file;
        return false;
        break;

    default:
        cout<<"Error in file recieving"<<endl;
        return false;
    }

    return false;
}
