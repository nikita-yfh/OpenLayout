#include "LayerCheckBox.h"
#include <wx/dcclient.h>

wxBEGIN_EVENT_TABLE(LayerCheckBox, wxPanel)
	EVT_LEFT_DOWN(LayerCheckBox::OnLeftDown)
	EVT_PAINT(LayerCheckBox::OnPaint)
wxEND_EVENT_TABLE()

bool LayerCheckBox::Create(wxWindow *parent, wxWindowID winid, const wxString &label,
		const wxPoint &pos, const wxSize &size, long style, const wxString& name) {
	wxPanel::Create(parent, winid, pos, size, style, name);
	SetLabel(label);
	checked = false;
	SetBackgroundColour(wxColor(0, 0, 0, 255));
	return true;
}

wxSize LayerCheckBox::DoGetBestSize() const {
	return GetTextExtent("##");
}

void LayerCheckBox::OnLeftDown(wxMouseEvent &e) {
	if(IsEnabled()) {
		checked = !checked;
		Refresh();
	}
}

void LayerCheckBox::OnPaint(wxPaintEvent&) {
	wxPaintDC dc(this);
	dc.DrawLabel(GetLabel(), GetSize(), wxALIGN_CENTER);
	if(checked) {
		dc.SetPen(*wxWHITE_PEN);
		float delta = (GetSize().x - GetSize().y) / 2 + 1;
		dc.DrawLine(delta + 1, 1, GetSize().x - 1 - delta, GetSize().y - 1);
	}
}


