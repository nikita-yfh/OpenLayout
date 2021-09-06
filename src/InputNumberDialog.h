#pragma once
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <float.h>

class InputNumberDialog: public wxDialog {
public:
	InputNumberDialog(wxWindow* parent,float min,float max,float init,float step);
	float Get();
	wxSpinCtrlDouble *input;
};

float ShowInputNumberDialog(wxWindow* parent,float min,float max,float init,float step);

