#ifndef GRID_BINDER_H
#define GRID_BINDER_H

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/listctrl.h>

class GridBinderDialog : public wxDialog {
public:
    GridBinderDialog(wxWindow* parent,const double keys[9]);
    void Get(double grids[9]);
private:
    double grids[9];
    wxButton *change;
    wxListView *key_list;
};
void ShowGridBinderDialog(wxWindow *parent,double keys[9]);
#endif
