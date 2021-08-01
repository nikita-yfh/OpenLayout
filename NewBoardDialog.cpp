#include "NewBoardDialog.h"
#include <sstream>
#include <vector>
#include <functional>
#define NEWBOARD_XPM
#include "images.h"
#undef NEWBOARD_XPM

#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>

#define CENTER wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL
#define LEFT wxALIGN_LEFT


template<typename T>
static string to_str(T value) {
    stringstream s;
    s<<value;
    return s.str();
}

NewBoardDialog::NewBoardDialog(wxWindow* parent) {
    Create(parent, wxID_ANY, _("New board"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
    wxFlexGridSizer *all_box = new wxFlexGridSizer(5, 1, 0, 0);
    {
        auto RecalcSize=[&](wxCommandEvent &e) {
            board.border=border->GetValue()*10000;
            if(board.type==BoardType::Round) {
                board.width=board.height=diameter->GetValue()*10000;
            } else {
                board.width=width->GetValue()*10000;
                board.height=height->GetValue()*10000;
            }
            all_width->SetLabel(to_str((board.width+board.border*2)/10000.0));
            all_height->SetLabel(to_str((board.height+board.border*2)/10000.0));
            Layout();
        };
        wxBoxSizer *content_box = new wxBoxSizer(wxHORIZONTAL);
        {
            wxFlexGridSizer *buttons=new wxFlexGridSizer(3, 2, 0, 0);
            wxString text[3]= {
                _("Empty working-space\nwithout PCB outline"),
                _("Working-space with\nrectangle PCB outline"),
                _("Working-space with\nround PCB outline")
            };
            const char**images[]= {
                board1_xpm,
                board2_xpm,
                board3_xpm
            };
            auto f1=[&](wxCommandEvent&) {
                SetType(BoardType::Empty);
            };
            auto f2=[&](wxCommandEvent&) {
                SetType(BoardType::Rectangle);
            };
            auto f3=[&](wxCommandEvent&) {
                SetType(BoardType::Round);
            };
            vector<function<void(wxCommandEvent&)>>func= {f1,f2,f3};
            for(int q=0; q<3; q++) {
                wxBitmapButton *bitmap=new wxBitmapButton(this, wxID_ANY, images[q]);
                bitmap->Bind(wxEVT_BUTTON,func[q]);
                buttons->Add(bitmap,1,wxALL|wxEXPAND);
                types[q]=new wxRadioButton(this,wxID_ANY,text[q]);
                types[q]->Bind(wxEVT_RADIOBUTTON,func[q]);
                buttons->Add(types[q],1,wxALL|wxEXPAND);

            }
            wxBoxSizer *all_panels=new wxBoxSizer(wxVERTICAL);
            {
                panels[0]=new wxPanel(this);
                wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
                wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);

                box->Add(new wxStaticText(panels[0],wxID_ANY, _("Width: ")), 0, CENTER, 5);
                width=new wxSpinCtrlDouble(panels[0],wxID_ANY,"160", wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05);
                width->Bind(wxEVT_SPINCTRLDOUBLE,RecalcSize);
                box->Add(width, 0, wxALL|wxEXPAND, 5);
                box->Add(new wxStaticText(panels[0],wxID_ANY, _("mm")), 0, CENTER, 5);
                box->Add(new wxStaticText(panels[0],wxID_ANY, _("Heigth: ")), 0, CENTER, 5);
                height=new wxSpinCtrlDouble(panels[0], wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05);
                height->Bind(wxEVT_SPINCTRLDOUBLE,RecalcSize);
                box->Add(height, 0, wxALL|wxEXPAND, 5);
                box->Add(new wxStaticText(panels[0],wxID_ANY, _("mm")), 0, CENTER, 5);

                all->Add(new wxStaticText(panels[0],wxID_ANY, _("Size of working-space:")), 0, LEFT, 5);
                all->Add(box,0,wxALL|wxEXPAND,0);
                panels[0]->SetSizerAndFit(all);
                all_panels->Add(panels[0],0,wxALL|wxEXPAND,0);
            }
            {
                panels[1]=new wxPanel(this);
                wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
                wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);

                box->Add(new wxStaticText(panels[1],wxID_ANY, _("Diameter: ")), 0, CENTER, 5);
                diameter=new wxSpinCtrlDouble(panels[1], wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, 0, 0, 500, 0, 0.05);
                diameter->Bind(wxEVT_SPINCTRLDOUBLE,RecalcSize);
                box->Add(diameter, 0, wxALL|wxEXPAND, 5);
                box->Add(new wxStaticText(panels[1],wxID_ANY, _("mm")), 0, CENTER, 5);

                all->Add(new wxStaticText(panels[1],wxID_ANY, _("Round board outline:")), 0, LEFT, 5);
                all->Add(box,0,wxALL|wxEXPAND,0);
                panels[1]->SetSizerAndFit(all);
                all_panels->Add(panels[1],0,wxALL|wxEXPAND,0);
            }
            {
                panels[2]=new wxPanel(this);
                wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
                wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);

                box->Add(new wxStaticText(panels[2],wxID_ANY, _("Distance: ")), 0, CENTER, 5);
                border=new wxSpinCtrlDouble(panels[2], wxID_ANY, "20", wxDefaultPosition, wxDefaultSize, 0, 0, 500, 0, 0.05);
                border->Bind(wxEVT_SPINCTRLDOUBLE,RecalcSize);
                box->Add(border, 0, wxALL|wxEXPAND, 5);
                box->Add(new wxStaticText(panels[2],wxID_ANY, _("mm")), 0, CENTER, 5);

                all->Add(new wxStaticText(panels[2],wxID_ANY, _("Distance from working-space:")), 0, LEFT, 5);
                all->Add(box,0,wxALL|wxEXPAND,0);
                panels[2]->SetSizerAndFit(all);
                all_panels->Add(panels[2],0,wxALL|wxEXPAND,0);
            }
            {
                panels[3]=new wxPanel(this);
                wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
                wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);

                box->Add(new wxStaticText(panels[3],wxID_ANY, _("Width:")), 0, CENTER, 5);
                all_width=new wxStaticText(panels[3],wxID_ANY, _("200"));
                box->Add(all_width, 0, CENTER, 5);
                box->Add(new wxStaticText(panels[3],wxID_ANY, _("mm")), 0, CENTER, 5);
                box->Add(new wxStaticText(panels[3],wxID_ANY, _("Height:")), 0, CENTER, 5);
                all_height=new wxStaticText(panels[3],wxID_ANY, _("140"));
                box->Add(all_height, 0, CENTER, 5);
                box->Add(new wxStaticText(panels[3],wxID_ANY, _("mm")), 0, CENTER, 5);

                all->Add(new wxStaticText(panels[3],wxID_ANY, _("Resulting working-space:")), 0, LEFT, 5);
                all->Add(box,0,wxALL|wxEXPAND,0);
                panels[3]->SetSizerAndFit(all);
                all_panels->Add(panels[3],0,wxALL|wxEXPAND,0);
            }

            all_panels->Add(-1,-1,1, wxALL|wxEXPAND, 5);
            input_name=new wxTextCtrl(this,wxID_ANY,"New board");
            input_name->Bind(wxEVT_TEXT,[&](wxCommandEvent &e) {
                strcpy(board.name,input_name->GetValue().c_str());
            });
            all_panels->Add(input_name,0,wxEXPAND,5);
            content_box->Add(buttons,1,wxALL|wxEXPAND,5);
            content_box->Add(all_panels,1,wxALL|wxEXPAND,5);
        }
        all_box->Add(content_box,1,wxALL|wxEXPAND,5);

    }


    wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
    buttons->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
    buttons->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
    buttons->Realize();
    all_box->Add(buttons, 0, wxALL|wxEXPAND, 5);
    SetSizerAndFit(all_box);

    SetType(BoardType::Empty);
}

void NewBoardDialog::SetType(const BoardType &type) {
    types[(int)type]->SetValue(true);
    board.type=type;
    switch(type) {
    case BoardType::Empty:
        panels[0]->Show();
        panels[1]->Hide();
        panels[2]->Hide();
        panels[3]->Hide();
        break;
    case BoardType::Rectangle:
        panels[0]->Show();
        panels[1]->Hide();
        panels[2]->Show();
        panels[3]->Show();
        break;
    case BoardType::Round:
        panels[0]->Hide();
        panels[1]->Show();
        panels[2]->Hide();
        panels[3]->Show();
        break;
    }
    Layout();
}
