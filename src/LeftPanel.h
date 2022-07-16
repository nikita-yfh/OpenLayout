#pragma once
#include <wx/panel.h>
#include <wx/toolbar.h>
#include <wx/spinctrl.h>
#include "PCB.h"
#include "Settings.h"

class LeftPanel : public wxPanel {
public:
	LeftPanel(wxWindow *parent, PCB &pcb, Settings &settings);
private:
	wxToolBar *toolbar;
	PCB &pcb;
	Settings &settings;
	bool rectFill;
	uint8_t padShape;

	wxSpinCtrlDouble *trackSize;
	wxSpinCtrlDouble *padSizeOuter;
	wxSpinCtrlDouble *padSizeInner;
	wxSpinCtrlDouble *smdWidth;
	wxSpinCtrlDouble *smdHeight;

	void PopupToolbarMenu(wxMenu *menu, int n);

	static void AddSubmenu(wxMenu *parent, wxMenu *child, const char *text, const wxBitmap &bitmap, bool enabled = true);
	static void AddItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap, bool enabled = true);
	static void AddCheckItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap, bool enabled = true);

	void UpdateSizes(wxUpdateUIEvent&);

	void ShowGridBinder(wxCommandEvent&);
	void AddNewGrid(wxCommandEvent&);
	void RemoveGrid(wxCommandEvent&);
	void SetSubgrid(wxCommandEvent&);
	void SetGridStyle(wxCommandEvent&);
	void ToggleGrid(wxCommandEvent&);

	void ShowPadSizeMenu(wxCommandEvent&);
	void ShowRectFillMenu(wxCommandEvent&);
	void ShowSpecialFormsDialog(wxCommandEvent&);
	void ShowGridMenu(wxCommandEvent&);
	void ShowTrackMenu(wxCommandEvent&);
	void ShowPadMenu(wxCommandEvent&);
	void ShowSmdMenu(wxCommandEvent&);

	void SelectGrid(wxCommandEvent&);
	void UpdateGrid(wxUpdateUIEvent&);

	void SetTrackSize(float size);
	void SetTrackSize(wxSpinDoubleEvent&);
	void SelectTrackSize(wxCommandEvent&);
	void RemoveTrackSize(wxCommandEvent&);
	void AddTrackSize(wxCommandEvent&);

	void SetPadSize(const PadSize &size);
	void SetPadSize(wxSpinDoubleEvent&);
	void SelectPadSize(wxCommandEvent&);
	void RemovePadSize(wxCommandEvent&);
	void AddPadSize(wxCommandEvent&);

	void SetSmdSize(const Vec2 &size);
	void SetSmdSize(wxSpinDoubleEvent&);
	void SwapSmdSize(wxCommandEvent&);
	void SelectSmdSize(wxCommandEvent&);
	void RemoveSmdSize(wxCommandEvent&);
	void AddSmdSize(wxCommandEvent&);

	void SetRectFill(wxCommandEvent&);
	void SetPadShape(wxCommandEvent&);
	void ToggleMetallization(wxCommandEvent&);
	void UpdateMetallization(wxUpdateUIEvent&);

	void UpdatePad(bool metallization, uint8_t shape);

	wxDECLARE_EVENT_TABLE();
};
