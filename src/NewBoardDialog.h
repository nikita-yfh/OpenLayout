#pragma once

#include "Board.h"

#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

class NewBoardDialog : public wxDialog {
public:
	NewBoardDialog(wxWindow* parent);

	Board *CreateBoard(bool originTop) const;
private:

	wxRadioButton *types[3];
	wxPanel *panels[4];

	wxSpinCtrlDouble *width;
	wxSpinCtrlDouble *height;
	wxSpinCtrlDouble *diameter;
	wxSpinCtrlDouble *border;
	wxTextCtrl *name;
	wxStaticText *allWidth;
	wxStaticText *allHeight;

	void SetEmpty(wxCommandEvent&);
	void SetRectangle(wxCommandEvent&);
	void SetRound(wxCommandEvent&);
	void RecalcSize(wxCommandEvent&);
};

