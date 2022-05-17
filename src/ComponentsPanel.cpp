#include "ComponentsPanel.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/button.h>

enum {
	ID_NUMBER = 200,
	ID_LAYER,
	ID_ROTATION,
	ID_PACKAGE,
	ID_COMMENT,
	ID_EXPORT,
};

wxBEGIN_EVENT_TABLE(ComponentsPanel, wxPanel)
	EVT_CHECKBOX(ID_NUMBER,		ComponentsPanel::ToggleColumn<0>)
	EVT_CHECKBOX(ID_LAYER,		ComponentsPanel::ToggleColumn<3>)
	EVT_CHECKBOX(ID_ROTATION,	ComponentsPanel::ToggleColumn<4>)
	EVT_CHECKBOX(ID_PACKAGE,	ComponentsPanel::ToggleColumn<5>)
	EVT_CHECKBOX(ID_COMMENT,	ComponentsPanel::ToggleColumn<6>)
wxEND_EVENT_TABLE()

ComponentsPanel::ComponentsPanel(wxWindow *parent, const PCB &_pcb)
		: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, -1), wxBORDER_SUNKEN), pcb(_pcb) {
	wxFlexGridSizer *content = new wxFlexGridSizer(8, 1, 3, 0);
	list = new wxListView(this, wxID_ANY);
	
	list->InsertColumn(0, _("No."),		wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	list->InsertColumn(1, _("ID"),		wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	list->InsertColumn(2, _("Value"),	wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	list->InsertColumn(3, _("Layer"),	wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	list->InsertColumn(4, _("Rot"),		wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	list->InsertColumn(5, _("Package"),	wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);
	list->InsertColumn(6, _("Comment"),	wxLIST_ALIGN_LEFT, wxLIST_AUTOSIZE_USEHEADER);

	content->Add(list, 1, wxEXPAND);
	content->Add(new wxStaticText(this, wxID_ANY, _("Show:")), 0, wxEXPAND | wxALL, 10);

	wxCheckBox *checkboxes[5];
	checkboxes[0] = new wxCheckBox(this, ID_NUMBER,		_("Number"));
	checkboxes[1] = new wxCheckBox(this, ID_LAYER,		_("Layer"));
	checkboxes[2] = new wxCheckBox(this, ID_ROTATION,	_("Rotation"));
	checkboxes[3] = new wxCheckBox(this, ID_PACKAGE,	_("Package"));
	checkboxes[4] = new wxCheckBox(this, ID_COMMENT,	_("Comment"));
	for(wxCheckBox *checkbox : checkboxes) {
		checkbox->SetValue(true);
		content->Add(checkbox, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	}

	content->Add(new wxButton(this, ID_EXPORT, _("Export...")), 0, wxALL, 10);
	SetSizer(content);
}
