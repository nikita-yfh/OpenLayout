#include "SettingsDialog.h"

#include <wx/valgen.h>

//(*InternalHeaders(SettingsDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(SettingsDialog)
const long SettingsDialog::ID_STATICTEXT1 = wxNewId();
const long SettingsDialog::ID_CHOICE1 = wxNewId();
const long SettingsDialog::ID_STATICTEXT2 = wxNewId();
const long SettingsDialog::ID_CHOICE2 = wxNewId();
const long SettingsDialog::ID_CHECKBOX1 = wxNewId();
const long SettingsDialog::ID_CHECKBOX2 = wxNewId();
const long SettingsDialog::ID_CHECKBOX3 = wxNewId();
const long SettingsDialog::ID_CHECKBOX4 = wxNewId();
const long SettingsDialog::ID_CHECKBOX5 = wxNewId();
const long SettingsDialog::ID_CHECKBOX6 = wxNewId();
const long SettingsDialog::ID_CHECKBOX7 = wxNewId();
const long SettingsDialog::ID_CHECKBOX8 = wxNewId();
const long SettingsDialog::ID_CHECKBOX9 = wxNewId();
const long SettingsDialog::ID_CHECKBOX10 = wxNewId();
const long SettingsDialog::ID_CHECKBOX11 = wxNewId();
const long SettingsDialog::ID_PANEL1 = wxNewId();
const long SettingsDialog::ID_STATICTEXT3 = wxNewId();
const long SettingsDialog::ID_CHOICE3 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL1 = wxNewId();
const long SettingsDialog::ID_STATICTEXT4 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT5 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL3 = wxNewId();
const long SettingsDialog::ID_STATICTEXT6 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL4 = wxNewId();
const long SettingsDialog::ID_STATICTEXT7 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL5 = wxNewId();
const long SettingsDialog::ID_STATICTEXT8 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL6 = wxNewId();
const long SettingsDialog::ID_STATICTEXT9 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL7 = wxNewId();
const long SettingsDialog::ID_STATICTEXT10 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL8 = wxNewId();
const long SettingsDialog::ID_STATICTEXT11 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL9 = wxNewId();
const long SettingsDialog::ID_STATICTEXT12 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL10 = wxNewId();
const long SettingsDialog::ID_STATICTEXT13 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL11 = wxNewId();
const long SettingsDialog::ID_STATICTEXT15 = wxNewId();
const long SettingsDialog::ID_COLOURPICKERCTRL12 = wxNewId();
const long SettingsDialog::ID_STATICTEXT14 = wxNewId();
const long SettingsDialog::ID_BUTTON1 = wxNewId();
const long SettingsDialog::ID_PANEL_COLORS = wxNewId();
const long SettingsDialog::ID_PANEL2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT16 = wxNewId();
const long SettingsDialog::ID_DIRPICKERCTRL4 = wxNewId();
const long SettingsDialog::ID_STATICTEXT17 = wxNewId();
const long SettingsDialog::ID_DIRPICKERCTRL2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT18 = wxNewId();
const long SettingsDialog::ID_DIRPICKERCTRL3 = wxNewId();
const long SettingsDialog::ID_STATICTEXT19 = wxNewId();
const long SettingsDialog::ID_DIRPICKERCTRL5 = wxNewId();
const long SettingsDialog::ID_STATICTEXT20 = wxNewId();
const long SettingsDialog::ID_DIRPICKERCTRL1 = wxNewId();
const long SettingsDialog::ID_PANEL_DIRS = wxNewId();
const long SettingsDialog::ID_STATICTEXT21 = wxNewId();
const long SettingsDialog::ID_CHECKBOX12 = wxNewId();
const long SettingsDialog::ID_PANEL3 = wxNewId();
const long SettingsDialog::ID_STATICTEXT22 = wxNewId();
const long SettingsDialog::ID_DIRPICKERCTRL6 = wxNewId();
const long SettingsDialog::ID_BUTTON2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT23 = wxNewId();
const long SettingsDialog::ID_BUTTON3 = wxNewId();
const long SettingsDialog::ID_STATICTEXT24 = wxNewId();
const long SettingsDialog::ID_PANEL4 = wxNewId();
const long SettingsDialog::ID_SPINCTRL1 = wxNewId();
const long SettingsDialog::ID_STATICTEXT25 = wxNewId();
const long SettingsDialog::ID_PANEL5 = wxNewId();
const long SettingsDialog::ID_STATICTEXT27 = wxNewId();
const long SettingsDialog::ID_SPINCTRL2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT28 = wxNewId();
const long SettingsDialog::ID_SPINCTRL3 = wxNewId();
const long SettingsDialog::ID_STATICTEXT26 = wxNewId();
const long SettingsDialog::ID_PANEL6 = wxNewId();
const long SettingsDialog::ID_LISTBOX1 = wxNewId();
const long SettingsDialog::ID_STATICTEXT29 = wxNewId();
const long SettingsDialog::ID_CHOICE4 = wxNewId();
const long SettingsDialog::ID_PANEL7 = wxNewId();
const long SettingsDialog::ID_CHECKBOX13 = wxNewId();
const long SettingsDialog::ID_CHECKBOX14 = wxNewId();
const long SettingsDialog::ID_CHECKBOX15 = wxNewId();
const long SettingsDialog::ID_CHECKBOX16 = wxNewId();
const long SettingsDialog::ID_RADIOBUTTON1 = wxNewId();
const long SettingsDialog::ID_RADIOBUTTON2 = wxNewId();
const long SettingsDialog::ID_STATICTEXT30 = wxNewId();
const long SettingsDialog::ID_STATICTEXT31 = wxNewId();
const long SettingsDialog::ID_PANEL10 = wxNewId();
const long SettingsDialog::ID_PANEL_CROSSHAIR = wxNewId();
const long SettingsDialog::ID_PANEL8 = wxNewId();
const long SettingsDialog::ID_STATICTEXT32 = wxNewId();
const long SettingsDialog::ID_CHECKBOX17 = wxNewId();
const long SettingsDialog::ID_STATICTEXT33 = wxNewId();
const long SettingsDialog::ID_SPINCTRL4 = wxNewId();
const long SettingsDialog::ID_STATICTEXT34 = wxNewId();
const long SettingsDialog::ID_PANEL_AUTOSAVE = wxNewId();
const long SettingsDialog::ID_PANEL9 = wxNewId();
const long SettingsDialog::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SettingsDialog,wxDialog)
	EVT_UPDATE_UI(ID_PANEL_COLORS, SettingsDialog::UpdateColorPanel)
	EVT_UPDATE_UI(ID_PANEL_DIRS, SettingsDialog::UpdateDirsPanel)
	EVT_UPDATE_UI(ID_PANEL_CROSSHAIR, SettingsDialog::UpdateCrosshairPanel)
	EVT_UPDATE_UI(ID_PANEL_AUTOSAVE, SettingsDialog::UpdateAutosavePanel)
    //(*EventTable(SettingsDialog)
    //*)
END_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size) {
    //(*Initialize(SettingsDialog)
    wxBoxSizer* BoxSizer10;
    wxBoxSizer* BoxSizer11;
    wxBoxSizer* BoxSizer12;
    wxBoxSizer* BoxSizer13;
    wxBoxSizer* BoxSizer14;
    wxBoxSizer* BoxSizer15;
    wxBoxSizer* BoxSizer16;
    wxBoxSizer* BoxSizer17;
    wxBoxSizer* BoxSizer18;
    wxBoxSizer* BoxSizer19;
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer20;
    wxBoxSizer* BoxSizer21;
    wxBoxSizer* BoxSizer22;
    wxBoxSizer* BoxSizer23;
    wxBoxSizer* BoxSizer24;
    wxBoxSizer* BoxSizer25;
    wxBoxSizer* BoxSizer26;
    wxBoxSizer* BoxSizer27;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer9;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer2;
    wxStdDialogButtonSizer* StdDialogButtonSizer1;

    Create(parent, wxID_ANY, _("General settings"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxNB_LEFT, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Base unit:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer3->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice1 = new wxChoice(Panel1, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice1->SetSelection( Choice1->Append(_("mm")) );
    Choice1->Append(_("mil (1/1000 Zoll)"));
    BoxSizer3->Add(Choice1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Drillings:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer3->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Choice2 = new wxChoice(Panel1, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    Choice2->SetSelection( Choice2->Append(_("Background color")) );
    Choice2->Append(_("White"));
    Choice2->Append(_("Black"));
    BoxSizer3->Add(Choice2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer3, 0, wxEXPAND, 5);
    CheckBox1 = new wxCheckBox(Panel1, ID_CHECKBOX1, _("Show Board-Zoom"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox1->SetValue(false);
    BoxSizer2->Add(CheckBox1, 0, wxEXPAND, 5);
    CheckBox2 = new wxCheckBox(Panel1, ID_CHECKBOX2, _("Darken ground-plane"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox2->SetValue(false);
    BoxSizer2->Add(CheckBox2, 0, wxEXPAND, 5);
    CheckBox3 = new wxCheckBox(Panel1, ID_CHECKBOX3, _("Show ground-plane of all layers simultaneos"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBox3->SetValue(false);
    BoxSizer2->Add(CheckBox3, 0, wxEXPAND, 5);
    CheckBox4 = new wxCheckBox(Panel1, ID_CHECKBOX4, _("Consider connections (rubberbands) in TEST mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBox4->SetValue(false);
    BoxSizer2->Add(CheckBox4, 0, wxEXPAND, 5);
    CheckBox5 = new wxCheckBox(Panel1, ID_CHECKBOX5, _("Blinking TEST mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBox5->SetValue(false);
    BoxSizer2->Add(CheckBox5, 0, wxALL|wxEXPAND, 0);
    CheckBox6 = new wxCheckBox(Panel1, ID_CHECKBOX6, _("Double-click takes size parameters of elements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    CheckBox6->SetValue(false);
    BoxSizer2->Add(CheckBox6, 0, wxEXPAND, 5);
    CheckBox7 = new wxCheckBox(Panel1, ID_CHECKBOX7, _("Limit text height (track width min. 0.15 mm)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    CheckBox7->SetValue(false);
    BoxSizer2->Add(CheckBox7, 0, wxEXPAND, 5);
    CheckBox8 = new wxCheckBox(Panel1, ID_CHECKBOX8, _("Components ID and value always readable after rotating"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
    CheckBox8->SetValue(false);
    BoxSizer2->Add(CheckBox8, 0, wxEXPAND, 5);
    CheckBox9 = new wxCheckBox(Panel1, ID_CHECKBOX9, _("Optimize nodes of a track automatically"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
    CheckBox9->SetValue(false);
    BoxSizer2->Add(CheckBox9, 0, wxEXPAND, 5);
    CheckBox10 = new wxCheckBox(Panel1, ID_CHECKBOX10, _("Origin top/left (instead of bottom/left)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
    CheckBox10->SetValue(false);
    BoxSizer2->Add(CheckBox10, 0, wxEXPAND, 5);
    CheckBox11 = new wxCheckBox(Panel1, ID_CHECKBOX11, _("Use origin in CAM-export (Gerber/Excellon/HPGL)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX11"));
    CheckBox11->SetValue(false);
    BoxSizer2->Add(CheckBox11, 0, wxEXPAND, 5);
    Panel1->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel1);
    BoxSizer2->SetSizeHints(Panel1);
    Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("Color scheme:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    BoxSizer5->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ColorSchemeChoise = new wxChoice(Panel2, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    ColorSchemeChoise->SetSelection( ColorSchemeChoise->Append(_("Standart")) );
    ColorSchemeChoise->Append(_("User 1"));
    ColorSchemeChoise->Append(_("User 2"));
    ColorSchemeChoise->Append(_("User 3"));
    BoxSizer5->Add(ColorSchemeChoise, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4->Add(BoxSizer5, 0, wxALL|wxEXPAND, 5);
    ColorSchemePanel = new wxPanel(Panel2, ID_PANEL_COLORS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_COLORS"));
    ColorSchemePanel->Disable();
    BoxSizer9 = new wxBoxSizer(wxVERTICAL);
    FlexGridSizer1 = new wxFlexGridSizer(7, 4, 0, 0);
    ColorC1 = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL1, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL1"));
    FlexGridSizer1->Add(ColorC1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT4, _("C1 (Copper-Top)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer1->Add(StaticText4, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorBgr = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL2, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL2"));
    FlexGridSizer1->Add(ColorBgr, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT5, _("Background"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer1->Add(StaticText5, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorS1 = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL3, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL3"));
    FlexGridSizer1->Add(ColorS1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT6, _("S1 (Silkscreen-Top)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    FlexGridSizer1->Add(StaticText6, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorLines = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL4, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL4"));
    FlexGridSizer1->Add(ColorLines, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT7, _("Grid-lines"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    FlexGridSizer1->Add(StaticText7, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorC2 = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL5, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL5"));
    FlexGridSizer1->Add(ColorC2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText8 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT8, _("C2 (Copper-Bottom)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    FlexGridSizer1->Add(StaticText8, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorDots = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL6, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL6"));
    FlexGridSizer1->Add(ColorDots, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT9, _("Grid-dots"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    FlexGridSizer1->Add(StaticText9, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorS2 = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL7, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL7"));
    FlexGridSizer1->Add(ColorS2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText10 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT10, _("S2 (Silkscreen-Bottom)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    FlexGridSizer1->Add(StaticText10, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(-1,-1,1, wxEXPAND, 5);
    FlexGridSizer1->Add(-1,-1,1, wxEXPAND, 5);
    ColorI1 = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL8, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL8"));
    FlexGridSizer1->Add(ColorI1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT11, _("I1 (Copper-Inner 1)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    FlexGridSizer1->Add(StaticText11, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorCon = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL9, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL9"));
    FlexGridSizer1->Add(ColorCon, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT12, _("Connections"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    FlexGridSizer1->Add(StaticText12, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorI2 = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL10, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL10"));
    FlexGridSizer1->Add(ColorI2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText13 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT13, _("I2 (Copper-Inner 2)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    FlexGridSizer1->Add(StaticText13, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorVia = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL11, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL11"));
    FlexGridSizer1->Add(ColorVia, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText15 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT15, _("Via"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    FlexGridSizer1->Add(StaticText15, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    ColorO = new wxColourPickerCtrl(ColorSchemePanel, ID_COLOURPICKERCTRL12, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL12"));
    FlexGridSizer1->Add(ColorO, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText14 = new wxStaticText(ColorSchemePanel, ID_STATICTEXT14, _("O (Outline)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    FlexGridSizer1->Add(StaticText14, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer9->Add(FlexGridSizer1, 0, wxEXPAND, 0);
    ResetColor = new wxButton(ColorSchemePanel, ID_BUTTON1, _("Reset scheme to default"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer9->Add(ResetColor, 0, wxALL|wxALIGN_RIGHT, 5);
    ColorSchemePanel->SetSizer(BoxSizer9);
    BoxSizer9->Fit(ColorSchemePanel);
    BoxSizer9->SetSizeHints(ColorSchemePanel);
    BoxSizer4->Add(ColorSchemePanel, 1, wxALL|wxEXPAND, 5);
    Panel2->SetSizer(BoxSizer4);
    BoxSizer4->Fit(Panel2);
    BoxSizer4->SetSizeHints(Panel2);
    Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxPoint(197,11), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    BoxSizer6 = new wxBoxSizer(wxVERTICAL);
    BoxSizer7 = new wxBoxSizer(wxVERTICAL);
    BoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    StaticText16 = new wxStaticText(Panel3, ID_STATICTEXT16, _("Layout files:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    BoxSizer26->Add(StaticText16, 1, wxALL|wxEXPAND, 5);
    DirPickerCtrl4 = new wxDirPickerCtrl(Panel3, ID_DIRPICKERCTRL4, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL, wxDefaultValidator, _T("ID_DIRPICKERCTRL4"));
    BoxSizer26->Add(DirPickerCtrl4, 2, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer7->Add(BoxSizer26, 1, wxEXPAND, 5);
    OtherDirsPanel = new wxPanel(Panel3, ID_PANEL_DIRS, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTRANSPARENT_WINDOW|wxTAB_TRAVERSAL, _T("ID_PANEL_DIRS"));
    OtherDirsPanel->Disable();
    BoxSizer8 = new wxBoxSizer(wxVERTICAL);
    BoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    StaticText17 = new wxStaticText(OtherDirsPanel, ID_STATICTEXT17, _("Gerber export:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    BoxSizer24->Add(StaticText17, 1, wxALL|wxEXPAND, 5);
    DirPickerCtrl2 = new wxDirPickerCtrl(OtherDirsPanel, ID_DIRPICKERCTRL2, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL, wxDefaultValidator, _T("ID_DIRPICKERCTRL2"));
    BoxSizer24->Add(DirPickerCtrl2, 2, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer8->Add(BoxSizer24, 1, wxEXPAND, 5);
    BoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
    StaticText18 = new wxStaticText(OtherDirsPanel, ID_STATICTEXT18, _("Bitmap export:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    BoxSizer25->Add(StaticText18, 1, wxALL|wxEXPAND, 5);
    DirPickerCtrl3 = new wxDirPickerCtrl(OtherDirsPanel, ID_DIRPICKERCTRL3, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL, wxDefaultValidator, _T("ID_DIRPICKERCTRL3"));
    BoxSizer25->Add(DirPickerCtrl3, 2, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer8->Add(BoxSizer25, 1, wxEXPAND, 5);
    BoxSizer27 = new wxBoxSizer(wxHORIZONTAL);
    StaticText19 = new wxStaticText(OtherDirsPanel, ID_STATICTEXT19, _("HPGL export:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
    BoxSizer27->Add(StaticText19, 1, wxALL|wxEXPAND, 5);
    DirPickerCtrl5 = new wxDirPickerCtrl(OtherDirsPanel, ID_DIRPICKERCTRL5, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL, wxDefaultValidator, _T("ID_DIRPICKERCTRL5"));
    BoxSizer27->Add(DirPickerCtrl5, 2, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer8->Add(BoxSizer27, 1, wxEXPAND, 5);
    BoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    StaticText20 = new wxStaticText(OtherDirsPanel, ID_STATICTEXT20, _("Scanned copies:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
    BoxSizer23->Add(StaticText20, 1, wxALL|wxEXPAND, 5);
    DirPickerCtrl1 = new wxDirPickerCtrl(OtherDirsPanel, ID_DIRPICKERCTRL1, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL, wxDefaultValidator, _T("ID_DIRPICKERCTRL1"));
    BoxSizer23->Add(DirPickerCtrl1, 2, wxLEFT|wxRIGHT|wxEXPAND, 5);
    BoxSizer8->Add(BoxSizer23, 0, wxEXPAND, 0);
    OtherDirsPanel->SetSizer(BoxSizer8);
    BoxSizer8->Fit(OtherDirsPanel);
    BoxSizer8->SetSizeHints(OtherDirsPanel);
    BoxSizer7->Add(OtherDirsPanel, 0, wxEXPAND, 5);
    BoxSizer6->Add(BoxSizer7, 0, wxALL|wxEXPAND, 5);
    StaticText21 = new wxStaticText(Panel3, ID_STATICTEXT21, _("Leave this fields empty, if you want OpenLayout to remember\nthe last used directories."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
    BoxSizer6->Add(StaticText21, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer6->Add(-1,-1,1, wxALL|wxEXPAND, 5);
    SameDir = new wxCheckBox(Panel3, ID_CHECKBOX12, _("Use the same folder for all file types"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX12"));
    SameDir->SetValue(true);
    BoxSizer6->Add(SameDir, 0, wxALL|wxEXPAND, 5);
    Panel3->SetSizer(BoxSizer6);
    BoxSizer6->Fit(Panel3);
    BoxSizer6->SetSizeHints(Panel3);
    Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxPoint(399,13), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    BoxSizer10 = new wxBoxSizer(wxVERTICAL);
    StaticText22 = new wxStaticText(Panel4, ID_STATICTEXT22, _("Root-Directory for macros:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
    BoxSizer10->Add(StaticText22, 0, wxALL|wxALIGN_LEFT, 5);
    DirPickerCtrl6 = new wxDirPickerCtrl(Panel4, ID_DIRPICKERCTRL6, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL, wxDefaultValidator, _T("ID_DIRPICKERCTRL6"));
    BoxSizer10->Add(DirPickerCtrl6, 0, wxALL|wxEXPAND, 20);
    FlexGridSizer2 = new wxFlexGridSizer(2, 2, 0, 0);
    Button2 = new wxButton(Panel4, ID_BUTTON2, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    FlexGridSizer2->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText23 = new wxStaticText(Panel4, ID_STATICTEXT23, _("This will reset the Macro-Directory to default setting"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
    FlexGridSizer2->Add(StaticText23, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(Panel4, ID_BUTTON3, _("Folder"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    FlexGridSizer2->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText24 = new wxStaticText(Panel4, ID_STATICTEXT24, _("Opens the Macro-Directory in the file manager"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
    FlexGridSizer2->Add(StaticText24, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer10->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
    Panel4->SetSizer(BoxSizer10);
    BoxSizer10->Fit(Panel4);
    BoxSizer10->SetSizeHints(Panel4);
    Panel5 = new wxPanel(Notebook1, ID_PANEL5, wxPoint(460,12), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    BoxSizer11 = new wxBoxSizer(wxVERTICAL);
    SpinCtrl1 = new wxSpinCtrl(Panel5, ID_SPINCTRL1, _T("50"), wxDefaultPosition, wxDefaultSize, 0, 1, 50, 50, _T("ID_SPINCTRL1"));
    SpinCtrl1->SetValue(_T("50"));
    BoxSizer11->Add(SpinCtrl1, 0, wxALL|wxALIGN_LEFT, 5);
    StaticText25 = new wxStaticText(Panel5, ID_STATICTEXT25, _("Here you can define the maximum number of UNDO operations.\nThe maximum is 50 operations\nIf your system runs very slowly while working with big layouts,\nyou can decrease this value down to 1."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
    BoxSizer11->Add(StaticText25, 1, wxALL|wxALIGN_LEFT, 5);
    Panel5->SetSizer(BoxSizer11);
    BoxSizer11->Fit(Panel5);
    BoxSizer11->SetSizeHints(Panel5);
    Panel6 = new wxPanel(Notebook1, ID_PANEL6, wxPoint(27,279), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    BoxSizer12 = new wxBoxSizer(wxVERTICAL);
    BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    StaticText27 = new wxStaticText(Panel6, ID_STATICTEXT27, _("Copper-Thickness in um:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
    BoxSizer13->Add(StaticText27, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl2 = new wxSpinCtrl(Panel6, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL2"));
    SpinCtrl2->SetValue(_T("0"));
    BoxSizer13->Add(SpinCtrl2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer12->Add(BoxSizer13, 0, wxALL|wxEXPAND, 5);
    BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    StaticText28 = new wxStaticText(Panel6, ID_STATICTEXT28, _("Temperature enhance in C:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
    BoxSizer14->Add(StaticText28, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl3 = new wxSpinCtrl(Panel6, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL3"));
    SpinCtrl3->SetValue(_T("0"));
    BoxSizer14->Add(SpinCtrl3, 1, wxALIGN_TOP, 5);
    BoxSizer12->Add(BoxSizer14, 0, wxALL|wxEXPAND, 5);
    StaticText26 = new wxStaticText(Panel6, ID_STATICTEXT26, _("The calculation of the max. current-carrying capacity for a track,\nis only a coarse approximate value.\nThe real value is depending on many other factors and the environment.\n"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
    BoxSizer12->Add(StaticText26, 1, wxALL|wxALIGN_LEFT, 5);
    Panel6->SetSizer(BoxSizer12);
    BoxSizer12->Fit(Panel6);
    BoxSizer12->SetSizeHints(Panel6);
    Panel7 = new wxPanel(Notebook1, ID_PANEL7, wxPoint(94,229), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    BoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    HotkeyList = new wxListBox(Panel7, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    HotkeyList->SetSelection( HotkeyList->Append(_("Edit")) );
    HotkeyList->Append(_("Zoom"));
    HotkeyList->Append(_("Track"));
    HotkeyList->Append(_("Pad"));
    HotkeyList->Append(_("SMD-Pad"));
    HotkeyList->Append(_("Circle"));
    HotkeyList->Append(_("Rectangle"));
    HotkeyList->Append(_("Zone"));
    HotkeyList->Append(_("Special form"));
    HotkeyList->Append(_("Text"));
    HotkeyList->Append(_("Connection"));
    HotkeyList->Append(_("Autoroute"));
    HotkeyList->Append(_("Text"));
    HotkeyList->Append(_("Measure"));
    HotkeyList->Append(_("Photoview"));
    HotkeyList->Append(_("Solder mask"));
    BoxSizer15->Add(HotkeyList, 0, wxALL|wxEXPAND, 5);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel7, _("Change hotkey"));
    BoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    HotkeyLabel = new wxStaticText(Panel7, ID_STATICTEXT29, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT29"));
    BoxSizer16->Add(HotkeyLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    HotkeyChoise = new wxChoice(Panel7, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE4"));
    HotkeyChoise->Append(_("Esc"));
    HotkeyChoise->Append(_("A"));
    HotkeyChoise->Append(_("B"));
    HotkeyChoise->Append(_("C"));
    HotkeyChoise->Append(_("D"));
    HotkeyChoise->Append(_("E"));
    HotkeyChoise->Append(_("F"));
    HotkeyChoise->Append(_("G"));
    HotkeyChoise->Append(_("H"));
    HotkeyChoise->Append(_("I"));
    HotkeyChoise->Append(_("J"));
    HotkeyChoise->Append(_("K"));
    HotkeyChoise->Append(_("L"));
    HotkeyChoise->Append(_("M"));
    HotkeyChoise->Append(_("N"));
    HotkeyChoise->Append(_("O"));
    HotkeyChoise->Append(_("P"));
    HotkeyChoise->Append(_("Q"));
    HotkeyChoise->Append(_("R"));
    HotkeyChoise->Append(_("S"));
    HotkeyChoise->Append(_("T"));
    HotkeyChoise->Append(_("U"));
    HotkeyChoise->Append(_("V"));
    HotkeyChoise->Append(_("W"));
    HotkeyChoise->Append(_("X"));
    HotkeyChoise->Append(_("Y"));
    HotkeyChoise->Append(_("Z"));
    BoxSizer16->Add(HotkeyChoise, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    StaticBoxSizer1->Add(BoxSizer16, 0, wxALIGN_TOP, 0);
    BoxSizer15->Add(StaticBoxSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel7->SetSizer(BoxSizer15);
    BoxSizer15->Fit(Panel7);
    BoxSizer15->SetSizeHints(Panel7);
    Panel8 = new wxPanel(Notebook1, ID_PANEL8, wxPoint(100,248), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    BoxSizer17 = new wxBoxSizer(wxVERTICAL);
    CheckBox13 = new wxCheckBox(Panel8, ID_CHECKBOX13, _("Show 45 lines"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX13"));
    CheckBox13->SetValue(true);
    BoxSizer17->Add(CheckBox13, 0, wxALL|wxALIGN_LEFT, 5);
    CrosshairCoord = new wxCheckBox(Panel8, ID_CHECKBOX14, _("Show coordinates of crosshair"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX14"));
    CrosshairCoord->SetValue(true);
    BoxSizer17->Add(CrosshairCoord, 0, wxALL|wxALIGN_LEFT, 5);
    Panel13 = new wxPanel(Panel8, ID_PANEL_CROSSHAIR, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_CROSSHAIR"));
    BoxSizer18 = new wxBoxSizer(wxVERTICAL);
    CheckBox15 = new wxCheckBox(Panel13, ID_CHECKBOX15, _("Transparent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX15"));
    CheckBox15->SetValue(true);
    BoxSizer18->Add(CheckBox15, 1, wxALL|wxALIGN_LEFT, 5);
    CheckBox16 = new wxCheckBox(Panel13, ID_CHECKBOX16, _("Big text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX16"));
    CheckBox16->SetValue(false);
    BoxSizer18->Add(CheckBox16, 1, wxALL|wxALIGN_LEFT, 5);
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel13, _("Textbox"));
    BoxSizer19 = new wxBoxSizer(wxVERTICAL);
    RadioButton1 = new wxRadioButton(Panel13, ID_RADIOBUTTON1, _("Black background"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
    BoxSizer19->Add(RadioButton1, 1, wxALIGN_LEFT, 5);
    RadioButton2 = new wxRadioButton(Panel13, ID_RADIOBUTTON2, _("White background"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
    BoxSizer19->Add(RadioButton2, 1, wxALIGN_LEFT, 5);
    StaticBoxSizer2->Add(BoxSizer19, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel10 = new wxPanel(Panel13, ID_PANEL10, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL10"));
    Panel10->SetBackgroundColour(wxColour(0,0,0));
    BoxSizer20 = new wxBoxSizer(wxVERTICAL);
    StaticText29 = new wxStaticText(Panel10, ID_STATICTEXT30, _("X:  12.280 mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT30"));
    StaticText29->SetForegroundColour(wxColour(255,255,255));
    BoxSizer20->Add(StaticText29, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText30 = new wxStaticText(Panel10, ID_STATICTEXT31, _("Y:  14.640 mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT31"));
    StaticText30->SetForegroundColour(wxColour(255,255,255));
    BoxSizer20->Add(StaticText30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel10->SetSizer(BoxSizer20);
    BoxSizer20->Fit(Panel10);
    BoxSizer20->SetSizeHints(Panel10);
    StaticBoxSizer2->Add(Panel10, 1, wxALL|wxEXPAND, 5);
    BoxSizer18->Add(StaticBoxSizer2, 0, wxALL|wxALIGN_LEFT, 5);
    Panel13->SetSizer(BoxSizer18);
    BoxSizer18->Fit(Panel13);
    BoxSizer18->SetSizeHints(Panel13);
    BoxSizer17->Add(Panel13, 0, wxLEFT|wxALIGN_LEFT, 20);
    Panel8->SetSizer(BoxSizer17);
    BoxSizer17->Fit(Panel8);
    BoxSizer17->SetSizeHints(Panel8);
    Panel9 = new wxPanel(Notebook1, ID_PANEL9, wxPoint(113,233), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    BoxSizer21 = new wxBoxSizer(wxVERTICAL);
    StaticText31 = new wxStaticText(Panel9, ID_STATICTEXT32, _("Save layout periodical\nto a separate backup file (*.bak)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT32"));
    BoxSizer21->Add(StaticText31, 0, wxALL|wxALIGN_LEFT, 5);
    AutosaveButton = new wxCheckBox(Panel9, ID_CHECKBOX17, _("Activate AutoSave"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX17"));
    AutosaveButton->SetValue(false);
    BoxSizer21->Add(AutosaveButton, 0, wxALL|wxALIGN_LEFT, 5);
    Panel11 = new wxPanel(Panel9, ID_PANEL_AUTOSAVE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL_AUTOSAVE"));
    Panel11->Disable();
    BoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    StaticText32 = new wxStaticText(Panel11, ID_STATICTEXT33, _("Interval:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT33"));
    BoxSizer22->Add(StaticText32, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SpinCtrl4 = new wxSpinCtrl(Panel11, ID_SPINCTRL4, _T("5"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 5, _T("ID_SPINCTRL4"));
    SpinCtrl4->SetValue(_T("5"));
    BoxSizer22->Add(SpinCtrl4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText33 = new wxStaticText(Panel11, ID_STATICTEXT34, _("min"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT34"));
    BoxSizer22->Add(StaticText33, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel11->SetSizer(BoxSizer22);
    BoxSizer22->Fit(Panel11);
    BoxSizer22->SetSizeHints(Panel11);
    BoxSizer21->Add(Panel11, 0, wxLEFT|wxALIGN_LEFT, 20);
    Panel9->SetSizer(BoxSizer21);
    BoxSizer21->Fit(Panel9);
    BoxSizer21->SetSizeHints(Panel9);
    Notebook1->AddPage(Panel1, _("General settings"), false);
    Notebook1->AddPage(Panel2, _("Colors"), false);
    Notebook1->AddPage(Panel3, _("Working directories"), false);
    Notebook1->AddPage(Panel4, _("Macro-Directory"), false);
    Notebook1->AddPage(Panel5, _("Undo-Depth"), false);
    Notebook1->AddPage(Panel6, _("Imax"), false);
    Notebook1->AddPage(Panel7, _("Hotkeys"), false);
    Notebook1->AddPage(Panel8, _("Crosshair"), false);
    Notebook1->AddPage(Panel9, _("AutoSave"), false);
    BoxSizer1->Add(Notebook1, 1, wxALL|wxEXPAND, 5);
    StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
    StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
    StdDialogButtonSizer1->Realize();
    BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_CHOICE3,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChoiseSelect);
    Connect(ID_COLOURPICKERCTRL1,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL2,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL3,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL4,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL5,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL6,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL7,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL8,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL9,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL10,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL11,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_COLOURPICKERCTRL12,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&SettingsDialog::OnColorSchemeChanged);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnResetColorClick);
    Connect(ID_CHECKBOX12,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SettingsDialog::OnSameDirClick);
    //*)
    SetColorScheme(ColorScheme());
}

SettingsDialog::~SettingsDialog() {
    //(*Destroy(SettingsDialog)
    //*)
}
void SettingsDialog::Set(const Settings s1){
	s=s1;
	if(s.s_color_scheme!=0)
		SetColorScheme(s.colors[s.s_color_scheme-1]);
	else
		SetColorScheme(ColorScheme());
	ColorSchemeChoise->SetSelection(s.s_color_scheme);
}
void SettingsDialog::Get(Settings &s1){
	s1=s;
}
void SettingsDialog::SetColorScheme(const ColorScheme c){
	ColorC1		-> SetColour(c.c1);
	ColorC2		-> SetColour(c.c2);
	ColorS1		-> SetColour(c.s1);
	ColorS2		-> SetColour(c.s2);
	ColorI1		-> SetColour(c.i1);
	ColorI2		-> SetColour(c.i2);
	ColorO		-> SetColour(c.o);
	ColorBgr	-> SetColour(c.bgr);
	ColorLines	-> SetColour(c.lines);
	ColorDots	-> SetColour(c.dots);
	ColorVia	-> SetColour(c.via);
}

void SettingsDialog::GetColorScheme(ColorScheme &c){
	c.c1		= ColorC1		-> GetColour();
	c.c2		= ColorC2		-> GetColour();
	c.s1		= ColorS1		-> GetColour();
	c.s2		= ColorS2		-> GetColour();
	c.i1		= ColorI1		-> GetColour();
	c.i2		= ColorI2		-> GetColour();
	c.o			= ColorO		-> GetColour();
	c.bgr		= ColorBgr		-> GetColour();
	c.lines		= ColorLines	-> GetColour();
	c.dots		= ColorDots		-> GetColour();
	c.via		= ColorVia		-> GetColour();
}
void SettingsDialog::OnColorSchemeChoiseSelect(wxCommandEvent& event){
	int selected=static_cast<wxChoice*>(event.GetEventObject())->GetSelection();
	if(selected!=0)
		SetColorScheme(s.colors[selected-1]);
	else
		SetColorScheme(ColorScheme());
	s.s_color_scheme=selected;
}

void SettingsDialog::OnResetColorClick(wxCommandEvent& event){
	assert(s.s_color_scheme!=0);
	SetColorScheme(ColorScheme());
	s.colors[s.s_color_scheme-1]=ColorScheme();
}

void SettingsDialog::OnColorSchemeChanged(wxColourPickerEvent& event){
	assert(s.s_color_scheme!=0);
	GetColorScheme(s.colors[s.s_color_scheme-1]);
}

void SettingsDialog::OnSameDirClick(wxCommandEvent& event){
	bool selected=static_cast<wxCheckBox*>(event.GetEventObject())->GetValue();
	OtherDirsPanel->Enable(!selected);
}

void SettingsDialog::UpdateColorPanel(wxUpdateUIEvent &event){
	event.Enable(s.s_color_scheme>0);
}
void SettingsDialog::UpdateDirsPanel(wxUpdateUIEvent &event){
	event.Enable(!SameDir->GetValue());
}
void SettingsDialog::UpdateCrosshairPanel(wxUpdateUIEvent &event){
	event.Enable(CrosshairCoord->GetValue());
}
void SettingsDialog::UpdateAutosavePanel(wxUpdateUIEvent &event){
	event.Enable(AutosaveButton->GetValue());
}
