#pragma once

#include <pqxx/pqxx>
#include <memory>
#include "User.h"

class SocketPQSL
{
public:
    SocketPQSL(const std::string& dbName,
        const std::string& user,
        const std::string& password,
        const std::string& port,
        const std::string& host);
    ~SocketPQSL();
    pqxx::result query(std::string strSQL);

    void connectDB();
    bool reconnectDB();
    int createDB(const std::string& dbName, const std::string& user, const std::string& password, const std::string& port, const std::string& host);
    bool checkDB();
    pqxx::result createTable(const std::string& tableName,const fields& flds);
    pqxx::result writeData(const std::string& tableName, const std::string& data, const fields& flds);
    pqxx::result writeUser(const std::string& tableName, std::vector<User>& users);
    pqxx::result readDataFromTable(const std::string& tableName);
    bool readUsers(const std::string& tableName, std::vector<User>& users);

    void log(const pqxx::sql_error& e) const;
    void log(const std::string& str) const;
private:
    
    std::unique_ptr<pqxx::connection> socket;
    std::string dbname = "OtherTest";
    std::string user = "postgres";
    std::string password = "123";
    std::string host = "127.0.0.1";
    std::string port = "5432";

};

