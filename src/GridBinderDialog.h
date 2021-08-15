#ifndef GRID_BINDER_H
#define GRID_BINDER_H

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/stattext.h>

class GridBinderDialog : public wxDialog {
public:
    GridBinderDialog(wxWindow* parent,const double keys[9]);
private:
    void Get(double keys[9]);
    double keys[9];
    wxButton *buttons[9];
    wxStaticText *text[9];

};

#endif
