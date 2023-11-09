#pragma once
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "request.h"

#define MESSAGE_LENGTH 1024
#define PORT 1666

class Request;

class Chat
{
public:
    Chat(DataBase* db);
    ~Chat();
    void run();
    void request();
    bool send(Request* request) const;
private:
    friend class Request; 
    void makeRequest(Request* request);
    bool active_{ true };
    Request* request_{ nullptr };
    DataBase* database_{ nullptr };
};
