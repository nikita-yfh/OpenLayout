#include "InputNumberDialog.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include "Utils.h"

InputNumberDialog::InputNumberDialog(wxWindow* parent,float min,float max,float init,float step)
	:wxDialog(parent, wxID_ANY, _("New grid")) {
	wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
	{
		input=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,min,max,init,step);
		all_box->Add(input,1,wxEXPAND|wxALL,5);
	}
	{
		wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
		buttons->AddButton(new wxButton(this, wxID_OK));
		buttons->AddButton(new wxButton(this, wxID_CANCEL));
		buttons->Realize();
		all_box->Add(buttons, 0, wxALL|wxEXPAND, 10);
	}
	SetSizerAndFit(all_box);
}

float InputNumberDialog::Get() {
	return input->GetValue();
}

float ShowInputNumberDialog(wxWindow* parent,float min,float max,float init,float step) {
	InputNumberDialog dialog(parent,min,max,init,step);
	if(dialog.ShowModal()!=wxID_OK)
		return FLT_MAX;
	return dialog.Get();
}
