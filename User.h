#pragma once
#include <experimental/filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <string>
#include <vector>
#include <format>
#include <cmath>


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

	struct user_info
	{
		std::string name = "";
		std::string incrypted_password = "";
		std::string participation_in_soc_activities = "";
		int authority = 0;
		int number = 0;
		float gpa = 0;
		float income_per_fam_member = 0;
		int theme = 0;
	};
	static std::vector<user_info> users;

	void SaveUserInfo();
	void FindUser();
	static void AddNewUser(User new_user);
	static void CreateAdminZero();

	std::string GetName();
	std::string GetParticipation();
	int GetAuthority();
	int GetNumber();
	float GetGPA();
	float GetIncomePerFamMember();
	int GetTheme();

	void SetParticipation(std::string participation);
	void SetNumber(int place);
	void SetGPA(float gpa);
	void SetIncomePerFamMember(float income);
	void SetTheme(int theme);
	void SetMaxGPA(int max_gpa);

private:
	std::string name = "";
	std::string password = "";
	std::string participation_in_soc_activities = "None"; //i'm thinking something like, none, low, medium, high, whatever
	int number = 0;
	float gpa = 0;
	float income_per_fam_member = 0;
	int authority = 0;
	int theme = 0;

	int MAX_GPA = 4;

	static const char NEXT_LINE_SYMBOL = ',';
	static const char NEXT_FIELD_SYMBOL = ':';
	static const std::streamsize CHUNK = 64;

	static void GetAllUserInfos();
	std::string ApplyCipher(std::string password, std::string name, int option);
	void WriteSingleUserToFile(std::ofstream& file, user_info data);
	static bool ReadSingleUserToFile(std::ifstream& file, user_info& user);
};