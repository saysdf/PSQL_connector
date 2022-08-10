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
    void printToQuery(int i,std::string& q);
    void printToQuery(double i, std::string& q);
    void printToQuery(std::string i, std::string& q);
    
    //Don't forget pop last ','

    template <typename TupleT, std::size_t... Is>
    void printTupleManual(const TupleT& tp, std::index_sequence<Is...>, std::string& q) {
        (printToQuery(std::get<Is>(tp, q)), ...);
    }

    template <typename TupleT, std::size_t TupSize = std::tuple_size_v<TupleT>>
    void printTuple(const TupleT& tp, std::string& q) {
        printTupleManual(tp, std::make_index_sequence<TupSize>{},q);
    }

    std::unique_ptr<pqxx::connection> socket;
    std::string dbname = "OtherTest";
    std::string user = "postgres";
    std::string password = "123";
    std::string host = "127.0.0.1";
    std::string port = "5432";

};

