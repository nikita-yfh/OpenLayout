#include "GridBinderDialog.h"
#include "InputGridDialog.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include "Utils.h"
#include "InputGridDialog.h"

#define KEYBINDER_XPM
#include "images.h"
#undef KEYBINDER_XPM

GridBinderDialog::GridBinderDialog(wxWindow* parent,const double k[9])
    :wxDialog(parent, wxID_ANY, _("Grid values for Keys 1..9")) {
    wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
    {
    	memcpy(keys,k,sizeof(keys));
    	auto bind=[&](wxCommandEvent &e){
    		wxButton *button=static_cast<wxButton*>(e.GetEventObject());
    		for(int q=0;q<9;q++){
    			if(buttons[q]==button){
    				InputGridDialog input(this);
    				if(input.ShowModal()==wxID_OK){
    					double grid=input.Get();
    					text[q]->SetLabel(get_grid_str(grid));
    					keys[q]=grid;
    					Layout();
    				}
    			}
    		}
    	};
		wxGridSizer *sizer=new wxFlexGridSizer(9,3,0,20);
		for(int q=0;q<9;q++){
			sizer->Add(new wxStaticText(this,wxID_ANY,char(q+'1')),0,wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL);
			text[q]=new wxStaticText(this,wxID_ANY,get_grid_str(k[q]));
			sizer->Add(text[q],1,wxALIGN_RIGHT|wxALIGN_CENTRE_VERTICAL);
			buttons[q]=new wxButton(this,wxID_ANY,"Change");
		//	buttons[q]->SetBitmap(arrow_xpm);
			buttons[q]->Bind(wxEVT_BUTTON,bind);
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

void GridBinderDialog::Get(double k[9]){
	memcpy(k,keys,sizeof(keys));
}
void ShowGridBinderDialog(wxWindow *parent,double keys[9]){
	GridBinderDialog dialog(parent,keys);
	if(dialog.ShowModal()==wxID_OK)
		dialog.Get(keys);
}

