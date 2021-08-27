#pragma once
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include "PCBFile.h"
#include "Settings.h"

class LeftPanel : public wxScrolledWindow{
public:
	LeftPanel(wxWindow *parent);
private:
    wxSpinCtrlDouble *w_smd_w;
    wxSpinCtrlDouble *w_smd_h;
    wxSpinCtrlDouble *w_pad_outer;
    wxSpinCtrlDouble *w_pad_inner;
    wxSpinCtrlDouble *w_track_size;
    wxButton *grid_button;

    void build_smd_menu();
    void build_pad_menu();
    void build_track_menu();
    void build_grid_menu();
    void set_smd_size(Vec2 size);
    void set_pad_size(Vec2 size);
    void set_track_size(float size);
    void set_grid_size(double grid);
    void swap_smd_size(wxCommandEvent&);
    void set_grid(float val);
};
