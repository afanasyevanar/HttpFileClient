#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 80

using namespace std;

string message = "GET / HTTP/1.1"
                 "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:63.0) Gecko/20100101 Firefox/63.0"
                 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
                 "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3"
                 "Accept-Encoding: gzip, deflate"
                 "Connection: keep-alive"
                 "Upgrade-Insecure-Requests: 1";

char buf[sizeof(message)];
char* reciveBuffer = new char[1024];

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); // или любой другой порт...
    addr.sin_addr.s_addr = inet_addr("5.255.255.55");

    cout<<"Try to connect"<<endl;
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    cout<<"Try to send"<<endl;
    send(sock, message.c_str(), message.length(), 0);


    auto recievedBytes = recv(sock, reciveBuffer, 1024, 0);
    cout<<"Get from server:"<<recievedBytes<<endl;
    cout<<reciveBuffer;


    printf(buf);
    close(sock);

    return 0;
}
