#pragma once
#include "User.h"
#include <fstream>
class WriterUser
{
public:
	WriterUser() {};
	virtual ~WriterUser() {};
	virtual bool write(const std::string& filename, std::vector<User>& users) {
		try {
			std::ofstream infile(filename);
			if (!infile.is_open())
				return false;
			for (auto& user : users) {
				infile << "{\n\t\"id\":" << std::get<0>(user) <<
					",\n\t\"name\":\"" << std::get<1>(user) <<
					"\",\n\t\"age\":" << std::get<2>(user) <<
					",\n\t\"address\": \n\t" <<
					std::get<3>(user) << "\n}\n";
			}
			return true;
		}
		catch (...) {
			return false;
		}
	}
};

