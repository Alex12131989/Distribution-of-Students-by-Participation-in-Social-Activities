#include "User.h"

using namespace std::experimental::filesystem;

void User::SaveTheme(int theme)
{
	path working_path = current_path();
	working_path /= std::format("BBS/{}", name);
	create_directories(working_path);
	working_path /= "theme.bin";

	std::ofstream working_file(working_path);
	//CONTINUE FROM HERE
}

void User::SaveUserInfo(std::string user_info)
{

}

int User::GetTheme()
{
	int theme = 0;
	return theme;
}

std::vector<std::string> User::GetUserInfo()
{
	//vector<name, password>
	std::vector<std::string> user_info;

	return user_info;
}