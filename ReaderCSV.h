#pragma once
#include "ReaderUser.h"
class ReaderCSV :
    public ReaderUser
{
public:
	ReaderCSV() : ReaderUser() {}
	~ReaderCSV() {}
	bool read(const std::string& filename, std::vector<User>& users) override;
private:
	int parceId(std::string& id) const override;
	std::string parceName(std::string& name) const override;
	int parceAge(std::string& age) const override;
	std::string parceAddress(std::string& address) const override;
};