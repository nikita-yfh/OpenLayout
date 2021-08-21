#pragma once
#include "PCBFile.h"

#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

enum class BoardType {
    Empty,
    Rectangle,
    Round
};

class NewBoardDialog: public wxDialog {
public:
    NewBoardDialog(wxWindow* parent);

    Board build();
    bool isValid();

private:

    wxRadioButton *types[3];
    wxPanel *panels[4];

    wxSpinCtrlDouble *width;
    wxSpinCtrlDouble *height;
    wxSpinCtrlDouble *diameter;
    wxSpinCtrlDouble *border;
    wxTextCtrl *input_name;
    wxStaticText *all_width;
    wxStaticText *all_height;

    void SetType(const BoardType &b);

    uint32_t size_w;
    uint32_t size_h;

    uint32_t border_size;

    char name[20];

    BoardType type;

    void RecalcSize();
    void _RecalcSize(wxCommandEvent &e);
};

