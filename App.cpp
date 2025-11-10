#include "App.h"
#include "MainFrame.h"
#include "User.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	User::CreateAdminZero();
	MainFrame* mainFrame = new MainFrame("Distribution of Students by Participation in Social Activities");
	mainFrame->SetClientSize(MainWindowWidth, MainWindowHeight);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}