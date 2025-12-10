#include "AuthenticationWindow.h"
#include "User.h"
#include "App.h"

//move to AuthenticationWindow.cpp
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	User::CreateAdminZero();
	AuthenticationWindow* authentication_window = new AuthenticationWindow("Distribution of Students by Participation in Social Activities");
	authentication_window->SetClientSize(MainWindowWidth, MainWindowHeight);
	authentication_window->Center();
	authentication_window->Show();
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
