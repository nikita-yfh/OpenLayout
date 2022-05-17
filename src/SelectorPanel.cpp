#include "SelectorPanel.h"
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

enum {
	ID_ELEMENTS = 300,
	ID_SORTING,
	ID_LAYERS
};

wxBEGIN_EVENT_TABLE(SelectorPanel, wxPanel)
	EVT_CHOICE(ID_ELEMENTS, SelectorPanel::UpdateSortingList)
wxEND_EVENT_TABLE()

static const wxString elementsStr[] = {
	_("All pads"),
	_("Vias"),
	_("SMD-Pads"),
	_("Tracks"),
	_("Text labels")
};

static const wxString sortingStr[] = {
	_("Drillings"),
	_("Angular ring size"),
	_("Size"),
	_("Form"),
	_("Width"),
	_("Height"),
	_("Thickness"),
	_("String")
};

static const wxString layersStr[] = {
	_("Only copper"),
	_("Only silkscreen / outline"),
	_("All layer"),
};

SelectorPanel::SelectorPanel(wxWindow *parent, PCB &_pcb)
		: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN), pcb(_pcb) {

	wxChoice *elements = new wxChoice(this, ID_ELEMENTS, wxDefaultPosition, wxDefaultSize, 5, elementsStr);
	elements->SetSelection(0);

	sorting = new wxChoice(this, ID_SORTING, wxDefaultPosition, wxDefaultSize, 4, sortingStr);
	sorting->SetSelection(0);

	wxChoice *layers = new wxChoice(this, ID_LAYERS, wxDefaultPosition, wxDefaultSize, 3, layersStr);
	layers->SetSelection(0);

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	content->Add(new wxStaticText(this, wxID_ANY, _("Elements:")),	0, wxALL, 5);
	content->Add(elements, 0, wxEXPAND | wxALL, 5);
	content->Add(new wxStaticText(this, wxID_ANY, _("Sorting:")),	0, wxALL, 5);
	content->Add(sorting, 0, wxEXPAND | wxALL, 5);
	content->Add(new wxStaticText(this, wxID_ANY, _("Layer:")),		0, wxALL, 5);
	content->Add(layers, 0, wxEXPAND | wxALL, 5);

	SetSizer(content);
}

void SelectorPanel::UpdateSortingList(wxCommandEvent &e) {
	switch(e.GetInt()) {
	case 0: case 1:
		sorting->Set(4, sortingStr);
		break;
	case 2:
		sorting->Set(1, sortingStr + 2);
		break;
	case 3:
		sorting->Set(1, sortingStr + 4);
		break;
	case 4:
		sorting->Set(3, sortingStr + 5);
		break;
	}
	sorting->SetSelection(0);
}
