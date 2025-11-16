#pragma once
#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/list.h>


class AuthenticationWindow : public wxFrame
{
public:
	AuthenticationWindow(const wxString& title);

private:
	//variable
	static const int MAX_THEMES = 2;
	int theme = 0;

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
	wxWindowList all_objects;

	//initialization and placement
	void InitializeObjects();
	void PlaceObjects();

	//actions
	void BindObjects();
	void OnSignButtonClicked(wxCommandEvent& event);
	void OnChangeThemeButtonClicked(wxCommandEvent& event);
	void OnConfirmButtonClicked(wxCommandEvent& event);

	//actions logic
	void ChangeSigningMode();
	void PaintObjects(int theme);
	void CredentialValidation();

	//additional methods
	void GetAllChildren(wxWindow* parent, wxWindowList& all_children);
};