#include "GridBinderDialog.h"
#include "InputGridDialog.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include "Utils.h"

#define KEYBINDER_XPM
#include "images.h"
#undef KEYBINDER_XPM

GridBinderDialog::GridBinderDialog(wxWindow* parent,double keys[9])
    :wxDialog(parent, wxID_ANY, _("Grid values for Keys 1..9")) {
    wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
    {
		wxGridSizer *sizer=new wxFlexGridSizer(9,3,0,20);
		for(int q=0;q<9;q++){
			sizer->Add(new wxStaticText(this,wxID_ANY,char(q+'1')),0,wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL);
			text[q]=new wxStaticText(this,wxID_ANY,get_grid_str(keys[q]));
			sizer->Add(text[q],1,wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL);
			buttons[q]=new wxButton(this,wxID_ANY,"Change");
			buttons[q]->SetBitmap(arrow_xpm);
			sizer->Add(buttons[q],1);
		}
        all_box->Add(sizer,1,wxEXPAND|wxALL,10);
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
