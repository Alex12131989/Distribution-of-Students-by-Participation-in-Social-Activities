#include "AuthenticationWindow.h"
#include "User.h"
#include "App.h"

//move to AuthenticationWindow.cpp
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	User::CreateAdminZero();
	//move to AuthenticationWindow.cpp
	wxInitAllImageHandlers();
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

void App::GetAllChildren(wxWindow* parent, wxVector<wxWindow*>& all_children)
{
	if (!parent)
		return;

	wxWindowList direct_children = parent->GetChildren();

	for (auto direct_child : direct_children)
	{
		all_children.push_back(direct_child);
		GetAllChildren(direct_child, all_children);
	}
}
