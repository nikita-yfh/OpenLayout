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

		static const long ID_TYPES[3];
		static const long ID_PANELS[4];
		static const long ID_WIDTH;
		static const long ID_HEIGHT;
		static const long ID_DIAMETER;
		static const long ID_BORDER;
		static const long ID_INPUT_NAME;
		static const long ID_ALL_WIDTH;
		static const long ID_ALL_HEIGHT;

		void SetType(BoardType type);
		void RecalcSize(wxSpinDoubleEvent &e);
		void ChangeText(wxCommandEvent &e);
};

#endif
