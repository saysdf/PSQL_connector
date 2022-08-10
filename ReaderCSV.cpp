#include "ReaderCSV.h"
#include <fstream>

bool ReaderCSV::read(const std::string& filename, std::vector<User>& users)
{
    try {
        std::string oneUser;
        std::ifstream infile(filename);
        if (!infile.is_open())
            return false;
        std::string line;
        line.reserve(200);//a little bit more than one user. For better experience may move it to settings.
        std::getline(infile, line);
        while (std::getline(infile, line))
        {
            oneUser.append(line);
            if (line.find("}\"") != std::string::npos)
            {
                auto id = parceId(oneUser);
                auto name = parceName(oneUser);
                auto age = parceAge(oneUser);
                auto address = parceAddress(oneUser);
                users.push_back(std::make_tuple(id, name, age, address));
                oneUser.clear();
            }
        }
        return true;
    }
    catch (std::exception& e) {
        return false;
    }
}

int ReaderCSV::parceId(std::string& s)const
{
    //id, name,age, "{json}"
    auto index = s.find(',');
    int id = std::stoi(s.substr(0, index));
    s = s.substr(index+2,s.size()-index);
    return id;
}

std::string ReaderCSV::parceName(std::string& s)const
{
    //name,age, "{json}"
    auto index = s.find(',');
    std::string name = s.substr(0, index);
    s = s.substr(index+1, s.size() - index);
    return name;
}

int ReaderCSV::parceAge(std::string& s)const
{
    //age, "{json}"
    auto index = s.find(',');
    int age = std::stoi(s.substr(0, index));
    s = s.substr(index+1, s.size() - index);
    return age;
}

std::string ReaderCSV::parceAddress(std::string& address)const
{
    auto i = address.find_first_of('\"');
    auto j = address.find_last_of('\"');
    if (i != std::string::npos && j != std::string::npos)
    {
        address = address.substr(i+1,(j-i)-1);
    }
    auto p = address.find("\"\"");
    while (p != std::string::npos)
    {
        address.replace(p, 2, "\"");
        p = address.find("\"\"");
    }
    return address;
}
