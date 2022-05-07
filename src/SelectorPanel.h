#pragma once

#include <wx/panel.h>
#include <wx/choice.h>
#include "PCB.h"

class SelectorPanel : public wxPanel {
public:
	SelectorPanel(wxWindow *parent, PCB *pcb);

private:

	void UpdateSortingList(wxCommandEvent&);

	PCB *pcb;

	wxChoice *sorting;
	wxDECLARE_EVENT_TABLE();
};
