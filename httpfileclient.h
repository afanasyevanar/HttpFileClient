#ifndef HTTPFILECLIENT_H
#define HTTPFILECLIENT_H

#define BUFFER_SIZE 4096

#include <string>
#include <netinet/in.h>

using namespace std;

enum class Status{
    NOTFOUND = 404,
    OK=200
};

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
    int socket_;
    Status status_;

};

#endif // HTTPFILECLIENT_H
