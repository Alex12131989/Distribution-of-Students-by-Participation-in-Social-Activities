#pragma once
#include <wx/wx.h>
#include "User.h"

class EditWindow : public wxFrame
{
public:
	EditWindow(User::user_info user, bool unlimited_power);

private:
	void InitializeObjects(User::user_info user, bool unlimited_power);
	void PlaceObjects(bool unlimited_power);
	void PaintObjects();
	void BindObjects();

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

	wxButton* confirm_button;
	wxButton* save_button;
};