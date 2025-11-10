#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include <experimental/filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <format>

#include "cipher.h"


class User
{
public:
	User(std::string name, std::string password, int theme, bool update_users_info)
	{
		this->name = name;
		this->password = password;
		this->theme = theme;
		if (name == "Root Account")
			this->authority = 1;
		if (update_users_info)
			GetAllUserInfos();
	}

	void SaveUserInfo();
	void LoadUserInfo();
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