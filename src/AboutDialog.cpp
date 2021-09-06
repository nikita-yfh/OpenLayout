#include "AboutDialog.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>

AboutDialog::AboutDialog(wxWindow* parent) {
    Create(parent, wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
    all_box->Add(new wxStaticText(this,wxID_ANY,"OpenLayout by nikita-yfh"),0,wxEXPAND|wxALL,5);
    SetSizerAndFit(all_box);
}

