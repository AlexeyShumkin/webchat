#include "chat.h"

#define MESSAGE_LENGTH 1024
#define PORT 7777

Chat::Chat(DataBase* database) : database_{ database } { request_ = new SignUp(); }

Chat::~Chat() { delete request_; }

void Chat::request() { request_->request(this); }

bool Chat::send(Request* request) const { return database_->handle(request); }

void Chat::makeRequest(Request* request)
{
    delete request_;
    request_ = request;
}

void Chat::run()
{
    int socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;
    char message[MESSAGE_LENGTH];
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Socket creation failed!\n";
        exit(1);
    }
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(connection == -1)
    {
        std::cout << "Connection with the server failed!\n";
        exit(1);
    }
    std::cout << "Hello! You are welcome to register, or you can enter the chat room if you are already registered.\n";
    while(active_)
    {
        request();
    }
    close(socket_file_descriptor);
    std::cout << "Goodbye!\n";
}
