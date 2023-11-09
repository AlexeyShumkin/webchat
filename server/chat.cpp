#include "chat.h"

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
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int socket_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Socket creation failed!\n";
        exit(1);
    }
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(bind_status == -1)  
    {
        std::cout << "Socket binding failed!\n";
        exit(1);
    }
    connection_status = listen(socket_file_descriptor, 5);
    if(connection_status == -1)
    {
        std::cout << "Socket is unable to listen for new connections!\n";
        exit(1);
    }  
    else 
        std::cout << "Server is listening for new connection: ";
    length = sizeof(client);
    connection = accept(socket_file_descriptor,(struct sockaddr*)&client, &length);
    if(connection == -1) 
    {
        std::cout << "Server is unable to accept the data from client!\n";
        exit(1);
    }
    std::cout << "Hello! You are welcome to register, or you can enter the chat room if you are already registered.\n";
    while(active_)
        request();
    close(socket_file_descriptor);
    std::cout << "Goodbye!\n";
}
