#include "chat.h"
 
int main()
{
    LocalDB db;
    Chat chat(&db);
    chat.run();
}
