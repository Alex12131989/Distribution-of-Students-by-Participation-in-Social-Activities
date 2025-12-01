#include "AuthenticationWindow.h"
#include "Exception.h"
#include "cipher.h"
#include "User.h"
#include "App.h"

AuthenticationWindow::AuthenticationWindow(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	InitializeObjects();
	PlaceObjects();
	BindObjects();
	App::GetAllChildren(this, all_objects);
	PaintObjects(theme);
}

void AuthenticationWindow::InitializeObjects()
{
	panel = new wxPanel(this);
	get_password_panel = new wxPanel(panel, wxID_ANY);
	buttons_panel = new wxPanel(get_password_panel, wxID_ANY);

	welcome_label = new wxStaticText(get_password_panel, wxID_ANY, "Welcome! Please enter your credentials", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
	instruction_name_label = new wxStaticText(get_password_panel, wxID_ANY, "Full name");
	wxString instruction_password_string = "Symbols allowed: a-Z, 0-9, ";
	for (size_t i = 0; i < alphabet_size; ++i)
	{
		int ascii_char_code = static_cast<int>(alphabet[i]);
		if (!(	ascii_char_code >= 48 && ascii_char_code <= 57 || 
				ascii_char_code >= 65 && ascii_char_code <= 90 ||
				ascii_char_code >= 97 && ascii_char_code <= 122))
			instruction_password_string += alphabet[i];
	}
	instruction_password_label = new wxStaticText(get_password_panel, wxID_ANY, instruction_password_string);

	enter_name_field = new wxTextCtrl(get_password_panel, wxID_ANY, "Enter your full name here");
	enter_password_field = new wxTextCtrl(get_password_panel, wxID_ANY, "Enter password here");

	sign_button = new wxButton(buttons_panel, wxID_ANY, "Sign Up");
	change_theme_button = new wxButton(buttons_panel, wxID_ANY, "Change theme");
	confirm_button = new wxButton(buttons_panel, wxID_ANY, "Confirm");
}

void AuthenticationWindow::PlaceObjects()
{
	wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	buttons_sizer->Add(sign_button, wxSizerFlags(1));
	buttons_sizer->AddStretchSpacer();
	buttons_sizer->Add(change_theme_button, wxSizerFlags(1));
	buttons_sizer->AddStretchSpacer();
	buttons_sizer->Add(confirm_button, wxSizerFlags(1));
	buttons_panel->SetSizer(buttons_sizer);
	buttons_panel->Layout();

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
	get_password_panel->Layout();

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxHORIZONTAL);
	outer_sizer->AddStretchSpacer();
	outer_sizer->Add(get_password_panel, wxSizerFlags(0).Center());
	outer_sizer->AddStretchSpacer();
	panel->SetSizer(outer_sizer);
	outer_sizer->SetSizeHints(this);
	this->Layout();
}

void AuthenticationWindow::PaintObjects(int theme)
{
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
	this->Update();
}

void AuthenticationWindow::BindObjects()
{
	sign_button->Bind(wxEVT_BUTTON, &AuthenticationWindow::OnSignButtonClicked, this);
	change_theme_button->Bind(wxEVT_BUTTON, &AuthenticationWindow::OnChangeThemeButtonClicked, this);
	confirm_button->Bind(wxEVT_BUTTON, &AuthenticationWindow::OnConfirmButtonClicked, this);
}

void AuthenticationWindow::OnSignButtonClicked(wxCommandEvent& event)
{
	ChangeSigningMode();
}

void AuthenticationWindow::OnChangeThemeButtonClicked(wxCommandEvent& event)
{
	if (++theme >= MAX_THEMES)
		theme = theme % MAX_THEMES;
	PaintObjects(theme);
}

void AuthenticationWindow::OnConfirmButtonClicked(wxCommandEvent& event)
{
	try
	{
		CredentialValidation();
		wxString sign_option = sign_button->GetLabelText();
		User* user = new User(enter_name_field->GetValue().ToStdString(), enter_password_field->GetValue().ToStdString(), theme);
		if (sign_option == "Sign In")
		{
			user->FindUser();
			wxMessageBox(wxT("You signed in"), wxT("Congrats"), wxICON_INFORMATION);
			
		}
		else if (sign_option == "Sign Up")
		{
			user->AddNewUser();
			wxMessageBox(wxT("New account was created"), wxT("Congrats"), wxICON_INFORMATION);
		}
	}
	catch (Exception& exception)
	{
		switch (exception.code)
		{
		case 0:
			wxMessageBox(wxT("Devs didn't work this out yet"), wxT("Something went wrong"), wxICON_ERROR);
			break;
		case 1:
			wxMessageBox(wxT("Your password contained some illegal characters"), wxT("Illegal characters detected"), wxICON_ERROR);
			break;
		case 2:
			wxMessageBox(wxT("Your name contained some illegal characters"), wxT("Illegal characters detected"), wxICON_ERROR);
			break;
		case 3:
			wxMessageBox(wxT("You didn't enter your name"), wxT("Illegal action detected"), wxICON_ERROR);
			break;
		case 4:
			wxMessageBox(wxT("You didn't enter the password"), wxT("Illegal action detected"), wxICON_ERROR);
			break;
		case 5:
			wxMessageBox(wxT("Check your credentials, account weren't found"), wxT("Account doesn't exist"), wxICON_ERROR);
			break;
		case 6:
			wxMessageBox(wxT("Try signing in"), wxT("Account already exists"), wxICON_WARNING);
			break;
		case 7:
			wxMessageBox(wxT("Check your credentials"), wxT("Wrong password"), wxICON_ERROR);
			break;
		default:
			wxMessageBox(wxT("Woah, you just did something I haven't thought you would"),
						wxT("Congrants on thinking outside the box"), wxOK || wxICON_WARNING);
		}
	}
	catch (...)
	{
		wxMessageBox(wxT(":<"), wxT("Something went wrong"), wxICON_ERROR);
	}
}

void AuthenticationWindow::ChangeSigningMode()
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

void AuthenticationWindow::CredentialValidation()
{
	wxString user_name = enter_name_field->GetValue();
	if (user_name == "")
		throw Exception("Name was not entered");
	for (char name_char : user_name)
		if(!(	static_cast<int>(name_char) >= 65 && static_cast<int>(name_char) <= 90 ||
				static_cast<int>(name_char) >= 97 && static_cast<int>(name_char) <= 122 || 
				static_cast<int>(name_char) == 46 || static_cast<int>(name_char) == 32))
			throw Exception("Not allowed character in name");

	wxString user_password = enter_password_field->GetValue();
	if (user_name == "")
		throw Exception("Password was not entered");
	for (char name_char : user_password)
	{
		bool found_code = false;
		for (size_t i = 0; i < alphabet_size; ++i)
			if (static_cast<int>(name_char) == static_cast<int>(alphabet[i]))
			{
				found_code = true;
				i = alphabet_size;
			}
		if (!found_code)
			throw Exception("Not allowed character in password");
	}
}
