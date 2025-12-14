#pragma once
#include <wx/wx.h>
#include "User.h"

class EditWindow : public wxFrame
{
public:
	EditWindow(User::user_info user, bool unlimited_power, bool self_edition);

private:
	wxString pre_changed_name;
	User* currently_editing_user;

	void InitializeObjects(User::user_info user, bool unlimited_power, bool self_edition);
	void PlaceObjects(bool unlimited_power, bool self_edition);
	void PaintObjects(int theme);
	void BindObjects();

	void OnSaveButtonClicked(wxCommandEvent& event);
	void OnConfirmButtonsClicked(wxCommandEvent& event);	//kinda pointless (cz they can just close the window), but we all love to save explicitely, right?
	void OnClose(wxCloseEvent& event);

	bool Save();

	wxPanel* panel;
	
	wxStaticText* name_guide;
	wxStaticText* password_guide;
	wxStaticText* activity_guide;
	wxStaticText* authority_guide;
	wxStaticText* number_guide;
	wxStaticText* gpa_guide;
	wxStaticText* income_guide;

	wxTextCtrl* name_field;
	wxTextCtrl* password_field;
	wxTextCtrl* activity_field;
	wxTextCtrl* authority_field;
	wxTextCtrl* number_field;
	wxTextCtrl* gpa_field;
	wxTextCtrl* income_field;
	
	wxButton* confirm_button;		//save and quit
	wxButton* save_button;
};
