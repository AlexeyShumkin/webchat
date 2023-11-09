#pragma once
#include "handler.h"

class Chat;

class Request
{
public:
    virtual ~Request();
    virtual void request(Chat* chat) = 0;
    Handler* getHandler() const;
    const Dataset& getDataset() const;
    void setPathForRead(const fs::path& path);
protected:
    void makeRequest(Chat* chat, Request* request);
    void exit(Chat* chat);
    fs::path pathForRead_;
    Handler* handler_{ nullptr };
    Dataset dataset_;
};

class SignUp : public Request
{
public:
    SignUp();
    void request(Chat* chat) override;
    void sign();
};

class SignIn : public SignUp
{
public:
    SignIn();
    void request(Chat* chat) override;
};

class PubPost : public Request
{
public:
    PubPost() = default;
    PubPost(const std::string& sender);
    void request(Chat* chat) override;
    bool post();
    std::string getCurrentTime();
private:
    std::string recipient_{ "all" };
};

class PubRead : public Request
{
public:
    PubRead() = default;
    PubRead(const std::string& user);
    void request(Chat* chat) override;
    void read();
};

class PvtPost : public PubPost
{
public:
    PvtPost(const std::string& sender, const std::string& recipient);
    void request(Chat* chat) override;
};

class PvtRead : public PubRead
{
public:
    PvtRead(const std::string& user1, const std::string& user2);
    void request(Chat* chat) override;
};

class UsersDisplay : public Request
{
public:
    UsersDisplay() = default;
    UsersDisplay(const std::string& user);
    void request(Chat* chat) override;
    void showUsers();
};
