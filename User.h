#pragma once
#include <experimental/filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <format>


class User
{
public:
	User(std::string name, std::string password, int theme)
	{
		this->name = name;
		this->password = password;
		this->theme = theme;
		if (name == "Root Account")
			this->authority = 1;
		if (users.size() == 0)
			GetAllUserInfos();
	}

	void SaveUserInfo();
	void FindUser();
	static void AddNewUser(User new_user);
	static void CreateAdminZero();

private:
	struct user_info
	{
		std::string name = "";
		std::string incrypted_password = "";
		int authority = 0;
		int theme = 0;
	};
	static std::vector<user_info> users;
	std::string name;
	std::string password;
	int authority = 0;
	int theme;

	static const char NEXT_LINE_SYMBOL = ',';
	static const char NEXT_FIELD_SYMBOL = ':';
	static const std::streamsize CHUNK = 64;

	static void GetAllUserInfos();
	std::string ApplyCipher(std::string password, std::string name, int option);
	void WriteSingleUserToFile(std::ofstream& file, user_info data);
	static bool ReadSingleUserToFile(std::ifstream& file, user_info& user);
};