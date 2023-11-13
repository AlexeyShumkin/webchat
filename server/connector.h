#pragma once
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "request.h"

#define BUF_SIZE 30
#define PORT 1666

class Connector
{
public:
    bool establish();
    const int getSocketFD() const;
    void passOn(Dataset& dataset);
    void take(Request* request);
private:
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int socket_file_descriptor, connection, bind_status, connection_status;
    char buffer[BUF_SIZE];
};