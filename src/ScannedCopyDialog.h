#pragma once
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>

#include "PCBFile.h"
#include "Settings.h"

class ScannedCopyDialog: public wxDialog {
public:
    ScannedCopyDialog(wxWindow* parent,const ImageConfig images[2],Settings &s);

    void Get(ImageConfig images[2]);
private:
	wxSpinCtrl *dpi[2];
	wxSpinCtrl *dx[2];
	wxSpinCtrl *dy[2];
	wxCheckBox *show[2];
	wxFilePickerCtrl *path[2];
};
