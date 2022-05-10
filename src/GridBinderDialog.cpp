#include "GridBinderDialog.h"
#include "InputGridDialog.h"
#include <wx/sizer.h>

enum {
	ID_KEY_LIST = 1,
	ID_CHANGE
};

wxBEGIN_EVENT_TABLE(GridBinderDialog, wxDialog)
	EVT_LIST_ITEM_SELECTED(ID_KEY_LIST,	GridBinderDialog::OnSelectedItem)
	EVT_LIST_ITEM_ACTIVATED(ID_KEY_LIST,GridBinderDialog::OnActivatedItem)
	EVT_BUTTON(ID_CHANGE,				GridBinderDialog::ChangeGrid)
wxEND_EVENT_TABLE()

GridBinderDialog::GridBinderDialog(wxWindow *parent, Settings *_settings)
		: wxDialog(parent, wxID_ANY, _("Grid values for keys 1..9")), settings(_settings) {
	memcpy(grids, settings->gridBind, sizeof(grids));
	keyList = new wxListView(this, ID_KEY_LIST, wxDefaultPosition,
			wxDefaultSize, wxLC_REPORT | wxBORDER_SIMPLE);

	keyList->AppendColumn(_("Key"), wxLIST_FORMAT_LEFT, 50);
	keyList->AppendColumn(_("Grid"), wxLIST_FORMAT_LEFT, 100);
	for(int i = 0; i < 9; i++) {
		keyList->InsertItem(i, char(i + 1));
		keyList->SetItem(i, 0, char(i + '1'));
		keyList->SetItem(i, 1, settings->GetGridStr(grids[i]));
	}

	change = new wxButton(this, ID_CHANGE, _("Change grid"));
	change->Enable(false);

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	content->Add(keyList, 1, wxEXPAND | wxALL, 5);
	content->Add(change, 0, wxEXPAND | wxALL, 5);
	content->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);
	SetSizerAndFit(content);
}

void GridBinderDialog::OnSelectedItem(wxListEvent&) {
	change->Enable(true);
}
void GridBinderDialog::OnActivatedItem(wxListEvent &e) {
	ChangeGrid(e);
}

void GridBinderDialog::ChangeGrid(wxCommandEvent&) {
	int n = keyList->GetFirstSelected();
	double grid = InputGridDialog::Show(this, settings, grids[n]);
	if(grid != 0.0) {
		keyList->SetItem(n, 1, settings->GetGridStr(grid));
		grids[n] = grid;
	}
}


void GridBinderDialog::Get() {
	memcpy(settings->gridBind, grids, sizeof(grids));
}

void GridBinderDialog::Show(wxWindow *parent, Settings *settings) {
	GridBinderDialog *dialog = new GridBinderDialog(parent, settings);
	if(dialog->ShowModal() != wxID_OK)
		return;
	dialog->Get();
}
