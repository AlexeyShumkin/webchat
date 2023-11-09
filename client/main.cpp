#include "chat.h"
 
int main()
{
    // system("echo `date +'%H:%M:%S %d/%m/%Y'` >> data/msgData");
    LocalDB db;
    Chat chat(&db);
    chat.run();
}
