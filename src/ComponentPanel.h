#pragma once

#include "PCB.h"
#include <wx/panel.h>
#include <wx/listctrl.h>

class ComponentPanel : public wxPanel {
public:
	ComponentPanel(wxWindow *parent, const PCB *pcb);
private:

	template<int n>
	void ToggleColumn(wxCommandEvent &e) {
		list->SetColumnWidth(n, e.IsChecked() ? wxLIST_AUTOSIZE_USEHEADER : 0);
	}

	void Export(wxCommandEvent&);

	const PCB *pcb;

	wxListCtrl *list;
	wxDECLARE_EVENT_TABLE();
};
