#pragma once
#include <wx/toolbar.h>

class ToolBar : public wxToolBar{
public:
	ToolBar(wxWindow *parent);
private:
	void Align(wxCommandEvent&);
	void Zoom(wxCommandEvent&);
	wxDECLARE_EVENT_TABLE();
};
