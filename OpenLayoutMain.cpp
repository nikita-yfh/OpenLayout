/***************************************************************
 * Name:      OpenLayoutMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:
 **************************************************************/

#include "OpenLayoutMain.h"
#include "NewBoardDialog.h"
#include "LayerInfoDialog.h"
#include "ProjectInfoDialog.h"
#include "SettingsDialog.h"
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>

#define MAIN_XPM
#include "images.h"
#undef MAIN_XPM

#include <string>
#include <sstream>
using namespace std;
template<typename T>
static string to_str(T value) {
    stringstream s;
    s<<value;
    return s.str();
}
const long OpenLayoutFrame::ID_RADIOBOX1 = wxNewId();
const long OpenLayoutFrame::ID_BITMAPBUTTON4 = wxNewId();
const long OpenLayoutFrame::ID_BUTTON1 = wxNewId();
const long OpenLayoutFrame::ID_BITMAPBUTTON1 = wxNewId();
const long OpenLayoutFrame::ID_SPINCTRLDOUBLE1 = wxNewId();
const long OpenLayoutFrame::ID_BITMAPBUTTON2 = wxNewId();
const long OpenLayoutFrame::ID_SPINCTRLDOUBLE3 = wxNewId();
const long OpenLayoutFrame::ID_SPINCTRLDOUBLE2 = wxNewId();
const long OpenLayoutFrame::ID_BITMAPBUTTON3 = wxNewId();
const long OpenLayoutFrame::ID_SPINCTRLDOUBLE4 = wxNewId();
const long OpenLayoutFrame::ID_SPINCTRLDOUBLE5 = wxNewId();
const long OpenLayoutFrame::ID_PANEL1 = wxNewId();
const long OpenLayoutFrame::ID_CHOICE1 = wxNewId();
const long OpenLayoutFrame::ID_PANEL2 = wxNewId();
const long OpenLayoutFrame::ID_SCROLLEDWINDOW1 = wxNewId();
const long OpenLayoutFrame::ID_STATICTEXT1 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON1 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON2 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON3 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON4 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON5 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON6 = wxNewId();
const long OpenLayoutFrame::ID_TOGGLEBUTTON7 = wxNewId();
const long OpenLayoutFrame::ID_STATICTEXT2 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON1 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON2 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON3 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON4 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON5 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON6 = wxNewId();
const long OpenLayoutFrame::ID_RADIOBUTTON7 = wxNewId();
const long OpenLayoutFrame::ID_PANEL4 = wxNewId();
const long OpenLayoutFrame::ID_BUTTON2 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM1 = wxNewId();
const long OpenLayoutFrame::OpenFile = wxNewId();
const long OpenLayoutFrame::SaveFile = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM2 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM3 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM4 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM5 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM6 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM7 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM8 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM9 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM10 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM11 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM12 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM13 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM14 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM15 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM16 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM17 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM18 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM19 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBARITEM20 = wxNewId();
const long OpenLayoutFrame::ID_TOOLBAR1 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM70 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM71 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM72 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM73 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM74 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM75 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM76 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM77 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM79 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM80 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM81 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM82 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM83 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM84 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM85 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM86 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM87 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM88 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM89 = wxNewId();
const long OpenLayoutFrame::ID_MENUITEM78 = wxNewId();

OpenLayoutFrame::OpenLayoutFrame(wxWindow* parent,wxWindowID id) {
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer9;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;

    Create(parent, wxID_ANY, _("OpenLayout"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(600,400));
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxVSCROLL|wxHSCROLL, _T("ID_PANEL1"));
    FlexGridSizer2 = new wxFlexGridSizer(7, 1, 0, 0);
    wxString __wxRadioBoxChoices_1[16] = {
        _("Edit"),
        _("Zoom"),
        _("Track"),
        _("Pad"),
        _("SMD-Pad"),
        _("Circle"),
        _("Rectangle"),
        _("Zone"),
        _("Special form"),
        _("Text"),
        _("Solder mask"),
        _("Connections"),
        _("Autoroute"),
        _("Test"),
        _("Measure"),
        _("Photoview")
    };
    RadioBox1 = new wxRadioBox(Panel1, ID_RADIOBOX1, _("Tools"), wxDefaultPosition, wxDefaultSize, 16, __wxRadioBoxChoices_1, 1, wxRA_HORIZONTAL, wxDefaultValidator, _T("ID_RADIOBOX1"));
    RadioBox1->SetSelection(0);
    FlexGridSizer2->Add(RadioBox1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
    GridSizeB = new wxBitmapButton(Panel1, ID_BITMAPBUTTON4, grid_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON4"));
    FlexGridSizer4->Add(GridSizeB, 1, wxALL|wxEXPAND, 0);
    GridSizeV = new wxButton(Panel1, ID_BUTTON1, _("Label"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer4->Add(GridSizeV, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer3 = new wxFlexGridSizer(3, 2, 0, 0);
    TrackSizeB = new wxBitmapButton(Panel1, ID_BITMAPBUTTON1, track_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
    FlexGridSizer3->Add(TrackSizeB, 1, wxALL|wxEXPAND, 0);
    TrackSizeV = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE1, _T("0"), wxDefaultPosition, wxSize(80,30), 0, 0, 99.99, 0, 0.05, _T("ID_SPINCTRLDOUBLE1"));
    TrackSizeV->SetValue(_T("0"));
    FlexGridSizer3->Add(TrackSizeV, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
    PadSizeB = new wxBitmapButton(Panel1, ID_BITMAPBUTTON2, pad_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
    FlexGridSizer3->Add(PadSizeB, 1, wxALL|wxEXPAND, 0);
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    PadSize1 = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE3, _T("0"), wxDefaultPosition, wxSize(80,30), 0, 0, 99.99, 0, 0.05, _T("ID_SPINCTRLDOUBLE3"));
    PadSize1->SetValue(_T("0"));
    BoxSizer1->Add(PadSize1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    PadSize2 = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE2, _T("0"), wxDefaultPosition, wxSize(80,30), 0, 0, 99.99, 0, 0.05, _T("ID_SPINCTRLDOUBLE2"));
    PadSize2->SetValue(_T("0"));
    BoxSizer1->Add(PadSize2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer3->Add(BoxSizer1, 1, wxALL|wxEXPAND, 0);
    PadSMDV = new wxBitmapButton(Panel1, ID_BITMAPBUTTON3, pad_smd_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
    FlexGridSizer3->Add(PadSMDV, 1, wxALL|wxEXPAND, 0);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    PadSMDW = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE4, _T("0"), wxDefaultPosition, wxSize(80,30), 0, 0, 99.99, 0, 0.05, _T("ID_SPINCTRLDOUBLE4"));
    PadSMDW->SetValue(_T("0"));
    BoxSizer2->Add(PadSMDW, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    PadSMDH = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE5, _T("0"), wxDefaultPosition, wxSize(80,30), 0, 0, 99.99, 0, 0.05, _T("ID_SPINCTRLDOUBLE5"));
    PadSMDH->SetValue(_T("0"));
    BoxSizer2->Add(PadSMDH, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    FlexGridSizer3->Add(BoxSizer2, 1, wxALL|wxEXPAND, 0);
    FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 0);
    Panel1->SetSizer(FlexGridSizer2);
    FlexGridSizer2->Fit(Panel1);
    FlexGridSizer2->SetSizeHints(Panel1);
    BoxSizer6->Add(Panel1, 0, wxALL, 5);
    BoxSizer3 = new wxBoxSizer(wxVERTICAL);
    BoardChoice = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    BoardChoice->SetSelection( BoardChoice->Append(_("New board")) );
    BoxSizer3->Add(BoardChoice, 0, wxALL|wxEXPAND, 0);
    Scroll = new wxScrolledWindow(this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxHSCROLL|wxFULL_REPAINT_ON_RESIZE, _T("ID_SCROLLEDWINDOW1"));
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    Canvas = new wxPanel(Scroll, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer4->Add(Canvas, 1, wxALL|wxEXPAND, 5);
    Scroll->SetSizer(BoxSizer4);
    BoxSizer4->Fit(Scroll);
    BoxSizer4->SetSizeHints(Scroll);
    BoxSizer3->Add(Scroll, 1, wxALL|wxEXPAND, 5);
    BoxSizer6->Add(BoxSizer3, 1, wxALL|wxEXPAND, 5);
    BoxSizer5->Add(BoxSizer6, 1, wxALIGN_LEFT, 0);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    FlexGridSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("visible"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer1->Add(StaticText1, 1, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    ToggleButton1 = new wxToggleButton(this, ID_TOGGLEBUTTON1, _("C1"), wxDefaultPosition, wxSize(30,30), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON1"));
    ToggleButton1->SetValue(true);
    BoxSizer8->Add(ToggleButton1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    ToggleButton2 = new wxToggleButton(this, ID_TOGGLEBUTTON2, _("S1"), wxDefaultPosition, wxSize(0,0), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON2"));
    ToggleButton2->SetValue(true);
    BoxSizer8->Add(ToggleButton2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    ToggleButton3 = new wxToggleButton(this, ID_TOGGLEBUTTON3, _("I1"), wxDefaultPosition, wxSize(0,0), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON3"));
    ToggleButton3->SetValue(true);
    BoxSizer8->Add(ToggleButton3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    ToggleButton4 = new wxToggleButton(this, ID_TOGGLEBUTTON4, _("I2"), wxDefaultPosition, wxSize(0,0), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON4"));
    ToggleButton4->SetValue(true);
    BoxSizer8->Add(ToggleButton4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    ToggleButton5 = new wxToggleButton(this, ID_TOGGLEBUTTON5, _("C2"), wxDefaultPosition, wxSize(0,0), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON5"));
    ToggleButton5->SetValue(true);
    BoxSizer8->Add(ToggleButton5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    ToggleButton6 = new wxToggleButton(this, ID_TOGGLEBUTTON6, _("S2"), wxDefaultPosition, wxSize(0,0), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON6"));
    ToggleButton6->SetValue(true);
    BoxSizer8->Add(ToggleButton6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    ToggleButton7 = new wxToggleButton(this, ID_TOGGLEBUTTON7, _("O"), wxDefaultPosition, wxSize(0,0), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON7"));
    ToggleButton7->SetValue(true);
    BoxSizer8->Add(ToggleButton7, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    FlexGridSizer1->Add(BoxSizer8, 0, wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("active"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer1->Add(StaticText2, 1, wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel3 = new wxPanel(this, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    RadioButton1 = new wxRadioButton(Panel3, ID_RADIOBUTTON1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
    BoxSizer9->Add(RadioButton1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    RadioButton2 = new wxRadioButton(Panel3, ID_RADIOBUTTON2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
    BoxSizer9->Add(RadioButton2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    RadioButton3 = new wxRadioButton(Panel3, ID_RADIOBUTTON3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
    BoxSizer9->Add(RadioButton3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    RadioButton4 = new wxRadioButton(Panel3, ID_RADIOBUTTON4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
    BoxSizer9->Add(RadioButton4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    RadioButton5 = new wxRadioButton(Panel3, ID_RADIOBUTTON5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
    BoxSizer9->Add(RadioButton5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    RadioButton6 = new wxRadioButton(Panel3, ID_RADIOBUTTON6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
    BoxSizer9->Add(RadioButton6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    RadioButton7 = new wxRadioButton(Panel3, ID_RADIOBUTTON7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON7"));
    BoxSizer9->Add(RadioButton7, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxSHAPED, 5);
    Panel3->SetSizer(BoxSizer9);
    BoxSizer9->Fit(Panel3);
    BoxSizer9->SetSizeHints(Panel3);
    FlexGridSizer1->Add(Panel3, 1, wxEXPAND, 5);
    BoxSizer7->Add(FlexGridSizer1, 1, wxALL|wxALIGN_TOP, 5);
    LayerHelpButton = new wxButton(this, ID_BUTTON2, _("\?"), wxDefaultPosition, wxSize(20,-1), wxBU_EXACTFIT, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer7->Add(LayerHelpButton, 0, wxEXPAND, 5);
    BoxSizer5->Add(BoxSizer7, 0, wxEXPAND, 5);
    SetSizer(BoxSizer5);

    ToolBar = new wxToolBar(this, ID_TOOLBAR1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL, _T("ID_TOOLBAR1"));
    ToolBarItem1 = ToolBar->AddTool(ID_TOOLBARITEM1, _("New file"), new_xpm, wxNullBitmap, wxITEM_NORMAL, _("New file"), wxEmptyString);
    ToolBarItem2 = ToolBar->AddTool(OpenFile, _("Open file"), open_xpm, wxNullBitmap, wxITEM_NORMAL, _("Open file"), wxEmptyString);
    ToolBarItem3 = ToolBar->AddTool(SaveFile, _("Save file"), save_xpm, wxNullBitmap, wxITEM_NORMAL, _("Save file"), wxEmptyString);
    ToolBarItem4 = ToolBar->AddTool(ID_TOOLBARITEM2, _("Print"), print_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem5 = ToolBar->AddTool(ID_TOOLBARITEM3, _("Undo"), undo_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem6 = ToolBar->AddTool(ID_TOOLBARITEM4, _("Redo"), redo_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem7 = ToolBar->AddTool(ID_TOOLBARITEM5, _("Cut"), cut_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem8 = ToolBar->AddTool(ID_TOOLBARITEM6, _("Copy"), copy_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem9 = ToolBar->AddTool(ID_TOOLBARITEM7, _("Paste"), paste_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem10 = ToolBar->AddTool(ID_TOOLBARITEM8, _("Delete"), delete_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem11 = ToolBar->AddTool(ID_TOOLBARITEM9, _("Duplicate"), duplicate_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem12 = ToolBar->AddTool(ID_TOOLBARITEM10, _("Rotate"), rotate_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem13 = ToolBar->AddTool(ID_TOOLBARITEM11, _("Mirror horizontal"), mirror_h_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem14 = ToolBar->AddTool(ID_TOOLBARITEM12, _("Mirror vertical"), mirror_v_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem15 = ToolBar->AddTool(ID_TOOLBARITEM13, _("Align elements"), align_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem16 = ToolBar->AddTool(ID_TOOLBARITEM14, _("Snap to grid"), to_grid_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem17 = ToolBar->AddTool(ID_TOOLBARITEM15, _("Build group"), group_on_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBarItem18 = ToolBar->AddTool(ID_TOOLBARITEM16, _("Split group"), group_off_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem19 = ToolBar->AddTool(ID_TOOLBARITEM17, _("Zoom"), zoom_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem20 = ToolBar->AddTool(ID_TOOLBARITEM18, _("Transparent"), transparent_xpm, wxNullBitmap, wxITEM_NORMAL, _("Toggle transparent mode"), wxEmptyString);
    ToolBarItem21 = ToolBar->AddTool(ID_TOOLBARITEM19, _("Info"), info_xpm, wxNullBitmap, wxITEM_NORMAL, _("Show project info"), wxEmptyString);
    ToolBar->AddSeparator();
    ToolBarItem22 = ToolBar->AddTool(ID_TOOLBARITEM20, _("Scanned copy"), bitmap_xpm, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
    ToolBar->Realize();
    SetToolBar(ToolBar);
    MenuItem70 = new wxMenuItem((&GridMenu), ID_MENUITEM70, _("39.6875 um"));
    GridMenu.Append(MenuItem70);
    MenuItem71 = new wxMenuItem((&GridMenu), ID_MENUITEM71, _("79.375 um"));
    GridMenu.Append(MenuItem71);
    MenuItem72 = new wxMenuItem((&GridMenu), ID_MENUITEM72, _("158.75 um"));
    GridMenu.Append(MenuItem72);
    MenuItem73 = new wxMenuItem((&GridMenu), ID_MENUITEM73, _("0.3175 mm"));
    GridMenu.Append(MenuItem73);
    MenuItem74 = new wxMenuItem((&GridMenu), ID_MENUITEM74, _("0.635 mm"));
    GridMenu.Append(MenuItem74);
    MenuItem75 = new wxMenuItem((&GridMenu), ID_MENUITEM75, _("1.27 mm"));
    GridMenu.Append(MenuItem75);
    MenuItem76 = new wxMenuItem((&GridMenu), ID_MENUITEM76, _("2.54 mm"));
    GridMenu.Append(MenuItem76);
    MenuItem77 = new wxMenuItem((&GridMenu), ID_MENUITEM77, _("5.08 mm"));
    GridMenu.Append(MenuItem77);
    MenuItem78 = new wxMenu();
    MenuItem79 = new wxMenuItem(MenuItem78, ID_MENUITEM79, _("0.01 mm"));
    MenuItem78->Append(MenuItem79);
    MenuItem80 = new wxMenuItem(MenuItem78, ID_MENUITEM80, _("0.02 mm"));
    MenuItem78->Append(MenuItem80);
    MenuItem81 = new wxMenuItem(MenuItem78, ID_MENUITEM81, _("0.025 mm"));
    MenuItem78->Append(MenuItem81);
    MenuItem82 = new wxMenuItem(MenuItem78, ID_MENUITEM82, _("0.05 mm"));
    MenuItem78->Append(MenuItem82);
    MenuItem83 = new wxMenuItem(MenuItem78, ID_MENUITEM83, _("0.1 mm"));
    MenuItem78->Append(MenuItem83);
    MenuItem84 = new wxMenuItem(MenuItem78, ID_MENUITEM84, _("0.2 mm"));
    MenuItem78->Append(MenuItem84);
    MenuItem85 = new wxMenuItem(MenuItem78, ID_MENUITEM85, _("0.25 mm"));
    MenuItem78->Append(MenuItem85);
    MenuItem86 = new wxMenuItem(MenuItem78, ID_MENUITEM86, _("0.5 mm"));
    MenuItem78->Append(MenuItem86);
    MenuItem87 = new wxMenuItem(MenuItem78, ID_MENUITEM87, _("1 mm"));
    MenuItem78->Append(MenuItem87);
    MenuItem88 = new wxMenuItem(MenuItem78, ID_MENUITEM88, _("2 mm"));
    MenuItem78->Append(MenuItem88);
    MenuItem89 = new wxMenuItem(MenuItem78, ID_MENUITEM89, _("2.5 mm"));
    MenuItem78->Append(MenuItem89);
    GridMenu.Append(ID_MENUITEM78, _("Metric grids"), MenuItem78, wxEmptyString);
    SetSizer(BoxSizer5);
    Layout();

    Connect(ID_BITMAPBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSizeClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSizeClick);
    Connect(ID_SPINCTRLDOUBLE1,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&OpenLayoutFrame::OnTrackSizeVChange);
    Connect(ID_SPINCTRLDOUBLE3,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&OpenLayoutFrame::OnPadSize1Change);
    Connect(ID_SPINCTRLDOUBLE2,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&OpenLayoutFrame::OnPadSize2Change);
    Connect(ID_SPINCTRLDOUBLE4,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&OpenLayoutFrame::OnPadSMDWChange);
    Connect(ID_SPINCTRLDOUBLE5,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&OpenLayoutFrame::OnPadSMDHChange);
    Canvas->Connect(wxEVT_PAINT,(wxObjectEventFunction)&OpenLayoutFrame::OnPanel2Paint,0,this);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OpenLayoutFrame::OnLayerHelpButtonClick);
    Connect(ID_TOOLBARITEM19,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&OpenLayoutFrame::OnProjectInfoButtonClick);
    Connect(ID_MENUITEM70,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize1Selected);
    Connect(ID_MENUITEM71,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize2Selected);
    Connect(ID_MENUITEM72,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize3Selected);
    Connect(ID_MENUITEM73,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize4Selected);
    Connect(ID_MENUITEM74,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize5Selected);
    Connect(ID_MENUITEM75,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize6Selected);
    Connect(ID_MENUITEM76,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize7Selected);
    Connect(ID_MENUITEM77,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize8Selected);
    Connect(ID_MENUITEM79,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize9Selected);
    Connect(ID_MENUITEM80,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize10Selected);
    Connect(ID_MENUITEM81,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize11Selected);
    Connect(ID_MENUITEM82,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize12Selected);
    Connect(ID_MENUITEM84,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize14Selected);
    Connect(ID_MENUITEM85,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize15Selected);
    Connect(ID_MENUITEM86,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize16Selected);
    Connect(ID_MENUITEM87,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize17Selected);
    Connect(ID_MENUITEM88,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize18Selected);
    Connect(ID_MENUITEM89,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenLayoutFrame::OnGridSize19Selected);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&OpenLayoutFrame::OnClose);
    SetDefaults();
    init_menu_bar();
}

OpenLayoutFrame::~OpenLayoutFrame() {}

void OpenLayoutFrame::OnQuit(wxCommandEvent& event) {
    Close();
}
void OpenLayoutFrame::OnClose(wxCloseEvent& event) {
    Destroy();
}

void OpenLayoutFrame::SetDefaults() {
    SetGridSize(1.27f);
    SetPadSize1(1.8f);
    SetPadSize2(0.6);
    SetPadSMDW(0.9f);
    SetPadSMDH(1.8f);

    file.SetDefaults();
}
void OpenLayoutFrame::SwapSMDSize() {
    swap(pad_smd_w,pad_smd_h);
}


void OpenLayoutFrame::OnTrackSizeVChange(wxSpinDoubleEvent& event) {
    track_size=static_cast<wxSpinCtrlDouble*>(event.GetEventObject())->GetValue();
}
void OpenLayoutFrame::OnPadSize1Change(wxSpinDoubleEvent& event) {
    pad_r1=static_cast<wxSpinCtrlDouble*>(event.GetEventObject())->GetValue();
}
void OpenLayoutFrame::OnPadSize2Change(wxSpinDoubleEvent& event) {
    pad_r2=static_cast<wxSpinCtrlDouble*>(event.GetEventObject())->GetValue();
}
void OpenLayoutFrame::OnPadSMDWChange(wxSpinDoubleEvent& event) {
    pad_smd_w=static_cast<wxSpinCtrlDouble*>(event.GetEventObject())->GetValue();
}
void OpenLayoutFrame::OnPadSMDHChange(wxSpinDoubleEvent& event) {
    pad_smd_h=static_cast<wxSpinCtrlDouble*>(event.GetEventObject())->GetValue();
}

void OpenLayoutFrame::SetGridSize(float value,bool micro) {
    file.GetSelectedBoard().grid_size=value*(micro?0.001f:1.0f);
    string label=to_str(value);
    if(micro)label+=" um";
    else label+=" mm";
    GridSizeV->SetLabel(label);
}

void OpenLayoutFrame::SetTrackSize(float value,bool set) {
    if(set)track_size=value;
    TrackSizeV->SetValue(value);
}

void OpenLayoutFrame::SetPadSize1(float value,bool set) {
    if(set)pad_r1=value;
    PadSize1->SetValue(value);
}

void OpenLayoutFrame::SetPadSize2(float value,bool set) {
    if(set)pad_r2=value;
    PadSize2->SetValue(value);
}

void OpenLayoutFrame::SetPadSMDW(float value,bool set) {
    if(set)pad_smd_w=value;
    PadSMDW->SetValue(value);
}

void OpenLayoutFrame::SetPadSMDH(float value,bool set) {
    if(set)pad_smd_h=value;
    PadSMDH->SetValue(value);
}

void OpenLayoutFrame::OnGridSizeClick(wxCommandEvent& event) {
    PopupMenu(&GridMenu);
}

void OpenLayoutFrame::OnGridSize1Selected(wxCommandEvent& event) {
    SetGridSize(39.6875,true);
}
void OpenLayoutFrame::OnGridSize2Selected(wxCommandEvent& event) {
    SetGridSize(79.375, true);
}
void OpenLayoutFrame::OnGridSize3Selected(wxCommandEvent& event) {
    SetGridSize(158.75, true);
}
void OpenLayoutFrame::OnGridSize4Selected(wxCommandEvent& event) {
    SetGridSize(0.3175,false);
}
void OpenLayoutFrame::OnGridSize5Selected(wxCommandEvent& event) {
    SetGridSize(0.635, false);
}
void OpenLayoutFrame::OnGridSize6Selected(wxCommandEvent& event) {
    SetGridSize(1.27,  false);
}
void OpenLayoutFrame::OnGridSize7Selected(wxCommandEvent& event) {
    SetGridSize(2.54,  false);
}
void OpenLayoutFrame::OnGridSize8Selected(wxCommandEvent& event) {
    SetGridSize(5.08,  false);
}
void OpenLayoutFrame::OnGridSize9Selected(wxCommandEvent& event) {
    SetGridSize(0.01,  false);
}
void OpenLayoutFrame::OnGridSize10Selected(wxCommandEvent& event) {
    SetGridSize(0.02, false);
}
void OpenLayoutFrame::OnGridSize11Selected(wxCommandEvent& event) {
    SetGridSize(0.25, false);
}
void OpenLayoutFrame::OnGridSize12Selected(wxCommandEvent& event) {
    SetGridSize(0.5,  false);
}
void OpenLayoutFrame::OnGridSize13Selected(wxCommandEvent& event) {
    SetGridSize(0.1,  false);
}
void OpenLayoutFrame::OnGridSize14Selected(wxCommandEvent& event) {
    SetGridSize(0.2,  false);
}
void OpenLayoutFrame::OnGridSize15Selected(wxCommandEvent& event) {
    SetGridSize(0.25, false);
}
void OpenLayoutFrame::OnGridSize16Selected(wxCommandEvent& event) {
    SetGridSize(0.5,  false);
}
void OpenLayoutFrame::OnGridSize17Selected(wxCommandEvent& event) {
    SetGridSize(1,    false);
}
void OpenLayoutFrame::OnGridSize18Selected(wxCommandEvent& event) {
    SetGridSize(2,    false);
}
void OpenLayoutFrame::OnGridSize19Selected(wxCommandEvent& event) {
    SetGridSize(2.5,  false);
}

void OpenLayoutFrame::AddNewBoard(wxCommandEvent& event) {
    while(true) {
        NewBoardDialog dialog(this);
        if(dialog.ShowModal()==wxID_OK) {
            if(dialog.board.isValid()) {
                file.AddBoard(dialog.board);
                BoardChoice->Append(dialog.board.name);
                BoardChoice->SetSelection(file.GetNumBoards()-1);
                wxMessageDialog(nullptr,_("A new board added successfully"),_("Info"),wxOK|wxCENTRE|wxICON_INFORMATION).ShowModal();
                break;
            } else {
                wxMessageDialog(nullptr,_("The board is too big!"),_("Error"),wxOK|wxCENTRE|wxICON_ERROR).ShowModal();
            }
        } else break;
    }
}

void OpenLayoutFrame::OnPanel2Paint(wxPaintEvent& event) {
    wxPanel *panel=static_cast<wxPanel*>(event.GetEventObject());
    wxPaintDC dc(panel);
    dc.DrawLine(0,0,100,100);
    //Refresh();
    //Update();
}

void OpenLayoutFrame::OnBoardChoiceSelect(wxCommandEvent& event) {
    file.SelectBoard(static_cast<wxChoice*>(event.GetEventObject())->GetSelection());
}

void OpenLayoutFrame::OnLayerHelpButtonClick(wxCommandEvent& event) {
    LayerInfoDialog(this).ShowModal();
}


void OpenLayoutFrame::OnProjectInfoButtonClick(wxCommandEvent& event) {
    ProjectInfoDialog i(this);
    i.Set(file.info);
    if(i.ShowModal()==wxID_OK)
        i.Get(file.info);
}

void OpenLayoutFrame::OnSettingsButtonClick(wxCommandEvent& event) {
    SettingsDialog dialog(this,s);
    if(dialog.ShowModal()==wxID_OK)
        dialog.Get(s);
}
void OpenLayoutFrame::init_menu_bar() {
    wxMenuBar* menu_bar = new wxMenuBar();
    {
        //File
        wxMenu* menu= new wxMenu();
        menu->Append(wxID_NEW,_("New\tCtrl+N"));
        menu->Append(wxID_OPEN,_("Open\tCtrl+O"));
        menu->Append(wxID_SAVE,_("Save\tCtrl+S"));
        menu->Append(wxID_SAVEAS,_("Save as\tCtrl+Shift+S"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Save as macro"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("AutoSave"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Gerber-Import"));
        menu->AppendSeparator();
        {
            //File->Export
            wxMenu *submenu = new wxMenu();
            submenu->Append(wxID_ANY,_("Gerber Export"));
            submenu->Append(wxID_ANY,_("Drill data (Excellon)"));
            submenu->AppendSeparator();
            submenu->Append(wxID_ANY,_("Isolation milling (HPGL, *.plt)"));
            submenu->AppendSeparator();
            submenu->Append(wxID_ANY,_("Bitmap (*.bmp)"));
            submenu->Append(wxID_ANY,_("JPG (*.jpg)"));
            submenu->Append(wxID_ANY,_("GIF (*.gif)"));
            submenu->Append(wxID_ANY,_("EMP (*.emp)"));
            submenu->Append(wxID_ANY,_("PNG (*.png)"));
            menu->Append(wxID_ANY, _("Export"), submenu);
        }
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Directories"));
        menu->AppendSeparator();
        menu->Append(wxID_PRINT_SETUP,_("Printer setup"));
        menu->Append(wxID_PRINT,_("Print\tCtrl+P"));
        menu->AppendSeparator();
        menu->Append(wxID_EXIT,_("Exit\tCtrl+Q"));
        menu_bar->Append(menu, _("File"));
    }

    {
        //Edit
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_UNDO,_("Undo\tCtrl+Z"));
        menu->Append(wxID_REDO,_("Redo\tCtrl+Y"));
        menu->AppendSeparator();
        menu->Append(wxID_COPY,_("Copy\tCtrl+C"));
        menu->Append(wxID_CUT,_("Cut\tCtrl+X"));
        menu->Append(wxID_PASTE,_("Paste\tCtrl+V"));
        menu->Append(wxID_DUPLICATE,_("Duplicate\tCtrl+D"));
        menu->Append(wxID_DELETE,_("Delete\tDelete"));
        menu->AppendSeparator();
        menu->Append(wxID_SELECTALL,_("Select all\tCtrl+A"));
        menu_bar->Append(menu, _("Edit"));
    }
    {
        //Board
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_ANY,_("Add new board"));
        menu->Append(wxID_ANY,_("Properties"));
        menu->Append(wxID_ANY,_("Copy board"));
        menu->Append(wxID_ANY,_("Delete board"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Set board to right"));
        menu->Append(wxID_ANY,_("Set board to left"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Move board to right"));
        menu->Append(wxID_ANY,_("Move board to left"));
        menu->Append(wxID_ANY,_("Import boards from file"));
        menu->Append(wxID_ANY,_("Save boards to file"));
        menu_bar->Append(menu, _("Board"));
    }
    {
        //Functions
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_ANY,_("Rotate\tCtrl+R"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Mirror horisontal\tCtrl+H"));
        menu->Append(wxID_ANY,_("Mirror vertical\tCtrl+T"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Buld group\tCtrl+G"));
        menu->Append(wxID_ANY,_("Split group\tCtrl+U"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Change board side\tCtrl+W"));
        menu->Append(wxID_ANY,_("Set to layer"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Snap to grid"));
        menu->Append(wxID_ANY,_("Tile / Arrange circular"));
        menu_bar->Append(menu, _("Functions"));
    }
    {
        //Extras
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_INFO,_("Project info"));
        menu->Append(wxID_ANY,_("List drillings"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Scanned copy"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Footprint-Wizard"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Reset solder mask"));
        menu->Append(wxID_ANY,_("Remove connections (rubberbands)"));
        menu->Append(wxID_ANY,_("Delete elements outside the board"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Text-IO: Import elements"));
        menu->Append(wxID_ANY,_("Text-IO: Export elements"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Define plugin"));
        menu->Append(wxID_ANY,_("Run plugin"));
        menu_bar->Append(menu, _("Extras"));
    }
    {
        //Options
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_PROPERTIES,_("General settings"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Macro-Library"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("Properties-Panel"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("DRC-Panel"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("Components-Panel"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("Selector-Panel"));
        menu->AppendSeparator();
        menu->Append(wxID_ZOOM_FIT,_("Zoom board"));
        menu->Append(wxID_ANY,_("Zoom objects"));
        menu->Append(wxID_ANY,_("Zoom selection"));
        menu->Append(wxID_ANY,_("Zoom previous"));
        menu_bar->Append(menu, _("Options"));
    }
    {
        //Help
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_ABOUT,_("About"));
        menu_bar->Append(menu, _("Help"));
    }
    SetMenuBar(menu_bar);
}

