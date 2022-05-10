#pragma once
#include "Settings.h"
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>

class InputGridDialog : public wxDialog {
public:
	InputGridDialog(wxWindow *parent, const Settings *settings, double init);
	double Get() const;

	static double Show(wxWindow *parent, const Settings *settings, double init);
private:
	wxSpinCtrlDouble *input;
	wxChoice *units;
};


