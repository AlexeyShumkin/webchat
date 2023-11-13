#include "connector.h"
#include <iostream>

bool Connector::establish()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Socket creation failed!\n";
        return false;
    }
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(bind_status == -1)  
    {
        std::cout << "Socket binding failed!\n";
        return false;
    }
    connection_status = listen(socket_file_descriptor, 5);
    if(connection_status == -1)
    {
        std::cout << "Socket is unable to listen for new connections!\n";
        return false;
    }  
    length = sizeof(client);
    connection = accept(socket_file_descriptor,(struct sockaddr*)&client, &length);
    if(connection == -1) 
    {
        std::cout << "Server is unable to accept the data from client!\n";
        return false;
    }
    return true;
}

const int Connector::getSocketFD() const
{
    return socket_file_descriptor;
}

void Connector::passOn(Dataset& dataset)
{
    bzero(buffer, BUF_SIZE);
    int position = 0;
    for(auto& data : dataset)
    {
        data += '|';
        strcpy(buffer + position, data.c_str());
        position += data.size();
    }
    ssize_t bytes = write(connection, buffer, sizeof(buffer));
}

void Connector::take(Request* request)
{
    bzero(buffer, BUF_SIZE);
    read(connection, buffer, sizeof(buffer));
    request->dataset_.clear();
    for(int i = 0; i < strlen(buffer); ++i)
    {
        std::string tmp;
        while(buffer[i] != '|')
            tmp.push_back(buffer[i++]);
        request->dataset_.push_back(tmp);
    }
}
