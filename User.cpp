#include "Exception.h"
#include "cipher.h"
#include "User.h"

std::vector<User::user_info> User::users;
using namespace std::experimental::filesystem;

std::vector<float> User::Quicksort(std::vector<float> values, bool ascending)
{
	if (values.size() <= 1)
		return values;

	float pivot = values[values.size() - 1], j = -1;
	for (size_t i = 0; i < values.size() - 1; i++)
		if (ascending)
		{
			if (values[i] < pivot)
			{
				j++;
				std::swap(values[j], values[i]);
			}
		}
		else
			if (values[i] > pivot)
			{
				j++;
				std::swap(values[j], values[i]);
			}

	std::swap(values[j + 1], values[values.size() - 1]);

	std::vector<float> left(values.begin(), values.begin() + (j + 1));
	std::vector<float> right(values.begin() + (j + 2), values.end());

	left = Quicksort(left, ascending);
	right = Quicksort(right, ascending);

	left.push_back(pivot);
	left.insert(left.end(), right.begin(), right.end());
	return left;
}

void User::UnitializeBT(Node* root)
{
	if (root != nullptr)
	{
		if (root->left != nullptr)
			UnitializeBT(root->left);
		if (root->right != nullptr)
			UnitializeBT(root->right);
		delete root;
	}
} 

User::Node*User::Insert(Node* root, user_info new_user)
{
	if (root == nullptr)
		return new Node(new_user);

	if (new_user.name < root->data.name)
		root->left = Insert(root->left, new_user);
	else if (new_user.name > root->data.name)
		root->right = Insert(root->right, new_user);
	else
		throw Exception("User already exists");

	return root;
}

std::vector<User::user_info> User::RepresentBTAsVector(Node* root)
{
	std::vector<user_info> return_vector;
	if (root != nullptr)
		return_vector.push_back(root->data);

	if (root->left != nullptr)
	{
		std::vector<user_info> traversed_vector = RepresentBTAsVector(root->left);
		return_vector.insert(return_vector.end(), traversed_vector.begin(), traversed_vector.end());
	}
	if (root->right != nullptr)
	{
		std::vector<user_info> traversed_vector = RepresentBTAsVector(root->right);
		return_vector.insert(return_vector.end(), traversed_vector.begin(), traversed_vector.end());
	}
	return return_vector;
}

void User::CreateAdminZero()
{
	//this code would be given to the customer, and potentially be changed for every new copy
	auto admin_zero = new User("Root Account", "", 0);
	try
	{
		admin_zero->AddNewUser();
	}
	catch (Exception exception) {	/*only if someone ran this app already*/ }
}

void User::WriteSingleUserToFile(std::ofstream& file, user_info data)
{
	file.write(reinterpret_cast<const char*>(&data.authority), sizeof(data.authority));
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(data.name.data(), data.name.length());
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(data.incrypted_password.data(), data.incrypted_password.length());
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(data.participation_in_soc_activities.data(), data.participation_in_soc_activities.length());
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(data.number.data(), data.number.length());
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(reinterpret_cast<const char*>(&data.gpa), sizeof(data.gpa));
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(reinterpret_cast<const char*>(&data.income_per_fam_member), sizeof(data.income_per_fam_member));
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));

	file.write(reinterpret_cast<const char*>(&data.theme), sizeof(data.theme));
	file.write(reinterpret_cast<const char*>(&NEXT_LINE_SYMBOL), sizeof(NEXT_LINE_SYMBOL));
}

bool User::ReadSingleUserToFile(std::ifstream& file, user_info& user)
{
	char buffer = '\0';

	file.read(reinterpret_cast<char*>(&user.authority), sizeof(user.authority));
	for(int i = 0; i < 2; ++i)
		file.read(&buffer, 1);					//since next char is ':' and we need buffer not being equal to it
	if (file.eof())
		return false;

	while (buffer != NEXT_FIELD_SYMBOL)
	{
		user.name += buffer;
		file.read(&buffer, 1);
	}
	file.read(&buffer, 1);

	while (buffer != NEXT_FIELD_SYMBOL)
	{
		user.incrypted_password += buffer;
		file.read(&buffer, 1);
	}
	file.read(&buffer, 1);

	while (buffer != NEXT_FIELD_SYMBOL)
	{
		user.participation_in_soc_activities += buffer;
		file.read(&buffer, 1);
	}
	file.read(&buffer, 1);

	while (buffer != NEXT_FIELD_SYMBOL)
	{
		user.number += buffer;
		file.read(&buffer, 1);
	}

	file.read(reinterpret_cast<char*>(&user.gpa), sizeof(user.gpa));
	file.read(&buffer, 1);

	file.read(reinterpret_cast<char*>(&user.income_per_fam_member), sizeof(user.income_per_fam_member));
	file.read(&buffer, 1);

	file.read(reinterpret_cast<char*>(&user.theme), sizeof(user.theme));
	file.read(&buffer, 1);						//the next line symbol
	
	return true;
}

void User::SaveUserInfo()
{
	bool rewritten = false;
	std::string incrypted_password = ApplyCipher(password, name, 0);
	path working_path = current_path();
	working_path /= "Users";
	create_directory(working_path);
	working_path /= "user_info.bin";
	std::ofstream rewrite_file(working_path, std::ios::binary | std::ios::out | std::ios::trunc);
	for (user_info user : users)
	{
		if (user.name == name)
		{
			auto temp_struct = user_info{ .name = name, .incrypted_password = incrypted_password,
				.participation_in_soc_activities = participation_in_soc_activities,  .authority = authority, 
				.number = number, .gpa = gpa, .income_per_fam_member = income_per_fam_member, .theme = theme};
			WriteSingleUserToFile(rewrite_file, temp_struct);
			rewritten = true;
		}
		else
		{
			WriteSingleUserToFile(rewrite_file, user);
		}
	}
	if (!rewritten)
	{
		auto temp_struct = user_info{ .name = name, .incrypted_password = incrypted_password,
				.participation_in_soc_activities = participation_in_soc_activities,  .authority = authority,
				.number = number, .gpa = gpa, .income_per_fam_member = income_per_fam_member, .theme = theme };
		WriteSingleUserToFile(rewrite_file, temp_struct);
	}
	rewrite_file.close();
	GetAllUserInfos();
}

void User::FindUser()
{
	bool found = false, verified = false;
	for (size_t i = 0; i < users.size(); ++i)
		if (users[i].name == name)
		{
			found = true;
			authority = users[i].authority;
			std::string pulled_password = ApplyCipher(users[i].incrypted_password, users[i].name, 1);
			if (password == pulled_password)
				verified = true;
			number = users[i].number;
			income_per_fam_member = users[i].income_per_fam_member;
			gpa = users[i].gpa;
			participation_in_soc_activities = users[i].participation_in_soc_activities;
			theme = users[i].theme;
			i = users.size();
		}
	if (!found)
		throw Exception("Could not find the user");
	else if (!verified)
		throw Exception("Wrong password");
}

void User::ReloadUser()
{
	bool found = false;
	for (size_t i = 0; i < users.size(); ++i)
		if (users[i].name == name)
		{
			found = true;
			authority = users[i].authority;
			std::string pulled_password = ApplyCipher(users[i].incrypted_password, users[i].name, 1);
			password = pulled_password;
			number = users[i].number;
			income_per_fam_member = users[i].income_per_fam_member;
			gpa = users[i].gpa;
			participation_in_soc_activities = users[i].participation_in_soc_activities;
			theme = users[i].theme;
			i = users.size();
		}
	if (!found)
		throw Exception("Could not find the user");
}

void User::AddNewUser()
{
	for (user_info user : users)
		if (user.name == name)
			throw Exception("User already exists");
	SaveUserInfo();
}

void User::DeleteUser(user_info user_to_delete)
{
	path working_path = current_path();
	working_path /= "Users";
	create_directory(working_path);
	working_path /= "user_info.bin";
	std::ofstream rewrite_file(working_path, std::ios::binary | std::ios::out | std::ios::trunc);
	for (user_info user : users)
	{
		if (user_to_delete.name != user.name)
		{
			WriteSingleUserToFile(rewrite_file, user);
		}
	}
	rewrite_file.close();
	User* access_user = new User("", "", 0);
	access_user->GetAllUserInfos();
}

void User::GetAllUserInfos()
{
	path working_path = current_path();
	working_path /= "Users";
	create_directory(working_path);
	working_path /= "user_info.bin";
	std::ifstream working_file(working_path, std::ios::binary);

	user_info user;
	UnitializeBT(root);
	root = nullptr;
	while (ReadSingleUserToFile(working_file, user))
	{
		root = Insert(root, user);
		user = {};
	}
	working_file.close();
	users = RepresentBTAsVector(root);
}

std::string User::ApplyCipher(std::string password, std::string name, int option)
{
	char* key = GetKey(name.data());
	switch (option)
	{
	case 0:
		Encode(key, password.data());
		break;
	case 1:
		Decode(key, password.data());
		break;
	}
	return password;
}

void User::Sort(std::vector<User::user_info>& users, int subject, bool ascending)
{
	std::vector<float> values;
	for (User::user_info user : users)
	{
		switch (subject)
		{
		case 0:
			values.push_back(user.gpa);
			break;
		case 1:
			values.push_back(user.income_per_fam_member);
			break;
		}
	}
	values = Quicksort(values, ascending);
	for (size_t i = 0; i < values.size(); ++i)
		for (size_t j = i; j < users.size(); ++j)
			switch (subject)
			{
			case 0:
				if (users[j].gpa == values[i])
				{
					std::swap(users[j], users[i]);
					j = users.size();
				}
				break;
			case 1:
				if (users[j].income_per_fam_member == values[i])
				{
					std::swap(users[j], users[i]);
					j = users.size();
				}
				break;
			}
}

std::string User::GetName()
{
	return name;
}

std::string User::GetPassword()
{
	return password;
}

std::string User::GetParticipation()
{
	return participation_in_soc_activities;
}

int User::GetAuthority()
{
	return authority;
}

std::string User::GetNumber()
{
	return number;
}

float User::GetGPA()
{
	return std::round(gpa*100)/100;
}

float User::GetIncomePerFamMember()
{
	return std::round(income_per_fam_member * 100)/100;
}

int User::GetTheme()
{
	return theme;
}

void User::SetName(std::string name)
{
	std::string characters_allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. ";
	if (name.find_first_not_of(characters_allowed) != std::string::npos)
		throw Exception("Not allowed character in name");
	this->name = name;
}

void User::SetPassword(std::string new_password)
{
	std::string characters_allowed = alphabet;
	if (new_password.find_first_not_of(characters_allowed) != std::string::npos)
		throw Exception("Not allowed character in password");
	password = new_password;
	SaveUserInfo();
}

void User::SetAuthority(int authority)
{
	this->authority = authority;
}

void User::SetParticipation(std::string participation)
{
	if (participation == "None" || participation == "Low" || participation == "Medium" || participation == "High")
		this->participation_in_soc_activities = participation;
	else
		throw Exception("Wrong value");
}

void User::SetNumber(std::string number)
{
	if (number.find_first_not_of("0123456789+") != std::string::npos)
		throw Exception("Wrong value");
	this->number = number;
}

void User::SetGPA(float gpa)
{
	if (gpa >= 0 && gpa <= MAX_GPA)
		this->gpa = gpa;
	else
		throw Exception("Wrong value");
}

void User::SetIncomePerFamMember(float income)
{
	if (income >= 0)
		income_per_fam_member = income;
	else
		throw Exception("Wrong value");
}

void User::SetTheme(int theme)
{
	if (theme < 0)
		throw new std::out_of_range("Theme can't be negative");
	this->theme = theme;
}

void User::SetMaxGPA(int max_gpa)
{
	if (max_gpa >= 4)
		MAX_GPA = max_gpa;
	else
		throw Exception("Wrong value");
}
