#pragma once
#include <wx/wx.h>
#include <wx/list.h>


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	//variable

	//objects
	wxPanel* panel;
	wxPanel* get_password_panel;
	wxPanel* buttons_panel;

	wxStaticText* welcome_label;
	wxStaticText* instruction_name_label;
	wxStaticText* instruction_password_label;

	wxTextCtrl* enter_name_field;
	wxTextCtrl* enter_password_field;
	wxButton* sign_button;
	wxButton* change_theme_button;
	wxButton* confirm_button;

	//lists
	//wxWindowList all_objects;

	//methods
	void InitializeObjects();
	void PlaceObjects();
	void BindObjects();
	void GetAllChildren(wxWindow* parent, wxWindowList& all_children);
	void PaintObjects();
};