#pragma once
#include "wx/wx.h"
#include "User.h"

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, User* user);

private:
	//variables

	//objects
	wxPanel* panel;
	wxPanel* taskbar_panel;
	wxPanel* taskbar_centeralize_panel;
	wxPanel* current_card_panel;

	wxScrolledWindow* profile_scroll_panel;
	wxScrolledWindow* other_profiles_scroll_panel;
	
	wxStaticText* authority_display_label;

	wxChoice* working_with_accounts_dropdown_box;
	wxChoice* working_with_data_dropdow_box;
	wxChoice* working_with_data_edit_dropdown_box;
	wxChoice* working_with_data_processing_dropdown_box;

	//move it somewhere, don't keep it global
	wxBitmap* profile_picture;

	//lists
	wxArrayString working_with_acounts_admin_list = {"View", "Add", "Edit", "Delete"};
	wxArrayString working_with_acounts_user_list = { "View", "*Individual Task*", "Search", "Sort" };
	wxArrayString working_with_data_admin_list = { "Edit mode", "Processing mode" };
	wxArrayString working_with_data_edit_admin_list = { "View", "Add", "Edit", "Delete" };
	wxArrayString working_with_data_processing_admin_list = { "*Individual Task*", "Search", "Sort"};
	//-----------
	wxVector<wxPanel*> other_profiles_labels_list;
	wxVector<User::user_info> all_users;

	//initialization and placement
	void InitializeObjects(const int access_mode);
	void PlaceObjects(const int access_mode);

	//actions
	void BindObjects(const int access_mode);

	//action logic
	void PaintObjects(User* user);
	void GetProfileImage();
};