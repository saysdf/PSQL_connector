#include "SocketPQSL.h"
#include "ReaderCSV.h"
#include "WriterUser.h"
#include <iostream>
int main(int argc, char** argv)
{
    if (argc != 8) {
        std::cout << "input args : " << std::endl;
        std::cout << "[infile] [outfile] [dbname] [user] [password] [hostname] [port]" << std::endl;
        return 3;
    }
    ReaderCSV r;
    std::vector<User> users;
    if (r.read(argv[1], users) == false) {
        std::cout << "Can't open input file: "<< argv[1] << std::endl;
        return 1;
    }
    
    //SocketPQSL a("othertestg","postgres","123","5432","localhost");
    SocketPQSL a(argv[3], argv[4], argv[5], argv[7], argv[6]);
    a.connectDB();
    a.writeUser("UsersTable", users);
    users.clear();
    a.readUsers("UsersTable",users);
    WriterUser w;
    if (w.write(argv[2], users) == false)
        return 2;
    
    return 0;
}