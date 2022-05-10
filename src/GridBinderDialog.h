#pragma once
#include "Settings.h"
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/listctrl.h>

class GridBinderDialog : public wxDialog {
public:
	GridBinderDialog(wxWindow *window, Settings *settings);
	void Get();

	static void Show(wxWindow *parent, Settings *settings);
private:
	wxListView *keyList;
	wxButton *change;
	Settings *settings;
	double grids[9];

	void OnSelectedItem(wxListEvent&);
	void OnActivatedItem(wxListEvent&);
	void ChangeGrid(wxCommandEvent&);

	wxDECLARE_EVENT_TABLE();
};

