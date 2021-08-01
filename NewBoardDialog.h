#ifndef NEWBOARDDIALOG_H
#define NEWBOARDDIALOG_H
#include "Board.h"

#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

class NewBoardDialog: public wxDialog
{
	public:
		NewBoardDialog(wxWindow* parent);
		Board board;

	private:
		
		wxRadioButton *types[3];
		wxPanel *panels[4];
		
		wxSpinCtrlDouble *width;
		wxSpinCtrlDouble *height;
		wxSpinCtrlDouble *diameter;
		wxSpinCtrlDouble *border;
		wxTextCtrl *input_name;
		wxStaticText *all_width;
		wxStaticText *all_height;
		
		void SetType(const BoardType &b);
};

#endif
