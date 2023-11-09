#include "chat.h"

Request::~Request() { delete handler_; }

Handler* Request::getHandler() const { return handler_; }

const Dataset& Request::getDataset() const { return dataset_; }

void Request::setPathForRead(const fs::path& path) { pathForRead_ = path; }

void Request::makeRequest(Chat* chat, Request* request) { chat->makeRequest(request); }

void Request::exit(Chat* chat) { chat->active_ = false; }

SignUp::SignUp() { handler_ = new SignUpHandler(); }

void SignUp::request(Chat* chat)
{
    char action = '0';
    std::cout << "Sign up(1), sign in(2), exit(q): ";
    std::cin >> action;
    switch(action)
    {
    case '1':
        sign();
        if (dataset_[0] == "all" || !chat->send(this))
		    std::cout << "This login is already taken!\n";
	    else
		    std::cout << "Registration was successful!\n";
        break;
    case '2':
        makeRequest(chat, new SignIn());
        break;
    case 'q':
        exit(chat);
        break;
    default:
        std::cout << "Your command is unclear. Please, select an action from the list:\n";
    }
}

void SignUp::sign()
{
    std::string login;
	std::string password;
	std::cout << "Enter your login: ";
	std::cin >> login;
	std::cout << "Enter your password: ";
	std::cin >> password;
    dataset_.clear();
    dataset_.push_back(login);
    dataset_.push_back(password);
}

SignIn::SignIn() { handler_ = new SignInHandler(); }

void SignIn::request(Chat* chat)
{
    sign();
    if(!chat->send(this))
    {
        std::cout << "Invalid login or password!\n";
        makeRequest(chat, new SignUp());
    }
    else
    {
        std::cout << "Welcome to the chat room!\n";
        makeRequest(chat, new UsersDisplay(dataset_[0]));
    }
}

PubPost::PubPost(const std::string& sender)
{
    dataset_.push_back(sender);
    dataset_.push_back("all");
    handler_ = new PubPostHandler();
}

void PubPost::request(Chat* chat)
{
    char action = '0';
    std::cout << "Send message(1), read chat(2), dialog(3), display users(4), sign out(5), exit(q): ";
    std::cin >> action;
    switch(action)
    {
    case '1':
        if(!post())
            std::cout << "You can't send an empty message!\n";
        else
            chat->send(this);
        break;
    case '2':
        makeRequest(chat, new PubRead(dataset_[0]));
        break;
    case '3':
		std::cout << "Enter the recipient login: ";
		std::cin >> recipient_;
		if (recipient_ == dataset_[0])
        {
            std::cout << "The developer still believes that users should not send messages to themselves :)\n";
            break;
        }
        makeRequest(chat, new PvtPost(dataset_[0], recipient_));
        break;
    case '4':
        makeRequest(chat, new UsersDisplay(dataset_[0]));
        break;
    case '5':
        std::cout << "User " << dataset_[0] << " left the chat room.\n";
        makeRequest(chat, new SignUp());
        break;
    case 'q':
        exit(chat);
        break;
    default:
        std::cout << "Your command is unclear. Please, select an action from the list:\n";
    }
}

bool PubPost::post()
{
    std::string text;
	std::cout << "Message: ";
	std::getline(std::cin.ignore(), text);
    if(text.empty())
        return false;
    dataset_.push_back(text);
    dataset_.push_back(getCurrentTime());
    return true;
}

std::string PubPost::getCurrentTime()
{
    time_t now = time(nullptr);
	char buffer[20];
	strftime(buffer, sizeof(buffer), "%X %d/%m/%Y", localtime(&now));
	return buffer;
}

PubRead::PubRead(const std::string& user)
{
    dataset_.push_back(user);
    handler_ = new PubReadHandler();
}

void PubRead::request(Chat* chat)
{
    if(!chat->send(this))
        std::cout << "There are no messages in the chat room yet!\n";
    else
        read();
    makeRequest(chat, new PubPost(dataset_[0]));
}

void PubRead::read()
{
    std::ifstream in(pathForRead_);
	for (std::string line; std::getline(in, line);) 
    	std::cout << line << std::endl;
}

PvtPost::PvtPost(const std::string& sender, const std::string& recipient)
{
    dataset_.push_back(sender);
    dataset_.push_back(recipient);
    handler_ = new PvtPostHandler();
}

void PvtPost::request(Chat* chat)
{
    char action = '0';
    std::cout << "Send message(1), read conversation(2), back chat(3), exit(q): ";
    std::cin >> action;
    switch(action)
    {
    case '1':
        if(!post())
            std::cout << "You can't send an empty message!\n";
        else
        {
            if(!chat->send(this))
            {
                std::cout << "There is no user with this login in the chat room!\n";
                makeRequest(chat, new PubPost(dataset_[0]));
            }
        }
        break;
    case '2':
        makeRequest(chat, new PvtRead(dataset_[0], dataset_[1]));
        break;
    case '3':
        makeRequest(chat, new PubPost(dataset_[0]));
        break;
    case 'q':
        exit(chat);
        break;
    default:
        std::cout << "Your command is unclear. Please, select an action from the list:\n";
    }
}

PvtRead::PvtRead(const std::string& user1, const std::string& user2)
{
    dataset_.push_back(user1);
    dataset_.push_back(user2);
    handler_ = new PvtReadHandler();
}

void PvtRead::request(Chat* chat)
{
    if(!chat->send(this))
        std::cout << "There are no messages in this dialog yet!\n";
    else
        read();
    makeRequest(chat, new PvtPost(dataset_[0], dataset_[1]));
}

UsersDisplay::UsersDisplay(const std::string& user)
{
    dataset_.push_back(user);
    handler_ = new UsersDisplayHandler();
}

void UsersDisplay::request(Chat* chat)
{
    if(chat->send(this))
        showUsers();
    makeRequest(chat, new PubPost(dataset_[0]));
}

void UsersDisplay::showUsers()
{
    fs::path path;
	int number = 1;
	std::cout << "Now in chat room:\n";
    for (const auto& entry : fs::directory_iterator(pathForRead_))
    {
		path = entry;
		auto user = path.generic_string();
		user = user.substr(14);
		if(user == dataset_[0])
			std::cout << number++ << ") " << user << " | online\n";
		else
			std::cout << number++ << ") " << user << " | offline\n";
	}
}
