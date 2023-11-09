#pragma once
#include "database.h"

class Request;

class Handler
{
public:
    virtual bool specHandle(Request* request, DataBase* database) = 0;
protected:
    size_t makeDialogID(const std::string& sender, const std::string& recipient);
	size_t hashFunction(const std::string& password);
};

class SignUpHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};

class SignInHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};

class PubPostHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};

class PubReadHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};

class PvtPostHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};

class PvtReadHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};

class UsersDisplayHandler : public Handler
{
public:
    bool specHandle(Request* request, DataBase* database) override;
};
