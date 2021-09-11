#pragma once

#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

class TextTooltip : wxDialog{
public:
	TextTooltip(wxWindow *parent);
	void Show(int mousex,int mousey,wxString text);
	void Hide();
private:
	wxStaticText *text;
};
