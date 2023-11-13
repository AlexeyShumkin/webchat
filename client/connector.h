#pragma once
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
    int socket_file_descriptor, connection;
    char buffer[BUF_SIZE];
};