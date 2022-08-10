#pragma once
#include "User.h"
class ReaderUser
{
public:
	ReaderUser() {};
	virtual ~ReaderUser() {};
	virtual bool read(const std::string& filename, std::vector<User>& users) = 0;
	virtual int parceId(std::string& id) const = 0;
	virtual std::string parceName(std::string& name) const = 0;
	virtual int parceAge(std::string& age) const = 0;
	virtual std::string parceAddress(std::string& address) const = 0;
};

