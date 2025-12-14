#include  "EditWindow.h"
#include "Exception.h"
#include "App.h"

EditWindow::EditWindow(User::user_info user, bool unlimited_power, bool self_edition) : wxFrame(nullptr, wxID_ANY,  wxString::FromUTF8("Editing " + user.name))
{
	currently_editing_user = new User(user.name, user.incrypted_password, user.theme);
	pre_changed_name = user.name;
	try
	{
		currently_editing_user->FindUser();
	}
	catch (Exception exception) {	/*could be already registered, could be not, whatever*/ }
	InitializeObjects(user, unlimited_power, self_edition);
	PlaceObjects(unlimited_power, self_edition);
	PaintObjects(user.theme);
	BindObjects();
}

void EditWindow::InitializeObjects(User::user_info user, bool unlimited_power, bool self_edition)
{
	panel = new wxPanel(this, wxID_ANY);
	

	if (self_edition)
	{
		user.incrypted_password = "I don't friggin' get it, why do I need this to prevent it from crashing??!?!?!!!?!?!?!?";
		user.incrypted_password = User::ApplyCipher(currently_editing_user->GetPassword(), currently_editing_user->GetName(), 1);
		password_guide = new wxStaticText(panel, wxID_ANY, "Password");
		password_field = new wxTextCtrl(panel, wxID_ANY, user.incrypted_password);
	}
	if (unlimited_power)
	{
		name_guide = new wxStaticText(panel, wxID_ANY, "Name");
		name_field = new wxTextCtrl(panel, wxID_ANY, user.name);

		activity_guide = new wxStaticText(panel, wxID_ANY, "Participation in social activities");
		activity_field = new wxTextCtrl(panel, wxID_ANY, user.participation_in_soc_activities);

		authority_guide = new wxStaticText(panel, wxID_ANY, "Authority");
		wxString authority_string = "User";
		if (user.authority)
			authority_string = "Administrator";
		authority_field = new wxTextCtrl(panel, wxID_ANY, authority_string);

		number_guide = new wxStaticText(panel, wxID_ANY, "Phone number");
		number_field = new wxTextCtrl(panel, wxID_ANY, user.number);

		gpa_guide = new wxStaticText(panel, wxID_ANY, "Grade point average");
		gpa_field = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%.2f"), user.gpa));

		income_guide = new wxStaticText(panel, wxID_ANY, "Income per family member");
		income_field = new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%.2f"), user.income_per_fam_member));
	}

	confirm_button = new wxButton(panel, wxID_ANY, "Confirm");
	save_button = new wxButton(panel, wxID_ANY, "Save");
}

void EditWindow::PlaceObjects(bool unlimited_power, bool self_edition)
{
	wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	buttons_sizer->AddStretchSpacer(1);
	buttons_sizer->Add(save_button, wxSizerFlags().Expand());
	buttons_sizer->AddSpacer(10);
	buttons_sizer->Add(confirm_button, wxSizerFlags().Expand());

	wxBoxSizer* controls_sizer = new wxBoxSizer(wxVERTICAL);
	controls_sizer->AddStretchSpacer(1);
	if (unlimited_power)
	{
		controls_sizer->Add(name_guide, wxSizerFlags().Expand());
		controls_sizer->Add(name_field, wxSizerFlags().Expand());
		controls_sizer->AddSpacer(10);
	}
	if (self_edition)
	{
		controls_sizer->Add(password_guide, wxSizerFlags().Expand());
		controls_sizer->Add(password_field, wxSizerFlags().Expand());
	}
	if (unlimited_power)
	{
		controls_sizer->AddSpacer(10);
		controls_sizer->Add(activity_guide, wxSizerFlags().Expand());
		controls_sizer->Add(activity_field, wxSizerFlags().Expand());
		controls_sizer->AddSpacer(10);

		controls_sizer->Add(authority_guide, wxSizerFlags().Expand());
		controls_sizer->Add(authority_field, wxSizerFlags().Expand());
		controls_sizer->AddSpacer(10);

		controls_sizer->Add(number_guide, wxSizerFlags().Expand());
		controls_sizer->Add(number_field, wxSizerFlags().Expand());
		controls_sizer->AddSpacer(10);

		controls_sizer->Add(gpa_guide, wxSizerFlags().Expand());
		controls_sizer->Add(gpa_field, wxSizerFlags().Expand());
		controls_sizer->AddSpacer(10);

		controls_sizer->Add(income_guide, wxSizerFlags().Expand());
		controls_sizer->Add(income_field, wxSizerFlags().Expand());
	}
	controls_sizer->AddSpacer(20);
	controls_sizer->Add(buttons_sizer, wxSizerFlags().Expand());
	controls_sizer->AddStretchSpacer(1);

	wxBoxSizer* panel_sizer = new wxBoxSizer(wxHORIZONTAL);
	panel_sizer->AddStretchSpacer(25);
	panel_sizer->Add(controls_sizer, wxSizerFlags(50).Expand());
	panel_sizer->AddStretchSpacer(25);
	panel->SetSizer(panel_sizer);
	panel->Layout();

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxVERTICAL);
	outer_sizer->Add(panel, wxSizerFlags().Expand());
	this->SetSizer(outer_sizer);
	outer_sizer->SetSizeHints(this);
	this->Layout();
}

void EditWindow::PaintObjects(int theme)
{
	wxVector<wxWindow*> all_objects;
	App::GetAllChildren(this, all_objects);
	switch (theme)
	{
	case 0:
		this->SetBackgroundColour(RGB(0, 0, 0));
		for (auto object : all_objects)
		{
			object->SetBackgroundColour(RGB(0, 0, 0));
			object->SetForegroundColour(RGB(255, 255, 255));
		}
		break;
	case 1:
		this->SetBackgroundColour(RGB(255, 255, 255));
		for (auto object : all_objects)
		{
			object->SetBackgroundColour(RGB(255, 255, 255));
			object->SetForegroundColour(RGB(0, 0, 0));
		}
		break;
	}
	this->Refresh();
	this->Update();
}

void EditWindow::BindObjects()
{
	this->Bind(wxEVT_CLOSE_WINDOW, [=](wxCloseEvent& event) {EditWindow::OnClose(event);});
	confirm_button->Bind(wxEVT_BUTTON, &EditWindow::OnConfirmButtonsClicked, this);
	save_button->Bind(wxEVT_BUTTON, &EditWindow::OnSaveButtonClicked, this);
}

void EditWindow::OnSaveButtonClicked(wxCommandEvent& event)
{
	Save();
}

void EditWindow::OnConfirmButtonsClicked(wxCommandEvent& event)
{
	if (Save())
		this->Destroy();
}

void EditWindow::OnClose(wxCloseEvent& event)
{
	if (Save())
		this->Destroy();
}

bool EditWindow::Save()
{
	try
	{
		if (password_field != nullptr)
			currently_editing_user->SetPassword(password_field->GetValue().ToStdString());

		if (name_field != nullptr)
			currently_editing_user->SetName(name_field->GetValue().ToStdString());

		if (number_field != nullptr)
			currently_editing_user->SetNumber(number_field->GetValue().ToStdString());

		if (activity_field != nullptr)
			currently_editing_user->SetParticipation(activity_field->GetValue().ToStdString());

		if (gpa_field != nullptr)
		{
			double dgpa;
			gpa_field->GetValue().ToDouble(&dgpa);
			try
			{
				currently_editing_user->SetGPA(static_cast<float>(dgpa));
			}
			catch (Exception exception)
			{
				if (exception.code == 8) wxLogError(wxT("The GPA is out of bounds"));
			}
		}

		if (income_field != nullptr)
		{
			double dincome;
			income_field->GetValue().ToDouble(&dincome);
			try
			{
				currently_editing_user->SetIncomePerFamMember(static_cast<float>(dincome));
			}
			catch (Exception exception)
			{
				if (exception.code == 8) wxLogError(wxT("Income can't be negative"));
			}
		}

		if (authority_field != nullptr)
			if (authority_field->GetValue().Lower() == "administrator")
				currently_editing_user->SetAuthority(1);
			else
				currently_editing_user->SetAuthority(0);
	}
	catch (Exception& exception)
	{
		switch (exception.code)
		{
		case 1:
			wxLogError(wxT("Illegal characters in name"));
			break;
		case 2:
			wxLogError(wxT("Illegal characters in password"));
			break;
		case 8:
			wxLogError(wxT("Some of the field(s) contained illegal values"));
			break;
		}
		return false;
	}

	try
	{
		if (name_field == nullptr)
			currently_editing_user->SaveUserInfo();
		else
		{
			if (pre_changed_name == name_field->GetValue())
			{
				currently_editing_user->SaveUserInfo();
				pre_changed_name = currently_editing_user->GetName();
			}
			else
			{
				currently_editing_user->AddNewUser();
				User::user_info old_user = {
				.name = pre_changed_name.ToStdString(),
				.incrypted_password = currently_editing_user->GetPassword(),
				.participation_in_soc_activities = currently_editing_user->GetParticipation(),
				.authority = currently_editing_user->GetAuthority(),
				.number = currently_editing_user->GetNumber(),
				.gpa = currently_editing_user->GetGPA(),
				.income_per_fam_member = currently_editing_user->GetIncomePerFamMember(),
				.theme = currently_editing_user->GetTheme()
				};
				User::DeleteUser(old_user);
			}
		}
	}
	catch (Exception& exception)
	{
		if (exception.code == 6)
			wxLogError("Can't save because the name already taken");
		return false;
	}
	return true;
}
