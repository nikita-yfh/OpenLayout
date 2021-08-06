#ifndef INPUT_GRID_H
#define INPUT_GRID_H

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>

class InputGridDialog: public wxDialog {
public:
    InputGridDialog(wxWindow* parent);
	float Get(); //in mm

	wxSpinCtrlDouble *input;
	wxChoice *units;
};

#endif
