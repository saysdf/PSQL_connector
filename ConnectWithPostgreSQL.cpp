#include "SocketPQSL.h"
#include "ReaderCSV.h"
#include "WriterUser.h"
int main(int argc, char** argv)
{
    if (argc !=3)
        return 3;
    ReaderCSV r;
    std::vector<User> users;
    if (r.read(argv[1], users) == false)
        return 1;
    
    SocketPQSL a("othertestg","postgres","123","5432","localhost");
    a.connectDB();
    a.writeUser("UsersTable", users);
    users.clear();
    a.readUsers("UsersTable",users);
    WriterUser w;
    if (w.write(argv[2], users) == false)
        return 2;
    
    return 0;
}