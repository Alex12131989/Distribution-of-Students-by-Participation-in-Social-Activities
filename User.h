#pragma once
#include <experimental/filesystem>
#include <algorithm>
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
	struct user_info
	{
		std::string name = "";
		std::string incrypted_password = "";
		std::string participation_in_soc_activities = "";
		int authority = 0;
		std::string number = "";
		float gpa = 0;
		float income_per_fam_member = 0;
		int theme = 0;
	};

	static std::vector<user_info> users;
	User(std::string name, std::string password, int theme)
	{
		this->name = name;
		this->password = password;
		this->theme = theme;
		if (name == "Root Account")
			this->authority = 1;
		if (root == nullptr)
			GetAllUserInfos();
	}

	void SaveUserInfo();
	void FindUser();
	void AddNewUser();
	static void CreateAdminZero();

	static void Sort(std::vector<User::user_info>& users, int subject, bool ascending);
	std::string GetName();
	std::string GetParticipation();
	std::string GetNumber();
	int GetAuthority();
	float GetGPA();
	float GetIncomePerFamMember();
	int GetTheme();

	void SetParticipation(std::string participation);
	void SetNumber(std::string number);
	void SetGPA(float gpa);
	void SetIncomePerFamMember(float income);
	void SetTheme(int theme);
	void SetMaxGPA(int max_gpa);

private:
	//BT/BST
	struct Node
	{
		user_info data;
		Node* left;
		Node* right;
		Node(user_info new_user) : data(new_user), left(nullptr), right(nullptr) {}
	};
	Node* root = nullptr;
	static std::vector<float> Quicksort(std::vector<float> values, bool ascending);
	void UnitializeBT(Node* root);
	Node* Insert(Node* root, user_info new_user);
	std::vector<User::user_info> RepresentBTAsVector(Node* root);
	std::string name = "";
	std::string password = "";
	std::string participation_in_soc_activities = "None"; //i'm thinking something like, none, low, medium, high, whatever
	std::string number = "";
	float gpa = 0;
	float income_per_fam_member = 0;
	int authority = 0;
	int theme = 0;

	int MAX_GPA = 4;

	static const char NEXT_LINE_SYMBOL = ',';
	static const char NEXT_FIELD_SYMBOL = ':';
	static const std::streamsize CHUNK = 64;

	void GetAllUserInfos();
	std::string ApplyCipher(std::string password, std::string name, int option);
	void WriteSingleUserToFile(std::ofstream& file, user_info data);
	bool ReadSingleUserToFile(std::ifstream& file, user_info& user);
};