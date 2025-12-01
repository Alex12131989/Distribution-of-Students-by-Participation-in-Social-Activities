#pragma once
#include <wx/bitmap.h>
#include <wx/filefn.h>
#include <wx/string.h>
#include <wx/image.h>
#include <wx/wx.h>
#include "User.h"

class MainWindow : public wxFrame
{
public:
	MainWindow(const wxString& title, User* user);

private:
	//variables
	int icon_size = 20;
	bool ascending = true;

	//objects
	wxPanel* panel;
	wxPanel* taskbar_panel;
	wxPanel* taskbar_centralize_panel;
	wxPanel* authority_display_panel;
	wxPanel* current_card_panel;
	wxPanel* current_user_info_panel;
	wxPanel* search_panel;
	wxPanel* themes_panel;
	wxPanel* sort_panel;

	wxScrolledWindow* profile_scroll_panel;
	wxScrolledWindow* other_profiles_scroll_panel;
	
	wxStaticText* authority_display_label;
	wxStaticText* current_user_info_label;

	wxTextCtrl* search_field;

	wxChoice* themes_dropdown_box;
	wxChoice* sort_dropdown_box;

	wxStaticBitmap* profile_picture;

	wxBitmapButton* sort_order_button;

	//images
	wxStaticBitmap* search_image;
	wxStaticBitmap* themes_image;
	wxBitmap sort_bitmap;
	wxBitmap search_bitmap;
	wxBitmap change_theme_bitmap;
	wxBitmap add_bitmap;
	wxBitmap delete_bitmap;

	//lists
	//wxArrayString working_with_acounts_admin_list = {"View", "Add", "Edit", "Delete"};
	wxVector<wxWindow*> all_objects;
	wxVector<User::user_info> currently_displayed_other_users;
	wxArrayString themes_options_list = { "BnW", "WnB" };
	wxArrayString sort_options_list = {"Sort By Grade Point Average", "Sort By Income Per Family Member"};
	//-----------
	wxVector<wxPanel*> other_profiles_labels_list;

	//initialization and placement
	void InitializeObjects(User* user);
	void PlaceObjects(User* user);
	void PlaceOtherProfiles(wxString curren_user_name, wxString delimiter);

	//idk how to call it '_' arrrrgh
	wxImage GetProfileImage(wxString user_name, wxSize size);
	wxImage GetAssetsImage(wxString relative_address, wxSize size);
	void SetProfileImage(wxString address, wxString user_name);
	void ShapeOtherUsersLabels(wxPanel* parent_panel, User::user_info user_information);

	//actions
	void BindObjects(User* user);

	//action logic
	void OnSearchFieldChanged(wxCommandEvent& event, User* user);
	void OnSortOptionSelected(wxCommandEvent& event, User* user);
	void OnSortModeButtonClicked(wxCommandEvent& event, User* user);
	void OnThemeOptionSelected(wxCommandEvent& event, User* user);
	void OnClose(wxCloseEvent& event, User* user);
	//---
	void PaintObjects(User* user, int theme);
	void GetThemeIcons(int theme);
	void Sort(int subject, User* user);
};