#pragma once
#include "request.h"

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
