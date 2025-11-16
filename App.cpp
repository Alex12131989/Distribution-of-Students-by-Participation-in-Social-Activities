#include "App.h"
#include "AuthenticationWindow.h"
#include "User.h"

//move to AuthenticationWindow.cpp
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	User::CreateAdminZero();
	//move to AuthenticationWindow.cpp
	MainWindow* main_window = new MainWindow("Distribution of Students by Participation in Social Activities", 0/*change to something like user->authority*/);
	main_window->SetMinSize(wxSize(MainWindowWidth, MainWindowHeight));
	main_window->SetClientSize(MainWindowWidth, MainWindowHeight);
	main_window->Center();
	main_window->Show();

	/*
	AuthenticationWindow* authentication_window = new AuthenticationWindow("Distribution of Students by Participation in Social Activities");
	authentication_window->SetClientSize(MainWindowWidth, MainWindowHeight);
	authentication_window->Center();
	authentication_window->Show();
	*/
	return true;
}