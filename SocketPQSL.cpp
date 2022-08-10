#include "SocketPQSL.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>

SocketPQSL::SocketPQSL(const std::string& dbName,
    const std::string& user,
    const std::string& password,
    const std::string& port,
    const std::string& host)
    : dbname(dbName), user(user), password(password), port(port), host(host), socket(nullptr)
{

}

SocketPQSL::~SocketPQSL()
{
    socket=nullptr;
}

pqxx::result SocketPQSL::query(std::string strSQL) 
{
    //SetConnection();
    if (!reconnectDB()) {
        log("Can't exec query. Db is disconected.");
        return {};
    }
    pqxx::work trans(*socket, "trans");
    pqxx::result res = trans.exec(strSQL);
    trans.commit();
    return res;
}

void SocketPQSL::connectDB()
{
    try {
        socket = std::make_unique<pqxx::connection>("user = " + user + 
            " password = " + password + 
            " host = " + host + 
            " port = " + port + 
            " dbname=" + dbname);
        if (socket->is_open()) {
            log("Opened database successfully: " + dbname);
        }
        else {
            log("Can't open database");
            return;
        }
    }
    catch (const pqxx::sql_error& e) {
        log(e);
        log("Try to create database: " + dbname);
        createDB(dbname, user, password, port, host);
        return;
    }
    return ;
}

bool SocketPQSL::reconnectDB()
{   
    if (checkDB())
        return true;
    int retry = 3;
    connectDB();
    if (checkDB())
        return true;
    using namespace std::chrono_literals;
    while (bool(--retry) && !checkDB()) {
        log("Can't reconnect to databese. Attempts left"+std::to_string(retry)+" Retry after 5");
        for (int i = 5; i > 0; --i) {
            std::this_thread::sleep_for(1s);
            log(std::to_string(i));
        }
        connectDB();
    }
    assert(retry == 0 && !checkDB() && "Can't reconnect to databese!");
    if (checkDB())
        return true;
    return false;
}

bool SocketPQSL::checkDB()
{
    if (!socket)
        return false;
    return socket->is_open();
}

pqxx::result SocketPQSL::createTable(const std::string& tableName, const fields& flds)
{
    std::string strQuery = "CREATE TABLE " + tableName +" (";
    for (const auto& field : flds)
    {
        strQuery += field + ",";
    }
    strQuery.pop_back();
    strQuery += ");";
    try {
        auto r = query(strQuery);
        log("Create table success");
        return r;
    }
    catch (const pqxx::sql_error& e) {
        log(e);
        return {};
    }
}

pqxx::result SocketPQSL::writeData(const std::string& tableName, const std::string& data, const fields& flds)
{
    std::string strQuery = "INSERT INTO " + tableName + " VALUES " + data +";";
    try {
        auto r = query(strQuery);
        log("Wrtie data successful");
        return r;
    }
    catch (const pqxx::sql_error& e) {
        std::string sqlErrCode = e.sqlstate();
        log(e);
        if (sqlErrCode == "42P01")
        {
            log("Try to create table");
            createTable(tableName, flds);
            try {
                return query(strQuery);
            }
            catch (const pqxx::sql_error& e) {
                log(e);
                return {};
            }
        }
        
        return {};
    }
}

pqxx::result SocketPQSL::writeUser(const std::string& tableName, std::vector<User>& users)
{
    using namespace std::placeholders;
    if (users.empty())
        return pqxx::result();
    std::string data;

    for (const auto& user : users) {
        data += "("+std::to_string(std::get<0>(user)) + ",\'" +
            std::get<1>(user) + "\'," +
            std::to_string(std::get<2>(user)) + ",\'" +
            std::get<3>(user) + "\'),";
    }
    data.pop_back();
    return writeData(tableName,data,UserFields);
}

pqxx::result SocketPQSL::readDataFromTable(const std::string& tableName)
{
    try {
        std::string strQuery = " SELECT * FROM " + tableName + ";";
        return query(strQuery);
    }
    catch (const pqxx::sql_error& e) {
        log(e);
        return {};
    }
}

bool SocketPQSL::readUsers(const std::string& tableName, std::vector<User>& users)
{
    pqxx::result r=readDataFromTable(tableName);
    if (r.empty()) {
        log("User read failed table. Result is empty.");
        return false;
    }
    for (int i = 0; i < r.size(); ++i)
    {   
        pqxx::row row = r.at(i);
        if (row.size() < 4) {
            log("User read failed table. Row size <4.");
            return false;
        }
        users.push_back({ std::stoi(row.at(0).c_str()),
        row.at(1).c_str(),
        std::stoi(row.at(2).c_str()),
        row.at(3).c_str() });
    }
    log("Reading users successful");
    return true;
}


void SocketPQSL::log(const pqxx::sql_error& e)const
{
    std::cerr << "Database error: " << e.what()
        << ", error code: " << e.sqlstate()
        << "SQL Query was: " << e.query() << std::endl;
}

void SocketPQSL::log(const std::string& str) const
{
    std::cerr << str << std::endl;
}

void SocketPQSL::printToQuery(int i, std::string& q)
{
    q += std::to_string(i) + ",";
}

void SocketPQSL::printToQuery(double i, std::string& q)
{
    q += std::to_string(i) + ",";
}

void SocketPQSL::printToQuery(std::string i, std::string& q)
{
    q += "\'" + i + "\',";
}


int SocketPQSL::createDB(const std::string& dbName, const std::string& user, const std::string& password, const std::string& port, const std::string& host)
{
    try {
        pqxx::connection c("user = " + user + " password = "+ password +" host="+host+" port="+port);
        pqxx::nontransaction w(c);
        try {
            w.exec("CREATE DATABASE " + dbName);
            pqxx::result res(w.exec("GRANT ALL ON DATABASE " + dbName + " TO " + user));
            w.commit();
        }
        catch (pqxx::sql_error& e) {
            std::string sqlErrCode = e.sqlstate();
            if (sqlErrCode == "42P04") { // catch duplicate_database
                log("Database: " + dbName + " exists, proceeding further");
                return 0;
            }
            log(e);
            w.abort();
        }
        return 1;
    }
    catch (const pqxx::sql_error& e) {
        log(e);
        return 2;
    }
}
