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

class MeasureTooltip : wxDialog{
public:
	MeasureTooltip(wxWindow *parent);
	void Show(int mousex,int mousey,float x,float y);
	void Show(int mousex,int mousey,float x,float y,float dx,float dy);
	void Hide();
	void UpdateTheme(bool light);
private:
	wxStaticText *texts[6];
};
