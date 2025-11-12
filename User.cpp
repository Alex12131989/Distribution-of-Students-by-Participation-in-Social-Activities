#include "Exception.h"
#include "cipher.h"
#include "User.h"

std::vector<User::user_info> User::users;
using namespace std::experimental::filesystem;

void User::CreateAdminZero()
{
	//this code would be given to the customer, and potentially be changed for every new copy
	auto admin_zero = new User("Root Account", "kldjflksjljkjdshnklhcdykjdhb", 0);
	admin_zero->SaveUserInfo();
}

void User::WriteSingleUserToFile(std::ofstream& file, user_info data)
{
	auto temp_test = reinterpret_cast<const char*>(&data.authority);
	auto temp_test_size = sizeof(data.authority);
	file.write(reinterpret_cast<const char*>(&data.authority), sizeof(data.authority));
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
	file.write(data.name.data(), data.name.length());
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
	file.write(data.incrypted_password.data(), data.incrypted_password.length());
	file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
	file.write(reinterpret_cast<const char*>(&data.theme), sizeof(data.theme));
	file.write(reinterpret_cast<const char*>(&NEXT_LINE_SYMBOL), sizeof(NEXT_LINE_SYMBOL));
}

bool User::ReadSingleUserToFile(std::ifstream& file, user_info& user)
{
	char buffer = '\0';

	file.read(reinterpret_cast<char*>(&user.authority), sizeof(user.authority));
	if (file.eof())
		return false;
	for(int i = 0; i < 2; ++i)
		file.read(&buffer, 1);					//since next char is ':'

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
	std::ofstream rewrite_file(working_path, std::ios::binary || std::ios::out);
	for (user_info user : users)
	{
		if (user.name == name)
		{
			auto temp_struct = user_info{ name, incrypted_password, authority, theme };
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
		auto temp_struct = user_info{ name, incrypted_password, authority, theme };
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
			theme = users[i].theme;
			i = users.size();
		}
	if (!found)
		throw Exception("Could not find the user");
	else if (!verified)
		throw Exception("Wrong password");
}

void User::AddNewUser(User new_user)
{
	for (user_info user : users)
		if (user.name == new_user.name)
			throw Exception("User already exists");
	new_user.SaveUserInfo();
}

void User::GetAllUserInfos()
{
	path working_path = current_path();
	working_path /= "Users";
	create_directory(working_path);
	working_path /= "user_info.bin";
	std::ifstream working_file(working_path, std::ios::binary);
	user_info user;
	users.clear();
	while (ReadSingleUserToFile(working_file, user))
	{
		users.push_back(user);
		user = {};
	}
	working_file.close();
}

std::string User::ApplyCipher(std::string password, std::string name, int option)
{
	char* char_password = new char[password.length()], * char_name = new char[name.length()], * key;
	strcpy(char_password, password.c_str());
	strcpy(char_name, name.c_str());
	key = GetKey(char_name);
	switch (option)
	{
	case 0:
		Encode(key, char_password); //check this crap
		break;
	case 1:
		Decode(key, char_password);
		break;
	}
	password = char_password;
	return password;
}
