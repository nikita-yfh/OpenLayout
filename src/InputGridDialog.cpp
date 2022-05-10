#include "InputGridDialog.h"
#include <wx/msgdlg.h>
#include <wx/sizer.h>

InputGridDialog::InputGridDialog(wxWindow *parent, const Settings *settings, double init)
		: wxDialog(parent, wxID_ANY, _("New grid")) {
	wxString list[] = {_("mm"), _(L"\x00b5m"), _("mil")};
	units = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, list);

	char mm[50];
	if(settings->units == UNITS_MIL) {
		init /= 0.0254;
		units->SetSelection(2);
	}else if(snprintf(mm, 50, "%g", init) > 6) {
		init *= 1000.0;
		units->SetSelection(1);
	} else
		units->SetSelection(0);

	input = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
			wxSize(100, -1), wxSP_ARROW_KEYS, 0.00001, 999999.0, init, 0.1);
	input->SetDigits(6);

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(input, 1, wxALL, 5);
	sizer->Add(units, 0, wxALL, 5);
	content->Add(sizer, 1, wxEXPAND | wxALL, 10);
	content->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);

	input->SetFocus();
	SetSizerAndFit(content);
}

double InputGridDialog::Get() const {
	int selection = units->GetSelection();
	double value = input->GetValue();
	if(selection == 2) //mil
		return value * 0.0254;
	else if(selection == 1) //um
		return value * 0.001;
	return value;
}

double InputGridDialog::Show(wxWindow *parent, const Settings *settings, double init) {
	InputGridDialog *dialog = new InputGridDialog(parent, settings, init);
	if(dialog->ShowModal() != wxID_OK)
		return 0.0;
	double value = dialog->Get();
	if(value > 10.0) {
		wxMessageBox(_("The grid must not exceed 10 mm"), wxEmptyString, wxICON_ERROR);
		return 0.0;
	}
	return value;
}

