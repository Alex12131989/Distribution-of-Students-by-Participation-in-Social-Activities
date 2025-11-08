#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	InitializeObjects();
	GetAllChildren(this, all_objects);
	PaintObjects(theme);
	PlaceObjects();
	BindObjects();
}

void MainFrame::InitializeObjects()
{
	panel = new wxPanel(this);
	get_password_panel = new wxPanel(panel, wxID_ANY);
	buttons_panel = new wxPanel(get_password_panel, wxID_ANY);

	welcome_label = new wxStaticText(get_password_panel, wxID_ANY, "Welcome! Please enter your credentials", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	instruction_name_label = new wxStaticText(get_password_panel, wxID_ANY, "Full name");
	instruction_password_label = new wxStaticText(get_password_panel, wxID_ANY, "Symbols allowed: a-Z, 0-9, !@#$%^&*_");

	enter_name_field = new wxTextCtrl(get_password_panel, wxID_ANY, "Enter your full name here");
	enter_password_field = new wxTextCtrl(get_password_panel, wxID_ANY, "Enter password here");

	sign_button = new wxButton(buttons_panel, wxID_ANY, "Sign Up");
	change_theme_button = new wxButton(buttons_panel, wxID_ANY, "Change theme");
	confirm_button = new wxButton(buttons_panel, wxID_ANY, "Confirm");
}

void MainFrame::PlaceObjects()
{
	wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	buttons_sizer->Add(sign_button, wxSizerFlags(1));
	buttons_sizer->AddStretchSpacer();
	buttons_sizer->Add(change_theme_button, wxSizerFlags(1));
	buttons_sizer->AddStretchSpacer();
	buttons_sizer->Add(confirm_button, wxSizerFlags(1));
	buttons_panel->SetSizer(buttons_sizer);

	wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);
	panel_sizer->Add(welcome_label, wxSizerFlags(1).Expand());
	panel_sizer->AddSpacer(10);
	panel_sizer->Add(instruction_name_label, wxSizerFlags(1).Expand());
	panel_sizer->Add(enter_name_field, wxSizerFlags(1).Expand());
	panel_sizer->AddSpacer(10);
	panel_sizer->Add(instruction_password_label, wxSizerFlags(1).Expand());
	panel_sizer->Add(enter_password_field, wxSizerFlags(1).Expand());
	panel_sizer->AddSpacer(10);
	panel_sizer->Add(buttons_panel, wxSizerFlags(1).Expand());
	get_password_panel->SetSizer(panel_sizer);

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxHORIZONTAL);
	outer_sizer->AddStretchSpacer();
	outer_sizer->Add(get_password_panel, wxSizerFlags(0).Center());
	outer_sizer->AddStretchSpacer();
	panel->SetSizer(outer_sizer);
	outer_sizer->SetSizeHints(this);
}

void MainFrame::PaintObjects(int theme)
{
	panel->SetBackgroundColour(RGB(200, 100, 100));
	get_password_panel->SetBackgroundColour(RGB(0, 0, 0));
	buttons_panel->SetBackgroundColour(RGB(200, 200, 121));
	welcome_label->SetBackgroundColour(RGB(100, 100, 100));
	instruction_name_label->SetBackgroundColour(RGB(0, 0, 100));
	instruction_password_label->SetBackgroundColour(RGB(100, 0, 0));

	for (auto object : all_objects)
	{
		switch (theme)
		{
		case 0:
			object->SetBackgroundColour(RGB(0, 0, 0));
			object->SetForegroundColour(RGB(255, 255, 255));
			break;
		case 1:
			object->SetBackgroundColour(RGB(255, 255, 255));
			object->SetForegroundColour(RGB(0, 0, 0));
			break;
		}
	}
	this->Refresh();
}

void MainFrame::BindObjects()
{
	//sign_button->Bind(wxEVT_BUTTON, [=](wxCommandEvent) {OnSignButtonClicked(wxCommandEvent);});
	sign_button->Bind(wxEVT_BUTTON, &MainFrame::OnSignButtonClicked, this);
	change_theme_button->Bind(wxEVT_BUTTON, &MainFrame::OnChangeThemeButtonClicked, this);
	confirm_button->Bind(wxEVT_BUTTON, &MainFrame::OnConfirmButtonClicked, this);
}

void MainFrame::OnSignButtonClicked(wxCommandEvent& event)
{
	ChangeSigningMode();
}

void MainFrame::OnChangeThemeButtonClicked(wxCommandEvent& event)
{
	if (++theme >= MAX_THEMES)
		theme = theme % MAX_THEMES;
	PaintObjects(theme);
}

void MainFrame::OnConfirmButtonClicked(wxCommandEvent& event)
{
	CheckCredentials();
}

void MainFrame::ChangeSigningMode()
{
	wxString sign_button_text = sign_button->GetLabelText();
	if (sign_button_text == "Sign In")
	{
		sign_button->SetLabelText("Sign Up");
		welcome_label->SetLabelText("Welcome! Please enter your credentials");
	}
	else
	{
		sign_button->SetLabelText("Sign In");
		welcome_label->SetLabelText("Welcome back! Please enter your credentials");
	}
	get_password_panel->Layout();
}

void MainFrame::CheckCredentials()
{

}

void MainFrame::GetAllChildren(wxWindow* parent, wxWindowList& all_children)
{
	if (!parent)
		return;

	wxWindowList direct_children = parent->GetChildren();

	for (auto direct_child : direct_children)
	{
		all_children.Append(direct_child);
		GetAllChildren(direct_child, all_children);
	}

}
