#pragma once
#include <vector>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/tglbtn.h>
#include <wx/toolbar.h>
#include "PCBFile.h"
#include "Settings.h"

class OpenLayoutFrame: public wxFrame {
public:

    OpenLayoutFrame();
private:

    void init_menu_bar();
    void init_tool_bar(wxBoxSizer*);
    void close(wxCommandEvent&);
    void show_settings(wxCommandEvent&);
    void show_about(wxCommandEvent&);
    void show_project_info(wxCommandEvent&);
    void new_board(wxCommandEvent&);
    void group(wxCommandEvent&);
    void ungroup(wxCommandEvent&);
    void set_sel_layer(wxCommandEvent&);
    void del(wxCommandEvent&);
    void show_scan_properties(wxCommandEvent&);

    void updateui_edit(wxUpdateUIEvent&);
    void updateui_group(wxUpdateUIEvent&);
    void updateui_ungroup(wxUpdateUIEvent&);
    void updateui_multilayer(wxUpdateUIEvent&);
    wxDECLARE_EVENT_TABLE();
};
