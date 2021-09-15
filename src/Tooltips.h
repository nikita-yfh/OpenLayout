#pragma once

#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "Utils.h"

class TextTooltip : wxDialog{
public:
	TextTooltip(wxWindow *parent);
	void Show(Vec2i mouse,wxString text);
	void Hide();
private:
	wxStaticText *text;
};

class MeasureTooltip : wxDialog{
public:
	MeasureTooltip(wxWindow *parent);
	void Show(int mousex,int mousey);
	void Show(Vec2i mouse,Vec2 v);
	void Show(Vec2i mouse,Vec2 v1,Vec2 v2,bool show_angle);
	void Hide();
	void UpdateColors();
private:
	wxStaticText *texts[6];
};
