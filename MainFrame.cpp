#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	InitializeObjects();
	//GetAllChildren(this, all_objects);
	PaintObjects();
	PlaceObjects();
	BindObjects();
}

void MainFrame::InitializeObjects()
{
	panel = new wxPanel(this);
	get_password_panel = new wxPanel(panel, wxID_ANY);
	buttons_panel = new wxPanel(get_password_panel, wxID_ANY);

	welcome_label = new wxStaticText(get_password_panel, wxID_ANY, "Welcome back", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	instruction_name_label = new wxStaticText(get_password_panel, wxID_ANY, "Full name");
	instruction_password_label = new wxStaticText(get_password_panel, wxID_ANY, "Symbols allowed: a-Z, 0-9, !@#$%^&*_");

	enter_name_field = new wxTextCtrl(get_password_panel, wxID_ANY, "Enter your full name here");
	enter_password_field = new wxTextCtrl(get_password_panel, wxID_ANY, "Enter password here");

	sign_button = new wxButton(buttons_panel, wxID_ANY, "Sign up");
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
	panel_sizer->Add(buttons_panel, wxSizerFlags(1).Expand());
	get_password_panel->SetSizer(panel_sizer);

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxHORIZONTAL);
	outer_sizer->AddStretchSpacer();
	outer_sizer->Add(get_password_panel, wxSizerFlags(0).Center());
	outer_sizer->AddStretchSpacer();
	panel->SetSizer(outer_sizer);
	outer_sizer->SetSizeHints(this);
}

void MainFrame::BindObjects()
{

}

//void MainFrame::GetAllChildren(wxWindow* parent, wxWindowList& all_children)
//{
//	if (!parent)
//		return;
//
//	wxWindowList direct_children = parent->GetChildren();
//
//	for (auto direct_child : direct_children)
//	{
//		all_children.Append(direct_child);
//		GetAllChildren(direct_child, all_children);
//	}
//
//}

void MainFrame::PaintObjects()
{
	panel->SetBackgroundColour(RGB(200, 100, 100));
	get_password_panel->SetBackgroundColour(RGB(0, 0, 0));
	buttons_panel->SetBackgroundColour(RGB(200, 200, 121));
	welcome_label->SetBackgroundColour(RGB(100, 100, 100));
	instruction_name_label->SetBackgroundColour(RGB(0, 0, 100));
	instruction_password_label->SetBackgroundColour(RGB(100, 0, 0));
}
