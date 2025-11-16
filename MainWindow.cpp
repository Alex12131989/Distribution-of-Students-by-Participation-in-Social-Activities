#include "MainWindow.h"
#include "User.h"
#include "App.h"

MainWindow::MainWindow(const wxString& title, User* user) : wxFrame(nullptr, wxID_ANY, title)
{
	//for time being while I did NOT log into any account
	user = new User("Alex", "pT@@3X*", 0);
	user->FindUser();

	for (User::user_info user : User::users)
		all_users.push_back(user);
	InitializeObjects(user->GetAuthority());
	BindObjects(user->GetAuthority());
	PlaceObjects(user->GetAuthority());
	PaintObjects(user);
}

void MainWindow::InitializeObjects(const int access_mode)
{
	panel = new wxPanel(this);

	taskbar_panel = new wxPanel(panel, wxID_ANY);

	current_card_panel = new wxPanel(panel, wxID_ANY);

	taskbar_centeralize_panel = new wxPanel(taskbar_panel, wxID_ANY);

	authority_display_label = new wxStaticText(taskbar_centeralize_panel, wxID_ANY, "");

	profile_scroll_panel = new wxScrolledWindow(current_card_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	//profile_scroll_panel->SetMinSize(wxSize(MainWindowWidth*0.4, -1));

	other_profiles_scroll_panel = new wxScrolledWindow(current_card_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	//other_profiles_scroll_panel->SetMinSize(wxSize(MainWindowWidth * 0.55, -1));

	switch (access_mode)
	{
	case 0:
		authority_display_label->SetLabelText("User");
		break;
	case 1:
		authority_display_label->SetLabelText("Administrator");
		break;
	}
}

void MainWindow::PlaceObjects(const int access_mode)
{
	wxBoxSizer* taskbar_centeralize_sizer = new wxBoxSizer(wxHORIZONTAL);
	switch (access_mode)
	{
	case 0:
		break;
	case 1:
		break;
	}
	taskbar_centeralize_sizer->AddStretchSpacer();
	taskbar_centeralize_sizer->Add(authority_display_label, wxSizerFlags());
	taskbar_centeralize_sizer->AddSpacer(10);
	taskbar_centeralize_panel->SetSizer(taskbar_centeralize_sizer);
	taskbar_centeralize_panel->Layout();

	wxBoxSizer* taskbar_sizer = new wxBoxSizer(wxVERTICAL);
	taskbar_sizer->AddStretchSpacer();
	taskbar_sizer->Add(taskbar_centeralize_panel, wxSizerFlags().Expand());
	taskbar_sizer->AddStretchSpacer();
	taskbar_panel->SetSizer(taskbar_sizer);
	taskbar_panel->Layout();

	wxBoxSizer* current_card_sizer = new wxBoxSizer(wxHORIZONTAL);
	switch (access_mode)
	{
	case 0:
		break;
	case 1:
		break;
	}
	current_card_sizer->Add(profile_scroll_panel, wxSizerFlags(33).Expand());
	current_card_sizer->AddStretchSpacer(1);
	current_card_sizer->Add(other_profiles_scroll_panel, wxSizerFlags(66).Expand());
	current_card_panel->SetSizer(current_card_sizer);
	current_card_panel->Layout();

	wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
	panel_sizer->Add(taskbar_panel, wxSizerFlags(1).Expand());
	panel_sizer->AddSpacer(1);
	panel_sizer->Add(current_card_panel, wxSizerFlags(19).Expand());
	panel->SetSizer(panel_sizer);
	panel->Layout();

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxVERTICAL);
	outer_sizer->Add(panel, wxSizerFlags(1).Expand());
	this->SetSizer(outer_sizer);
	outer_sizer->SetSizeHints(this);
	this->Layout();
}

void MainWindow::BindObjects(const int access_mode)
{
	switch (access_mode)
	{
	case 0:
		break;
	case 1:
		break;
	}
}

void MainWindow::PaintObjects(User* user)
{
	//user->SetTheme(1);
	panel->SetBackgroundColour(RGB(0, 0, 0));
	taskbar_panel->SetBackgroundColour(RGB(100, 100, 100));
	current_card_panel->SetBackgroundColour(RGB(200, 200, 200));
	authority_display_label->SetBackgroundColour(RGB(255, 0, 0));
	profile_scroll_panel->SetBackgroundColour(RGB(0, 0, 255));
	other_profiles_scroll_panel->SetBackgroundColour(RGB(0, 255, 0));


	this->Refresh();
	this->Update();
}

void MainWindow::GetProfileImage()
{
	//get the photo, otherwise get stock image
}