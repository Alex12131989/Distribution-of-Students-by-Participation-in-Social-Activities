#include "User.h"
std::vector<User::user_info> User::users;
using namespace std::experimental::filesystem;

void User::SaveUserInfo()
{
	std::string incrypted_password = ApplyCipher(password, name, 0);
	path working_path = current_path();
	working_path /= "Users";
	create_directory(working_path);
	working_path /= "user_info.bin";
	std::ofstream rewrite_file(working_path, std::ios::binary);
	for (user_info user : users)
	{
		if (user.name == name)
		{
			rewrite_file.write(reinterpret_cast<const char*>(&authority), sizeof(authority));
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
			rewrite_file.write(name.data(), name.length());
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
			rewrite_file.write(incrypted_password.data(), incrypted_password.length());
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
			rewrite_file.write(reinterpret_cast<const char*>(&theme), sizeof(theme));
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_LINE_SYMBOL), sizeof(NEXT_LINE_SYMBOL));
		}
		else
		{
			rewrite_file.write(reinterpret_cast<const char*>(&user.authority), sizeof(user.authority));
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
			rewrite_file.write(user.name.data(), user.name.length());
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
			rewrite_file.write(user.incrypted_password.data(), user.incrypted_password.length());
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_FIELD_SYMBOL), sizeof(NEXT_FIELD_SYMBOL));
			rewrite_file.write(reinterpret_cast<const char*>(&user.theme), sizeof(user.theme));
			rewrite_file.write(reinterpret_cast<const char*>(&NEXT_LINE_SYMBOL), sizeof(NEXT_LINE_SYMBOL));
		}
	}
}

void User::LoadUserInfo()
{
	for (size_t i = 0; i < users.size(); ++i)
		if (users[i].name == name)
		{
			authority = users[i].authority;
			password = ApplyCipher(users[i].incrypted_password, users[i].name, 1);
			theme = users[i].theme;
			i = users.size();
		}
}

void User::GetAllUserInfos()
{
	path working_path = current_path();
	working_path /= "Users";
	create_directory(working_path);
	working_path /= "user_info.bin";
	std::ifstream working_file(working_path, std::ios::binary);

	std::vector<char> buffer;
	std::string empty_string = "";
	std::vector<std::string> lines = { empty_string };
	while (working_file.read(buffer.data(), CHUNK))
	{
		lines[lines.size() - 1].append(buffer.data(), CHUNK);
		size_t pos = lines[lines.size() - 1].find(NEXT_LINE_SYMBOL);
		if (pos != std::string::npos)
		{
			lines.push_back(empty_string);
			lines[lines.size() - 1].append(lines[lines.size() - 1].substr(pos + 1));
			lines[lines.size() - 2].resize(lines[lines.size() - 2].length() - CHUNK + pos);
		}

	}

	user_info empty_struct;
	for (std::string line : lines)
	{
		users.push_back(empty_struct);
		std::stringstream string_stream(empty_string);
		std::getline(string_stream, empty_string, NEXT_FIELD_SYMBOL);
		users[users.size() - 1].authority = std::stoi(empty_string);
		std::getline(string_stream, empty_string, NEXT_FIELD_SYMBOL);
		users[users.size() - 1].name = empty_string;
		std::getline(string_stream, empty_string, NEXT_FIELD_SYMBOL);
		users[users.size() - 1].incrypted_password = empty_string;
		std::getline(string_stream, empty_string);
		users[users.size() - 1].theme = std::stoi(empty_string);
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
		Encode(char_password, key);
		break;
	case 1:
		Decode(char_password, key);
		break;
	}
	password = char_password;
	return password;
}