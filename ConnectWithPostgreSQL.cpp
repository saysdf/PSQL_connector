#include "SocketPQSL.h"
#include "ReaderCSV.h"
#include "WriterUser.h"
int main()
{
    ReaderCSV r;
    std::vector<User> users;
    if (r.read("C:\\Users\\Oleg\\Documents\\Dev\\UtopiaTest\\users.csv", users) == false)
        return 1;
    
    SocketPQSL a("othertestg","postgres","123","5432","localhost");
    a.connectDB();
    a.writeUser("UsersTable", users);
    users.clear();
    a.readUsers("UsersTable",users);
    WriterUser w;
    if (w.write("C:\\Users\\Oleg\\Documents\\Dev\\UtopiaTest\\users1.json", users) == false)
        return 2;
    
    return 0;
}