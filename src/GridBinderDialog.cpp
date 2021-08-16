#include "GridBinderDialog.h"
#include "InputGridDialog.h"
#include "Utils.h"
#include "InputGridDialog.h"

GridBinderDialog::GridBinderDialog(wxWindow* parent,const double g[9])
    :wxDialog(parent, wxID_ANY, _("Grid values for Keys 1..9")) {
    wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
    {
    	memcpy(grids,g,sizeof(grids));
		key_list=new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxBORDER_SIMPLE);

		key_list->AppendColumn(_("Key"), wxLIST_FORMAT_LEFT, 50);
		key_list->AppendColumn(_("Grid"), wxLIST_FORMAT_LEFT, 100);
		for(int q=0;q<9;q++){
			key_list->InsertItem(q,char(q+1));
			key_list->SetItem(q,0,char(q+'1'));
			key_list->SetItem(q,1,get_grid_str(g[q]));
		}
        key_list->Bind(wxEVT_LIST_ITEM_SELECTED,[&](wxCommandEvent&){change->Enable(true);});
        all_box->Add(key_list,1,wxEXPAND|wxALL,10);


        change=new wxButton(this,wxID_ANY,_("Change grid"));
        change->Enable(false);
        all_box->Add(change,0,wxEXPAND|wxALL,10);
        change->Bind(wxEVT_BUTTON,[&](wxCommandEvent &e){
			int n=key_list->GetFirstSelected();
			assert(n>=0 && n<9);
			InputGridDialog input(this);
			if(input.ShowModal()==wxID_OK){
				double grid=input.Get();
				key_list->SetItem(n,1,get_grid_str(grid));
				grids[n]=grid;
			}

    	});
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

void GridBinderDialog::Get(double g[9]){
	memcpy(g,grids,sizeof(grids));
}
void ShowGridBinderDialog(wxWindow *parent,double grids[9]){
	GridBinderDialog dialog(parent,grids);
	if(dialog.ShowModal()==wxID_OK)
		dialog.Get(grids);
}

