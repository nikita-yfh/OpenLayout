#include "InputGridDialog.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include "Utils.h"

InputGridDialog::InputGridDialog(wxWindow* parent)
		:wxDialog(parent, wxID_ANY, _("New grid")){
    wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
    {
    	wxBoxSizer *content=new wxBoxSizer(wxHORIZONTAL);
		{
			input=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0.001f,10.0f,1.0f,0.1f);
			content->Add(input,1,wxEXPAND|wxALL,5);
		}
		{
			wxString list[]={_("mm"),_("um"),_("mil")};
			units=new wxChoice(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,3,list);
			units->SetSelection(0);
			content->Add(units,0,wxEXPAND|wxALL,5);
		}
		all_box->Add(content,1,wxEXPAND|wxALL,10);
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

float InputGridDialog::Get(){
	int sel=units->GetSelection();
	float val=input->GetValue();
	if(sel==0) //mm
		return val;
	else if(sel==1) //um
		return val/1000.0f;
	else //mil
		return to_mm(val);
}
