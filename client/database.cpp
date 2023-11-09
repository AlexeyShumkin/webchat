#include "database.h"
#include "request.h"

const fs::path& DataBase::getUserDataPath() const { return userDataPath_; }

const fs::path& DataBase::getMsgDataPath() const { return msgDataPath_; }

LocalDB::LocalDB()
{
    fs::create_directory("data");
	fs::create_directory(userDataPath_);
	fs::create_directory(msgDataPath_);
}

bool LocalDB::handle(Request* request)
{
    handler_ = request->getHandler();
    return handler_->specHandle(request, this);
}
