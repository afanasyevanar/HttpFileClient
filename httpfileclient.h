#ifndef HTTPFILECLIENT_H
#define HTTPFILECLIENT_H

#include <string>
#include <netinet/in.h>

using namespace std;

class HttpFileClient
{
public:
    HttpFileClient();
    ~HttpFileClient();
    string getAddress() const;
    void setAddress(const string &value);
    bool connectToServer(string address, uint16_t port);
    string getFileFromServer(string fileName);
    bool getFileFromServerAndSave(string fileName, string fullPath);

private:

    int sock;

};

#endif // HTTPFILECLIENT_H
