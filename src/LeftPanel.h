#pragma once
#include <wx/panel.h>
#include <wx/toolbar.h>
#include "PCB.h"

class LeftPanel : public wxPanel {
public:
	LeftPanel(wxWindow *parent, PCB *pcb);
private:
	wxToolBar *toolbar;
	PCB *pcb;
	bool rectFill;

	void PopupMenu(wxMenu *menu, int n);

	static void AddItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap);
	static void AddCheckItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap);

	void ShowPadMenu(wxCommandEvent&);
	void ShowRectMenu(wxCommandEvent&);

	void SetRectFill(wxCommandEvent&);
	void SetPadShape(wxCommandEvent&);
	void ToggleMetallization(wxCommandEvent&);
	void UpdateMetallization(wxUpdateUIEvent&);

	void UpdatePad(bool metallization, uint8_t shape);

	wxDECLARE_EVENT_TABLE();
};
