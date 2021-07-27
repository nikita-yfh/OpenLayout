#include "LayerInfoDialog.h"

#define LAYER_INFO_XPM
#include "images.h"
#undef LAYER_INFO_XPM

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>

LayerInfoDialog::LayerInfoDialog(wxWindow* parent){
	Create(parent, wxID_ANY, _("Layer info"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
	const char ** images[5]={
		layer_info1_xpm,
		layer_info2_xpm,
		layer_info3_xpm,
		layer_info4_xpm,
		layer_info_xpm
	};
	for(int q=0;q<5;q++){
		wxPanel *panel = new wxPanel(this, wxID_ANY);
		new wxStaticBitmap(panel,wxID_ANY, images[q]);
		all_box->Add(panel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	}
	SetSizerAndFit(all_box);
}

