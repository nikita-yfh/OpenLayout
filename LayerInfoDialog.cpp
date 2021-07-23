#include "LayerInfoDialog.h"
#include "BITMAP/layer_info.xpm"
#include "BITMAP/layer_info1.xpm"
#include "BITMAP/layer_info2.xpm"
#include "BITMAP/layer_info3.xpm"
#include "BITMAP/layer_info4.xpm"
//(*InternalHeaders(LayerInfoDialog)
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(LayerInfoDialog)
const long LayerInfoDialog::ID_STATICBITMAP1 = wxNewId();
const long LayerInfoDialog::ID_PANEL1 = wxNewId();
const long LayerInfoDialog::ID_STATICBITMAP2 = wxNewId();
const long LayerInfoDialog::ID_PANEL2 = wxNewId();
const long LayerInfoDialog::ID_STATICBITMAP3 = wxNewId();
const long LayerInfoDialog::ID_PANEL3 = wxNewId();
const long LayerInfoDialog::ID_STATICBITMAP4 = wxNewId();
const long LayerInfoDialog::ID_PANEL4 = wxNewId();
const long LayerInfoDialog::ID_STATICBITMAP5 = wxNewId();
const long LayerInfoDialog::ID_PANEL5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(LayerInfoDialog,wxDialog)
	//(*EventTable(LayerInfoDialog)
	//*)
END_EVENT_TABLE()

LayerInfoDialog::LayerInfoDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(LayerInfoDialog)
	wxBoxSizer* BoxSizer1;

	Create(parent, wxID_ANY, _("Layer info"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	StaticBitmap1 = new wxStaticBitmap(Panel1, ID_STATICBITMAP1, layer_info1_xpm, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP1"));
	BoxSizer1->Add(Panel1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel2 = new wxPanel(this, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	StaticBitmap2 = new wxStaticBitmap(Panel2, ID_STATICBITMAP2, layer_info2_xpm, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP2"));
	BoxSizer1->Add(Panel2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel3 = new wxPanel(this, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	StaticBitmap3 = new wxStaticBitmap(Panel3, ID_STATICBITMAP3, layer_info3_xpm, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP3"));
	BoxSizer1->Add(Panel3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel4 = new wxPanel(this, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	StaticBitmap4 = new wxStaticBitmap(Panel4, ID_STATICBITMAP4, layer_info4_xpm, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP4"));
	BoxSizer1->Add(Panel4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel5 = new wxPanel(this, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	StaticBitmap5 = new wxStaticBitmap(Panel5, ID_STATICBITMAP5, layer_info_xpm, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP5"));
	BoxSizer1->Add(Panel5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

LayerInfoDialog::~LayerInfoDialog()
{
	//(*Destroy(LayerInfoDialog)
	//*)
}

