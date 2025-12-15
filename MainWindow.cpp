#include "MainWindow.h"
#include "EditWindow.h"
#include "Exception.h"
#include "detector.h"
#include "User.h"
#include "App.h"

MainWindow::MainWindow(const wxString& title, User* user) : wxFrame(nullptr, wxID_ANY, title)
{
	this->user = user;
	InitializeObjects();
	BindObjects();
	PlaceObjects();
	App::GetAllChildren(this, all_objects);
	PaintObjects(user->GetTheme());
}

void MainWindow::InitializeObjects()
{
	GetThemeIcons(user->GetTheme());
	panel = new wxPanel(this);

	taskbar_panel = new wxPanel(panel, wxID_ANY);

	current_card_panel = new wxPanel(panel, wxID_ANY);

	taskbar_centralize_panel = new wxPanel(taskbar_panel, wxID_ANY);

	themes_panel = new wxPanel(taskbar_centralize_panel, wxID_ANY);

	themes_dropdown_box = new wxChoice(themes_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, themes_options_list);
	themes_dropdown_box->SetSelection(user->GetTheme());
	themes_image = new wxStaticBitmap(themes_panel, wxID_ANY, change_theme_bitmap);

	search_panel = new wxPanel(taskbar_centralize_panel, wxID_ANY);
	search_field = new wxTextCtrl(search_panel, wxID_ANY, "");
	search_image = new wxStaticBitmap(search_panel, wxID_ANY, search_bitmap);

	sort_panel = new wxPanel(taskbar_centralize_panel, wxID_ANY);
	sort_dropdown_box = new wxChoice(sort_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, sort_options_list);
	sort_order_button = new wxBitmapButton(sort_panel, wxID_ANY, sort_bitmap);

	view_button = new wxBitmapButton(taskbar_centralize_panel, wxID_ANY, view_bitmap);

	authority_display_panel = new wxPanel(taskbar_centralize_panel, wxID_ANY);

	authority_display_label = new wxStaticText(authority_display_panel, wxID_ANY, "");

	profile_scroll_panel = new wxScrolledWindow(current_card_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	//profile_scroll_panel->SetMinSize(wxSize(MainWindowWidth*0.4, -1));

	current_user_info_panel = new wxPanel(profile_scroll_panel, wxID_ANY);
	edit_current_user_info_button = new wxBitmapButton(profile_scroll_panel, wxID_ANY, edit_bitmap);

	wxBitmap profile_bitmap(GetProfileImage(wxString(user->GetName()), {200, 200}));
	profile_picture = new wxStaticBitmap(profile_scroll_panel, wxID_ANY, profile_bitmap, wxDefaultPosition, wxDefaultSize);

	wxString user_information_string = "Name: " + user->GetName() + "\nParticipation in social events: " + user->GetParticipation() + 
		"\nNumber: " + user->GetNumber() + "\nGrade point average: " + wxString::Format(wxT("%.2f"), user->GetGPA()) + 
		"\nIncome per family member: " + wxString::Format(wxT("%.2f"), user->GetIncomePerFamMember());
	current_user_info_label = new wxStaticText(current_user_info_panel, wxID_ANY, user_information_string);

	other_profiles_scroll_panel = new wxScrolledWindow(current_card_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
	//other_profiles_scroll_panel->SetMinSize(wxSize(MainWindowWidth * 0.55, -1));

	switch (user->GetAuthority())
	{
	case 0:
		authority_display_label->SetLabelText("User");
		break;
	case 1:
		authority_display_label->SetLabelText("Administrator");
		add_button = new wxBitmapButton(taskbar_centralize_panel, wxID_ANY, add_bitmap);
		break;
	}
}

void MainWindow::PlaceObjects()
{
	wxBoxSizer* search_sizer = new wxBoxSizer(wxHORIZONTAL);
	search_sizer->Add(search_field, wxSizerFlags().Expand());
	search_sizer->Add(search_image, wxSizerFlags().Expand());
	search_panel->SetSizer(search_sizer);
	search_panel->Layout();

	wxBoxSizer* sort_sizer = new wxBoxSizer(wxHORIZONTAL);
	sort_sizer->Add(sort_dropdown_box, wxSizerFlags().Expand());
	sort_sizer->Add(sort_order_button, wxSizerFlags().Expand());
	sort_panel->SetSizer(sort_sizer);
	sort_panel->Layout();

	wxBoxSizer* theme_sizer = new wxBoxSizer(wxHORIZONTAL);
	theme_sizer->Add(themes_dropdown_box, wxSizerFlags().Expand());
	theme_sizer->Add(themes_image, wxSizerFlags().Expand());
	themes_panel->SetSizer(theme_sizer);
	themes_panel->Layout();

	wxBoxSizer* authority_display_sizer = new wxBoxSizer(wxVERTICAL);
	authority_display_sizer->AddStretchSpacer();
	authority_display_sizer->Add(authority_display_label);
	authority_display_sizer->AddStretchSpacer();
	authority_display_panel->SetSizer(authority_display_sizer);
	authority_display_panel->Layout();

	wxBoxSizer* taskbar_centralize_sizer = new wxBoxSizer(wxHORIZONTAL);
	if (user->GetAuthority() == 1)
	{
		taskbar_centralize_sizer->AddSpacer(22);
		taskbar_centralize_sizer->Add(add_button, wxSizerFlags().Expand());
		taskbar_centralize_sizer->AddSpacer(22);
	}
	taskbar_centralize_sizer->Add(view_button, wxSizerFlags().Expand());
	taskbar_centralize_sizer->AddStretchSpacer();
	taskbar_centralize_sizer->Add(sort_panel, wxSizerFlags().Expand());
	taskbar_centralize_sizer->AddSpacer(20);
	taskbar_centralize_sizer->Add(search_panel, wxSizerFlags().Expand());
	taskbar_centralize_sizer->AddSpacer(20);
	taskbar_centralize_sizer->Add(themes_panel, wxSizerFlags().Expand());
	taskbar_centralize_sizer->AddSpacer(20);
	taskbar_centralize_sizer->Add(authority_display_panel, wxSizerFlags().Expand());
	taskbar_centralize_sizer->AddSpacer(10);
	taskbar_centralize_panel->SetSizer(taskbar_centralize_sizer);
	taskbar_centralize_panel->Layout();

	wxBoxSizer* taskbar_sizer = new wxBoxSizer(wxVERTICAL);
	taskbar_sizer->AddStretchSpacer();
	taskbar_sizer->Add(taskbar_centralize_panel, wxSizerFlags().Expand());
	taskbar_sizer->AddStretchSpacer();
	taskbar_panel->SetSizer(taskbar_sizer);
	taskbar_panel->Layout();

	wxBoxSizer* profile_info_sizer = new wxBoxSizer(wxHORIZONTAL);
	profile_info_sizer->AddStretchSpacer();
	profile_info_sizer->Add(current_user_info_label, wxSizerFlags().Expand());
	profile_info_sizer->AddStretchSpacer();
	current_user_info_panel->SetSizer(profile_info_sizer);
	current_user_info_panel->Layout();

	PlaceOtherProfiles(user->GetName(), "");

	wxBoxSizer* edit_button_sizer = new wxBoxSizer(wxHORIZONTAL);
	edit_button_sizer->AddStretchSpacer(1);
	edit_button_sizer->Add(edit_current_user_info_button, wxSizerFlags(0).Expand());
	edit_button_sizer->AddSpacer(20);

	wxBoxSizer* profile_sizer = new wxBoxSizer(wxVERTICAL);
	profile_sizer->AddSpacer(50);
	profile_sizer->Add(edit_button_sizer, wxSizerFlags().Expand());
	profile_sizer->AddSpacer(10);
	profile_sizer->Add(profile_picture, wxSizerFlags().Expand());
	profile_sizer->AddSpacer(20);
	profile_sizer->Add(current_user_info_panel, wxSizerFlags().Expand());
	profile_scroll_panel->SetSizer(profile_sizer);
	profile_scroll_panel->Layout();

	wxBoxSizer* current_card_sizer = new wxBoxSizer(wxHORIZONTAL);
	current_card_sizer->Add(profile_scroll_panel, wxSizerFlags(335).Expand());
	current_card_sizer->AddStretchSpacer(1);
	current_card_sizer->Add(other_profiles_scroll_panel, wxSizerFlags(664).Expand());
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

void MainWindow::PlaceOtherProfiles(wxString current_user_name, wxString delimiter)
{
	wxBoxSizer* other_users_sizer = new wxBoxSizer(wxVERTICAL);
	currently_displayed_other_users.clear();
	other_profiles_labels_list.clear();
	edit_buttons.clear();
	delete_buttons.clear();
	if (delimiter != "")
	{
		//exact match
		for (User::user_info other_user : User::users)
			if (other_user.name == delimiter &&other_user.name != current_user_name && other_user.name != "Root Account")
			{
				currently_displayed_other_users.push_back(other_user);
				auto other_user_panel = new wxPanel(other_profiles_scroll_panel, wxID_ANY);
				other_profiles_labels_list.push_back(other_user_panel);
				ShapeOtherUsersLabels(other_user_panel, other_user);
				other_users_sizer->Add(other_user_panel, wxSizerFlags().Expand());
				other_users_sizer->AddSpacer(20);
			}

		//partial match
		for (User::user_info other_user : User::users)
		{
			wxString other_user_name = other_user.name;
			if (other_user_name.MakeLower().Find(delimiter) != wxNOT_FOUND && other_user.name != current_user_name && other_user.name != "Root Account")
			{
				currently_displayed_other_users.push_back(other_user);
				auto other_user_panel = new wxPanel(other_profiles_scroll_panel, wxID_ANY);
				other_profiles_labels_list.push_back(other_user_panel);
				ShapeOtherUsersLabels(other_user_panel, other_user);
				other_users_sizer->Add(other_user_panel, wxSizerFlags().Expand());
				other_users_sizer->AddSpacer(20);
			}
		}
	}
	else
		//all users(except for current)
		for (User::user_info other_user : User::users)
			if (other_user.name != current_user_name && other_user.name != "Root Account")
			{
				currently_displayed_other_users.push_back(other_user);
				auto other_user_panel = new wxPanel(other_profiles_scroll_panel, wxID_ANY);
				other_profiles_labels_list.push_back(other_user_panel);
				ShapeOtherUsersLabels(other_user_panel, other_user);
				other_users_sizer->Add(other_user_panel, wxSizerFlags().Expand());
				other_users_sizer->AddSpacer(20);
			}
	other_profiles_scroll_panel->SetSizer(other_users_sizer);
	other_profiles_scroll_panel->Layout();
}

wxImage MainWindow::GetProfileImage(wxString user_name, wxSize size)
{
	wxLogNull noLog;//supresses 'image not found error' (in case fallback image needed to be loaded)
	wxImage profile_image;
	wxString working_path = wxGetCwd();
	working_path += "\\Users\\users_profile_icons\\" + user_name + ".png";
	if (!profile_image.LoadFile(working_path, wxBITMAP_TYPE_ANY))
	{
		working_path = wxGetCwd();
		working_path += "\\Assets\\General\\user_profile.png";
		profile_image.LoadFile(working_path, wxBITMAP_TYPE_PNG);
	}
	profile_image = profile_image.Scale(size, wxIMAGE_QUALITY_NEAREST);
	return profile_image;
}

wxImage MainWindow::GetAssetsImage(wxString relative_address, wxSize size)
{
	wxImage profile_image;
	wxString working_path = wxGetCwd();
	working_path += "\\" + relative_address;
	if (!profile_image.LoadFile(working_path, wxBITMAP_TYPE_PNG))
		throw Exception("File not found");
	profile_image = profile_image.Scale(size, wxIMAGE_QUALITY_HIGH);
	return profile_image;
}

void MainWindow::SetProfileImage(wxString address, wxString user_name)
{
	wxImage profile_image;
	wxString working_path = wxGetCwd();
	working_path += "/Users/users_profile_icons/" + user_name;
	if (wxRenameFile(address, working_path))
	{

	}
	else
		throw Exception("File not found");
}

void MainWindow::ShapeOtherUsersLabels(wxPanel* parent_panel, User::user_info user_information)
{
	wxPanel* centralize_panel = new wxPanel(parent_panel, wxID_ANY);
	wxBitmap profile_bitmap(GetProfileImage(user_information.name, { 50, 50 }));
	wxStaticBitmap* profile_image = new wxStaticBitmap(centralize_panel, wxID_ANY, profile_bitmap);
	wxString other_user_information_string;
	if (user_information.authority == 0)
		other_user_information_string = "User ";
	else
		other_user_information_string = "Administrator ";
	other_user_information_string += user_information.name + " " + user_information.number + "\nParticipation in social events: " + 
		user_information.participation_in_soc_activities + "\nGPA:" + wxString::Format(wxT("%.2f"),user_information.gpa) + 
		"\nIncome per family member: " + wxString::Format(wxT("%.2f"),user_information.income_per_fam_member);
	wxStaticText* profile_information = new wxStaticText(centralize_panel, wxID_ANY, other_user_information_string);


	wxBoxSizer* profile_information_sizer = new wxBoxSizer(wxHORIZONTAL);
	profile_information_sizer->AddSpacer(13);
	profile_information_sizer->Add(profile_image, wxSizerFlags(20).Expand());
	profile_information_sizer->AddStretchSpacer(5);
	if (authority_display_label->GetLabel()=="Administrator")
	{
		wxBitmapButton* edit_button = new wxBitmapButton(centralize_panel, wxID_ANY, edit_bitmap);
		size_t index = edit_buttons.size();
		edit_buttons.push_back(edit_button);
		edit_button->Bind(wxEVT_BUTTON, [this, index](wxCommandEvent& event) {MainWindow::OnEditButtonClicked(event, index);});

		wxBitmapButton* delete_button = new wxBitmapButton(centralize_panel, wxID_ANY, delete_bitmap);
		delete_buttons.push_back(delete_button);
		delete_button->Bind(wxEVT_BUTTON, [this, index](wxCommandEvent& event) {MainWindow::OnDeleteButtonClicked(event, index);});

		profile_information_sizer->Add(profile_information, wxSizerFlags(55).Expand());
		profile_information_sizer->AddStretchSpacer(4);
		profile_information_sizer->Add(edit_button, wxSizerFlags(8));
		profile_information_sizer->AddSpacer(10);
		profile_information_sizer->Add(delete_button, wxSizerFlags(8));
		profile_information_sizer->AddSpacer(13);
	}
	else
		profile_information_sizer->Add(profile_information, wxSizerFlags(75).Expand());
	centralize_panel->SetSizer(profile_information_sizer);
	centralize_panel->Layout();

	wxBoxSizer* outer_sizer = new wxBoxSizer(wxVERTICAL);
	outer_sizer->AddStretchSpacer();
	outer_sizer->Add(centralize_panel);
	outer_sizer->AddStretchSpacer();
	parent_panel->SetSizer(outer_sizer);
	parent_panel->Layout();

}

void MainWindow::BindObjects()
{
	themes_dropdown_box->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &MainWindow::OnThemeOptionSelected, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnClose, this);
	search_field->Bind(wxEVT_TEXT, [=](wxCommandEvent& event) {MainWindow::OnSearchFieldChanged(event, user->GetName());});
	sort_order_button->Bind(wxEVT_BUTTON, &MainWindow::OnSortModeButtonClicked, this);
	sort_dropdown_box->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &MainWindow::OnSortOptionSelected, this);
	edit_current_user_info_button->Bind(wxEVT_BUTTON, &MainWindow::OnSelfEditButtonClicked, this);
	view_button->Bind(wxEVT_BUTTON, &MainWindow::OnViewButtonClicked, this);
	if (user->GetAuthority() == 1)
		add_button->Bind(wxEVT_BUTTON, &MainWindow::OnAddButtonClicked, this);
}

void MainWindow::PaintObjects(int theme)
{
	wxVector<wxWindow*> taskbar_children;
	switch (theme)
	{
	case 0:
		for (auto object : all_objects)
		{
			object->SetBackgroundColour(RGB(0, 0, 0));
			object->SetForegroundColour(RGB(255, 255, 255));
		}
		App::GetAllChildren(taskbar_panel, taskbar_children);
		taskbar_panel->SetBackgroundColour(RGB(13, 13, 13));
		for (auto child : taskbar_children)
			child->SetBackgroundColour(RGB(13, 13, 13));
		other_profiles_scroll_panel->SetBackgroundColour(RGB(5, 5, 5));
		break;
	case 1:
		for (auto object : all_objects)
		{
			object->SetBackgroundColour(RGB(255, 255, 255));
			object->SetForegroundColour(RGB(0, 0, 0));
		}
		App::GetAllChildren(taskbar_panel, taskbar_children); 
		taskbar_panel->SetBackgroundColour(RGB(245, 245, 245));
		for (auto child : taskbar_children)
			child->SetBackgroundColour(RGB(245, 245, 245));
		other_profiles_scroll_panel->SetBackgroundColour(RGB(250, 250, 250));
		break;
	}
	current_card_panel->SetBackgroundColour(RGB(130, 130, 130));
	panel->SetBackgroundColour(RGB(130, 130, 130));
	authority_display_label->SetForegroundColour(RGB(200, 35, 13));
	this->Refresh();
	this->Update();
}

void MainWindow::GetThemeIcons(int theme)
{
	switch (theme)
	{
	case 0:
		search_bitmap = GetAssetsImage("Assets\\theme 1\\magnifying_glass.png", { icon_size, icon_size });
		sort_bitmap = GetAssetsImage("Assets\\theme 1\\sort_ascending.png", { icon_size, icon_size });
		change_theme_bitmap = GetAssetsImage("Assets\\theme 1\\pointing_circles.png", { icon_size, icon_size });
		add_bitmap = GetAssetsImage("Assets\\theme 1\\plus.png", { icon_size, icon_size });
		delete_bitmap = GetAssetsImage("Assets\\theme 1\\trash_bin.png", { icon_size, icon_size });
		view_bitmap = GetAssetsImage("Assets\\theme 1\\eye.png", { icon_size, icon_size });
		break;
	case 1:
		search_bitmap = GetAssetsImage("Assets\\theme 2\\magnifying_glass.png", { icon_size, icon_size });
		sort_bitmap = GetAssetsImage("Assets\\theme 2\\sort_ascending.png", { icon_size, icon_size });
		change_theme_bitmap = GetAssetsImage("Assets\\theme 2\\pointing_circles.png", { icon_size, icon_size });
		add_bitmap = GetAssetsImage("Assets\\theme 2\\plus.png", { icon_size, icon_size });
		delete_bitmap = GetAssetsImage("Assets\\theme 2\\trash_bin.png", { icon_size, icon_size });
		view_bitmap = GetAssetsImage("Assets\\theme 2\\eye.png", { icon_size, icon_size });
		break;
	}
	if (!edit_bitmap.IsOk())
		edit_bitmap = GetAssetsImage("Assets\\General\\pencil.png", { icon_size, icon_size });
}

void MainWindow::OnSearchFieldChanged(wxCommandEvent& event, wxString user_name)
{
	wxString searched_string = search_field->GetValue().Lower();
	for (auto other_profile_label : other_profiles_labels_list)
		other_profile_label->Destroy();
	PlaceOtherProfiles(user_name, searched_string);
	all_objects.clear();
	App::GetAllChildren(this, all_objects);
	PaintObjects(themes_dropdown_box->GetSelection());
}

void MainWindow::OnSortOptionSelected(wxCommandEvent& event)
{
	Sort(sort_dropdown_box->GetSelection());
}

void MainWindow::OnSortModeButtonClicked(wxCommandEvent& event)
{
	if (ascending)
	{
		ascending = false;
		switch (themes_dropdown_box->GetSelection())
		{
		case 0:
			sort_bitmap = GetAssetsImage("Assets\\theme 1\\sort_descending.png", {icon_size, icon_size});
			break;
		case 1:
			sort_bitmap = GetAssetsImage("Assets\\theme 2\\sort_descending.png", { icon_size, icon_size });
			break;
		}
	}
	else
	{
		ascending = true;
		switch (themes_dropdown_box->GetSelection())
		{
		case 0:
			sort_bitmap = GetAssetsImage("Assets\\theme 1\\sort_ascending.png", { icon_size, icon_size });
			break;
		case 1:
			sort_bitmap = GetAssetsImage("Assets\\theme 2\\sort_ascending.png", { icon_size, icon_size });
			break;
		}
	}
	sort_order_button->SetBitmapLabel(sort_bitmap);
	sort_order_button->Refresh();
	sort_order_button->Update();
	if (sort_dropdown_box->GetSelection() != wxNOT_FOUND)
		Sort(sort_dropdown_box->GetSelection());
}

void MainWindow::OnThemeOptionSelected(wxCommandEvent& event)
{
	int current_theme = themes_dropdown_box->GetSelection();
	if (current_theme != user->GetTheme())
	{
		GetThemeIcons(current_theme);
		themes_image->SetBitmap(change_theme_bitmap);
		sort_order_button->SetBitmapLabel(sort_bitmap);
		search_image->SetBitmap(search_bitmap);

		PaintObjects(current_theme);
		user->SetTheme(current_theme);
	}
}

void MainWindow::OnSelfEditButtonClicked(wxCommandEvent& event)
{
	User::user_info user_fields = { .name = user->GetName(), .incrypted_password = User::ApplyCipher(user->GetPassword(), user->GetName(), 0),
				.participation_in_soc_activities = user->GetParticipation(),  .authority = user->GetAuthority(),
				.number = user->GetNumber(), .gpa = user->GetGPA(), .income_per_fam_member = user->GetIncomePerFamMember(), .theme = user->GetTheme() };
	OpenEditWindow(user_fields, user->GetAuthority(), true);
}

void MainWindow::OnEditButtonClicked(wxCommandEvent& event, size_t user_number)
{
	if (authority_display_label->GetLabel() == "Administrator")
		OpenEditWindow(currently_displayed_other_users[user_number], 1, false);
}

void MainWindow::OnDeleteButtonClicked(wxCommandEvent& event, size_t user_number)
{
	wxMessageDialog dialog(NULL, "Are you sure you want to delete " + currently_displayed_other_users[user_number].name, "Confirm action", wxYES_NO | wxICON_WARNING | wxCENTRE);
	if (dialog.ShowModal() == wxID_YES) 
	{
		other_profiles_labels_list[user_number]->Destroy();
		other_profiles_labels_list.erase(other_profiles_labels_list.begin() + user_number);

		User::users.erase(std::remove_if(User::users.begin(), User::users.end(), 
			[&](const User::user_info& element)
			{
				return element.name == currently_displayed_other_users[user_number].name;
			}),
			User::users.end());
		User::DeleteUser(currently_displayed_other_users[user_number]);
		currently_displayed_other_users.erase(currently_displayed_other_users.begin() + user_number);
		delete_buttons.erase(delete_buttons.begin() + user_number);
		edit_buttons.erase(edit_buttons.begin() + user_number);
		Sort(sort_dropdown_box->GetSelection());
	}
}

void MainWindow::OnAddButtonClicked(wxCommandEvent& event)
{
	User::user_info new_user_info = {
		.name = "Nameless",
		.incrypted_password = "", 
		.participation_in_soc_activities = "None",
		.authority = 0,
		.number = "",
		.gpa = 0,
		.income_per_fam_member = 0,
		.theme = 0
	};

	if (authority_display_label->GetLabel() == "Administrator")
		OpenEditWindow(new_user_info, 1, false);
	wxLogMessage(wxT("New account added"));
}

void MainWindow::OnViewButtonClicked(wxCommandEvent& event)
{
	float* gpas = new float[currently_displayed_other_users.size()];
	for (size_t i = 0; i < currently_displayed_other_users.size(); ++i)
		gpas[i] = currently_displayed_other_users[i].gpa;

	float* incomes = new float[currently_displayed_other_users.size()];
	for (size_t i = 0; i < currently_displayed_other_users.size(); ++i)
		incomes[i] = currently_displayed_other_users[i].income_per_fam_member;

	int64_t* activity = new int64_t[currently_displayed_other_users.size()];
	for (size_t i = 0; i < currently_displayed_other_users.size(); ++i)
	{
		if (currently_displayed_other_users[i].participation_in_soc_activities == "None")
			activity[i] = 0;
		else if (currently_displayed_other_users[i].participation_in_soc_activities == "Low")
			activity[i] = 1;
		else if (currently_displayed_other_users[i].participation_in_soc_activities == "Medium")
			activity[i] = 2;
		else if (currently_displayed_other_users[i].participation_in_soc_activities == "High")
			activity[i] = 3;
		else
			activity[i] = 0;
	}

	int64_t* new_other_users_to_display = DetectBelowAverageUsers(gpas, incomes, activity, currently_displayed_other_users.size());

	for (auto other_profile_label : other_profiles_labels_list)
		other_profile_label->Destroy();
	other_profiles_labels_list.clear();
	edit_buttons.clear();
	delete_buttons.clear();

	wxBoxSizer* other_users_sizer = new wxBoxSizer(wxVERTICAL);
	//5 users to display (according to the task)
	size_t newly_displayed = 0;
	try
	{
		for (size_t i = 0; i < 5; ++i)
			if (i < currently_displayed_other_users.size())
			{
				auto other_user_panel = new wxPanel(other_profiles_scroll_panel, wxID_ANY);
				other_profiles_labels_list.push_back(other_user_panel);
				ShapeOtherUsersLabels(other_user_panel, currently_displayed_other_users.at(new_other_users_to_display[i]));
				currently_displayed_other_users.push_back(currently_displayed_other_users.at(new_other_users_to_display[i]));
				++newly_displayed;
				other_users_sizer->Add(other_user_panel, wxSizerFlags().Expand());
				other_users_sizer->AddSpacer(20);
			}
	}
	catch (std::out_of_range)
	{
		//in case there were less than 5 'below' average folks
	}
	other_profiles_scroll_panel->SetSizer(other_users_sizer);
	other_profiles_scroll_panel->Layout();
	all_objects.clear();
	App::GetAllChildren(this, all_objects);
	PaintObjects(themes_dropdown_box->GetSelection());
	currently_displayed_other_users.erase(currently_displayed_other_users.begin(), currently_displayed_other_users.end() - newly_displayed);
	FreeMemory(new_other_users_to_display);
}

void MainWindow::OnClose(wxCloseEvent& event)
{
	user->SetTheme(themes_dropdown_box->GetSelection());
	user->SaveUserInfo();
	event.Skip();
}

void MainWindow::Sort(int subject)
{
	for (auto other_profile_label : other_profiles_labels_list)
		other_profile_label->Destroy();
	other_profiles_labels_list.clear();
	edit_buttons.clear();
	delete_buttons.clear();
	//sorting gotta be inverted cz I display labels top-to-bottom
	User::Sort(currently_displayed_other_users, subject, !ascending);
	wxBoxSizer* other_users_sizer = new wxBoxSizer(wxVERTICAL);
	for (User::user_info other_user : currently_displayed_other_users)
	{
		auto other_user_panel = new wxPanel(other_profiles_scroll_panel, wxID_ANY);
		other_profiles_labels_list.push_back(other_user_panel);
		ShapeOtherUsersLabels(other_user_panel, other_user);
		other_users_sizer->Add(other_user_panel, wxSizerFlags().Expand());
		other_users_sizer->AddSpacer(20);
	}
	other_profiles_scroll_panel->SetSizer(other_users_sizer);
	other_profiles_scroll_panel->Layout();
	all_objects.clear();
	App::GetAllChildren(this, all_objects);
	PaintObjects(themes_dropdown_box->GetSelection());
}

void MainWindow::OpenEditWindow(User::user_info user, bool caller_authority, bool self_edition)
{
	EditWindow* edit_window = new EditWindow(user, caller_authority, self_edition);
	edit_window->Center();
	edit_window->Show();

	//not to display unedited user right away
	MSG msg;
	while (edit_window->IsShown())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);//to allow to type stuff
			DispatchMessage(&msg);//to allow process the window (interaction, drawing in the first place, etc.)
		}
		else
			Sleep(100);
	}

	if (self_edition)
	{
		this->user->ReloadUser();
		wxString user_information_string =
			"Name: " + this->user->GetName() +
			"\nParticipation in social events: " + this->user->GetParticipation() +
			"\nNumber: " + this->user->GetNumber() +
			"\nGrade point average: " + wxString::Format(wxT("%.2f"), this->user->GetGPA()) +
			"\nIncome per family member: " + wxString::Format(wxT("%.2f"), this->user->GetIncomePerFamMember());
		current_user_info_label->SetLabel(user_information_string);
	}
	else
	{
		for (auto oprofile : other_profiles_labels_list)
			oprofile->Destroy();
		PlaceOtherProfiles(this->user->GetName(), search_field->GetValue());

		all_objects.clear();
		App::GetAllChildren(this, all_objects);
		PaintObjects(this->user->GetTheme());
	}
}
