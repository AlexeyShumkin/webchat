#include "handler.h"
#include "request.h"

size_t Handler::makeDialogID(const std::string& sender, const std::string& recipient)
{
    size_t i = 0;
    size_t res = 0;
    size_t sum = sender.size() + recipient.size();
    while(i < sender.size() && i < recipient.size())
    {
        res += sender[i] + recipient[i];
        ++i;
    }
    return res <<= sum;
}

size_t Handler::hashFunction(const std::string& password)
{
    size_t i = 0;
    size_t j = password.size() - 1;
    size_t res = 0;
    while(i < j)
        res += password[i++] << password[j--];
    return res;
}

bool SignUpHandler::specHandle(Request* request, DataBase* database)
{
    if(!fs::exists(database->getUserDataPath() / request->getDataset()[0]))
    {
        size_t hash = hashFunction(request->getDataset()[1]);
        std::fstream fst;
        fst.open(database->getUserDataPath() / request->getDataset()[0], std::fstream::out);
        if(fst.is_open())
        {
            fst << hash;
            fst.close();
            return true;
        }
    }
    return false;
}

bool SignInHandler::specHandle(Request* request, DataBase* database)
{
    if(fs::exists(database->getUserDataPath() / request->getDataset()[0]))
    {
        size_t hash = hashFunction(request->getDataset()[1]);
        size_t tmp = 0;
        std::fstream fst;
        fst.open(database->getUserDataPath() / request->getDataset()[0], std::fstream::in);
        if(fst.is_open())
        {
            fst >> tmp;
            fst.close();
        }
        if(hash == tmp)
            return true;
    }
    return false;
}

bool PubPostHandler::specHandle(Request* request, DataBase* database)
{
    std::fstream fst;
    fst.open(database->getMsgDataPath() / request->getDataset()[1], std::fstream::app |  std::fstream::out);
    if(fst.is_open())
    {
        fst << request->getDataset()[0] 
        << " -> all | " << request->getDataset()[2] 
        << " | " << request->getDataset()[3] << '\n';
        fst.close();
        return true;
    }
    return false;
}

bool PubReadHandler::specHandle(Request* request, DataBase* database)
{
    if(!fs::exists(database->getMsgDataPath() / "all"))
        return false;
    request->setPathForRead(database->getMsgDataPath() / "all");
    return true;
}

bool PvtPostHandler::specHandle(Request* request, DataBase* database)
{
    if(!fs::exists(database->getUserDataPath() / request->getDataset()[1]))
        return false;
    std::fstream fst;
    auto dialog = std::to_string(makeDialogID(request->getDataset()[0], request->getDataset()[1]));
    fst.open(database->getMsgDataPath() / dialog, std::fstream::app |  std::fstream::out);
    if(fst.is_open())
    {
        fst << request->getDataset()[0] << " -> " 
        << request->getDataset()[1] << " | " 
        << request->getDataset()[2] << " | " 
        << request->getDataset()[3] <<'\n';
        fst.close();
        return true;
    }
    return false;
}

bool PvtReadHandler::specHandle(Request* request, DataBase* database)
{
    auto dialog = std::to_string(makeDialogID(request->getDataset()[0], request->getDataset()[1]));
    if(!fs::exists(database->getMsgDataPath() / dialog))
        return false;
    request->setPathForRead(database->getMsgDataPath() / dialog);
    return true;
}

bool UsersDisplayHandler::specHandle(Request* request, DataBase* database)
{
    if(!fs::exists(database->getUserDataPath()))
        return false;
    request->setPathForRead(database->getUserDataPath());
    return true;
}
