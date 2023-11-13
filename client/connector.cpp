#include "connector.h"

bool Connector::establish()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file_descriptor == -1)
    {
        std::cout << "Creation of Socket failed!\n";
        return false;
    }
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(connection == -1)
    {
        std::cout << "Connection with the server failed!\n";
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
    ssize_t bytes = write(socket_file_descriptor, buffer, sizeof(buffer));
}

void Connector::take(Request* request)
{
    bzero(buffer, BUF_SIZE);
    read(socket_file_descriptor, buffer, sizeof(buffer));
    request->dataset_.clear();
    for(int i = 0; i < strlen(buffer); ++i)
    {
        std::string tmp;
        while(buffer[i] != '|')
            tmp.push_back(buffer[i++]);
        request->dataset_.push_back(tmp);
    }
}
