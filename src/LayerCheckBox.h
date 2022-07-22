#pragma once
#include <wx/panel.h>

class LayerCheckBox : public wxPanel {
public:
	LayerCheckBox() {}
	LayerCheckBox(wxWindow *parent, wxWindowID winid, const wxString &label,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
			long style = 0, const wxString& name = "LayerCheckBox") {
		Create(parent, winid, label, pos, size, style, name);
	}

	bool Create(wxWindow *parent, wxWindowID winid, const wxString &label,
			const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
			long style = 0, const wxString& name = "LayerCheckBox");

private:
	virtual wxSize DoGetBestSize() const override;

	void OnPaint(wxPaintEvent&);
	void OnLeftDown(wxMouseEvent&);

	bool checked;

	wxDECLARE_EVENT_TABLE();
};


