#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <iostream>
#include <fstream>
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
	}

	void SaveTheme(int theme);
	void SaveUserInfo(std::string user_info);
	int GetTheme();
	std::vector<std::string> GetUserInfo();

private:
	std::string name;
	std::string password;
	int theme;
};