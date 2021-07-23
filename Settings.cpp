#include "Settings.h"

//(*InternalHeaders(Settings)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Settings)
const long Settings::ID_STATICTEXT1 = wxNewId();
const long Settings::ID_CHOICE1 = wxNewId();
const long Settings::ID_STATICTEXT2 = wxNewId();
const long Settings::ID_CHOICE2 = wxNewId();
const long Settings::ID_CHECKBOX1 = wxNewId();
const long Settings::ID_CHECKBOX2 = wxNewId();
const long Settings::ID_CHECKBOX3 = wxNewId();
const long Settings::ID_CHECKBOX4 = wxNewId();
const long Settings::ID_CHECKBOX5 = wxNewId();
const long Settings::ID_CHECKBOX6 = wxNewId();
const long Settings::ID_CHECKBOX7 = wxNewId();
const long Settings::ID_CHECKBOX8 = wxNewId();
const long Settings::ID_CHECKBOX9 = wxNewId();
const long Settings::ID_CHECKBOX10 = wxNewId();
const long Settings::ID_CHECKBOX11 = wxNewId();
const long Settings::ID_PANEL1 = wxNewId();
const long Settings::ID_STATICTEXT3 = wxNewId();
const long Settings::ID_CHOICE3 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL1 = wxNewId();
const long Settings::ID_STATICTEXT4 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL2 = wxNewId();
const long Settings::ID_STATICTEXT5 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL3 = wxNewId();
const long Settings::ID_STATICTEXT6 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL4 = wxNewId();
const long Settings::ID_STATICTEXT7 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL5 = wxNewId();
const long Settings::ID_STATICTEXT8 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL6 = wxNewId();
const long Settings::ID_STATICTEXT9 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL7 = wxNewId();
const long Settings::ID_STATICTEXT10 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL8 = wxNewId();
const long Settings::ID_STATICTEXT11 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL9 = wxNewId();
const long Settings::ID_STATICTEXT12 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL10 = wxNewId();
const long Settings::ID_STATICTEXT13 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL11 = wxNewId();
const long Settings::ID_STATICTEXT15 = wxNewId();
const long Settings::ID_COLOURPICKERCTRL12 = wxNewId();
const long Settings::ID_STATICTEXT14 = wxNewId();
const long Settings::ID_BUTTON1 = wxNewId();
const long Settings::ID_PANEL2 = wxNewId();
const long Settings::ID_STATICTEXT16 = wxNewId();
const long Settings::ID_STATICTEXT17 = wxNewId();
const long Settings::ID_STATICTEXT18 = wxNewId();
const long Settings::ID_STATICTEXT19 = wxNewId();
const long Settings::ID_STATICTEXT20 = wxNewId();
const long Settings::ID_DIRPICKERCTRL1 = wxNewId();
const long Settings::ID_DIRPICKERCTRL2 = wxNewId();
const long Settings::ID_DIRPICKERCTRL3 = wxNewId();
const long Settings::ID_DIRPICKERCTRL4 = wxNewId();
const long Settings::ID_DIRPICKERCTRL5 = wxNewId();
const long Settings::ID_STATICTEXT21 = wxNewId();
const long Settings::ID_CHECKBOX12 = wxNewId();
const long Settings::ID_PANEL3 = wxNewId();
const long Settings::ID_STATICTEXT22 = wxNewId();
const long Settings::ID_PANEL4 = wxNewId();
const long Settings::ID_NOTEBOOK1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Settings,wxDialog)
	//(*EventTable(Settings)
	//*)
END_EVENT_TABLE()

Settings::Settings(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Settings)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
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
	BoxSizer2->Add(CheckBox1, 1, wxEXPAND, 5);
	CheckBox2 = new wxCheckBox(Panel1, ID_CHECKBOX2, _("Darken ground-plane"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox2->SetValue(false);
	BoxSizer2->Add(CheckBox2, 1, wxEXPAND, 5);
	CheckBox3 = new wxCheckBox(Panel1, ID_CHECKBOX3, _("Show ground-plane of all layers simultaneos"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox3->SetValue(false);
	BoxSizer2->Add(CheckBox3, 1, wxEXPAND, 5);
	CheckBox4 = new wxCheckBox(Panel1, ID_CHECKBOX4, _("Consider connections (rubberbands) in TEST mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	CheckBox4->SetValue(false);
	BoxSizer2->Add(CheckBox4, 1, wxEXPAND, 5);
	CheckBox5 = new wxCheckBox(Panel1, ID_CHECKBOX5, _("Blinking TEST mode"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	CheckBox5->SetValue(false);
	BoxSizer2->Add(CheckBox5, 1, wxALL|wxEXPAND, 0);
	CheckBox6 = new wxCheckBox(Panel1, ID_CHECKBOX6, _("Double-click takes size parameters of elements"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
	CheckBox6->SetValue(false);
	BoxSizer2->Add(CheckBox6, 1, wxEXPAND, 5);
	CheckBox7 = new wxCheckBox(Panel1, ID_CHECKBOX7, _("Limit text height (track width min. 0.15 mm)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
	CheckBox7->SetValue(false);
	BoxSizer2->Add(CheckBox7, 1, wxEXPAND, 5);
	CheckBox8 = new wxCheckBox(Panel1, ID_CHECKBOX8, _("Components ID and value always readable after rotating"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
	CheckBox8->SetValue(false);
	BoxSizer2->Add(CheckBox8, 1, wxEXPAND, 5);
	CheckBox9 = new wxCheckBox(Panel1, ID_CHECKBOX9, _("Optimize nodes of a track automatically"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
	CheckBox9->SetValue(false);
	BoxSizer2->Add(CheckBox9, 1, wxEXPAND, 5);
	CheckBox10 = new wxCheckBox(Panel1, ID_CHECKBOX10, _("Origin top/left (instead of bottom/left)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
	CheckBox10->SetValue(false);
	BoxSizer2->Add(CheckBox10, 1, wxEXPAND, 5);
	CheckBox11 = new wxCheckBox(Panel1, ID_CHECKBOX11, _("Use origin in CAM-export (Gerber/Excellon/HPGL)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX11"));
	CheckBox11->SetValue(false);
	BoxSizer2->Add(CheckBox11, 1, wxEXPAND, 5);
	Panel1->SetSizer(BoxSizer2);
	BoxSizer2->Fit(Panel1);
	BoxSizer2->SetSizeHints(Panel1);
	Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(Panel2, ID_STATICTEXT3, _("Color scheme:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer5->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Choice3 = new wxChoice(Panel2, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
	Choice3->Append(_("Standart"));
	Choice3->Append(_("User 1"));
	Choice3->Append(_("User 2"));
	Choice3->Append(_("User 3"));
	BoxSizer5->Add(Choice3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(BoxSizer5, 0, wxALL|wxEXPAND, 5);
	FlexGridSizer1 = new wxFlexGridSizer(7, 4, 0, 0);
	ColourPickerCtrl1 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL1, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL1"));
	FlexGridSizer1->Add(ColourPickerCtrl1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT4, _("C1 (Copper-Top)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl2 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL2, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL2"));
	FlexGridSizer1->Add(ColourPickerCtrl2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(Panel2, ID_STATICTEXT5, _("Background"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl3 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL3, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL3"));
	FlexGridSizer1->Add(ColourPickerCtrl3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(Panel2, ID_STATICTEXT6, _("S1 (Silkscreen-Top)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl4 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL4, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL4"));
	FlexGridSizer1->Add(ColourPickerCtrl4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(Panel2, ID_STATICTEXT7, _("Grid-lines"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl5 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL5, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL5"));
	FlexGridSizer1->Add(ColourPickerCtrl5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(Panel2, ID_STATICTEXT8, _("C2 (Copper-Bottom)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl6 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL6, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL6"));
	FlexGridSizer1->Add(ColourPickerCtrl6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(Panel2, ID_STATICTEXT9, _("Grid-dots"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl7 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL7, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL7"));
	FlexGridSizer1->Add(ColourPickerCtrl7, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(Panel2, ID_STATICTEXT10, _("S2 (Silkscreen-Bottom)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer1->Add(StaticText10, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxEXPAND, 5);
	FlexGridSizer1->Add(-1,-1,1, wxEXPAND, 5);
	ColourPickerCtrl8 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL8, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL8"));
	FlexGridSizer1->Add(ColourPickerCtrl8, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(Panel2, ID_STATICTEXT11, _("I1 (Copper-Inner 1)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText11, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl9 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL9, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL9"));
	FlexGridSizer1->Add(ColourPickerCtrl9, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText12 = new wxStaticText(Panel2, ID_STATICTEXT12, _("Connections"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer1->Add(StaticText12, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl10 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL10, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL10"));
	FlexGridSizer1->Add(ColourPickerCtrl10, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText13 = new wxStaticText(Panel2, ID_STATICTEXT13, _("I2 (Copper-Inner 2)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer1->Add(StaticText13, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl11 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL11, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL11"));
	FlexGridSizer1->Add(ColourPickerCtrl11, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText15 = new wxStaticText(Panel2, ID_STATICTEXT15, _("Via"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer1->Add(StaticText15, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ColourPickerCtrl12 = new wxColourPickerCtrl(Panel2, ID_COLOURPICKERCTRL12, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL12"));
	FlexGridSizer1->Add(ColourPickerCtrl12, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText14 = new wxStaticText(Panel2, ID_STATICTEXT14, _("O (Outline)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer1->Add(StaticText14, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 5);
	Button1 = new wxButton(Panel2, ID_BUTTON1, _("Reset scheme to default"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer4->Add(Button1, 0, wxALL|wxALIGN_RIGHT, 5);
	Panel2->SetSizer(BoxSizer4);
	BoxSizer4->Fit(Panel2);
	BoxSizer4->SetSizeHints(Panel2);
	Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxPoint(197,11), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	StaticText16 = new wxStaticText(Panel3, ID_STATICTEXT16, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	BoxSizer8->Add(StaticText16, 1, wxALL|wxEXPAND, 5);
	StaticText17 = new wxStaticText(Panel3, ID_STATICTEXT17, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	BoxSizer8->Add(StaticText17, 1, wxALL|wxEXPAND, 5);
	StaticText18 = new wxStaticText(Panel3, ID_STATICTEXT18, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	BoxSizer8->Add(StaticText18, 1, wxALL|wxEXPAND, 5);
	StaticText19 = new wxStaticText(Panel3, ID_STATICTEXT19, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	BoxSizer8->Add(StaticText19, 1, wxALL|wxEXPAND, 5);
	StaticText20 = new wxStaticText(Panel3, ID_STATICTEXT20, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	BoxSizer8->Add(StaticText20, 1, wxALL|wxEXPAND, 5);
	BoxSizer7->Add(BoxSizer8, 1, wxALL|wxEXPAND, 5);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	DirPickerCtrl1 = new wxDirPickerCtrl(Panel3, ID_DIRPICKERCTRL1, wxEmptyString, _("Select a directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST, wxDefaultValidator, _T("ID_DIRPICKERCTRL1"));
	BoxSizer9->Add(DirPickerCtrl1, 1, wxEXPAND, 5);
	DirPickerCtrl2 = new wxDirPickerCtrl(Panel3, ID_DIRPICKERCTRL2, wxEmptyString, _("Select a directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST, wxDefaultValidator, _T("ID_DIRPICKERCTRL2"));
	BoxSizer9->Add(DirPickerCtrl2, 1, wxEXPAND, 5);
	DirPickerCtrl3 = new wxDirPickerCtrl(Panel3, ID_DIRPICKERCTRL3, wxEmptyString, _("Select a directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST, wxDefaultValidator, _T("ID_DIRPICKERCTRL3"));
	BoxSizer9->Add(DirPickerCtrl3, 1, wxEXPAND, 5);
	DirPickerCtrl4 = new wxDirPickerCtrl(Panel3, ID_DIRPICKERCTRL4, wxEmptyString, _("Select a directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST, wxDefaultValidator, _T("ID_DIRPICKERCTRL4"));
	BoxSizer9->Add(DirPickerCtrl4, 1, wxEXPAND, 5);
	DirPickerCtrl5 = new wxDirPickerCtrl(Panel3, ID_DIRPICKERCTRL5, wxEmptyString, _("Select a directory"), wxDefaultPosition, wxDefaultSize, wxDIRP_DIR_MUST_EXIST, wxDefaultValidator, _T("ID_DIRPICKERCTRL5"));
	BoxSizer9->Add(DirPickerCtrl5, 1, wxEXPAND, 5);
	BoxSizer7->Add(BoxSizer9, 1, wxALL|wxEXPAND, 5);
	BoxSizer6->Add(BoxSizer7, 0, wxALL|wxEXPAND, 5);
	StaticText21 = new wxStaticText(Panel3, ID_STATICTEXT21, _("Leave this fields empty, if you want OpenLayout to remember\nthe last used directories."), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	BoxSizer6->Add(StaticText21, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer6->Add(0,0,1, wxALL|wxEXPAND, 5);
	CheckBox12 = new wxCheckBox(Panel3, ID_CHECKBOX12, _("Use the same folder for all file types"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX12"));
	CheckBox12->SetValue(false);
	BoxSizer6->Add(CheckBox12, 0, wxALL|wxEXPAND, 5);
	Panel3->SetSizer(BoxSizer6);
	BoxSizer6->Fit(Panel3);
	BoxSizer6->SetSizeHints(Panel3);
	Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxPoint(399,13), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	StaticText22 = new wxStaticText(Panel4, ID_STATICTEXT22, _("Root-Directory for macros:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	BoxSizer10->Add(StaticText22, 1, wxALL|wxALIGN_LEFT, 5);
	Panel4->SetSizer(BoxSizer10);
	BoxSizer10->Fit(Panel4);
	BoxSizer10->SetSizeHints(Panel4);
	Notebook1->AddPage(Panel1, _("General settings"), false);
	Notebook1->AddPage(Panel2, _("Colors"), false);
	Notebook1->AddPage(Panel3, _("Working directories"), false);
	Notebook1->AddPage(Panel4, _("Macro-Directory"), false);
	BoxSizer1->Add(Notebook1, 1, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

Settings::~Settings()
{
	//(*Destroy(Settings)
	//*)
}


