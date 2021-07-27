#include "NewBoardDialog.h"
#include <sstream>
#define NEWBOARD_XPM
#include "images.h"
#undef NEWBOARD_XPM

//(*InternalHeaders(NewBoardDialog)
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(NewBoardDialog)
const long NewBoardDialog::ID_BITMAPBUTTON1 = wxNewId();
const long NewBoardDialog::ID_RADIOBUTTON1 = wxNewId();
const long NewBoardDialog::ID_BITMAPBUTTON2 = wxNewId();
const long NewBoardDialog::ID_RADIOBUTTON2 = wxNewId();
const long NewBoardDialog::ID_BITMAPBUTTON3 = wxNewId();
const long NewBoardDialog::ID_RADIOBUTTON3 = wxNewId();
const long NewBoardDialog::ID_PANEL2 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT1 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT2 = wxNewId();
const long NewBoardDialog::ID_SPINCTRLDOUBLE1 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT5 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT3 = wxNewId();
const long NewBoardDialog::ID_SPINCTRLDOUBLE2 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT6 = wxNewId();
const long NewBoardDialog::ID_PANEL1 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT7 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT8 = wxNewId();
const long NewBoardDialog::ID_SPINCTRLDOUBLE3 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT9 = wxNewId();
const long NewBoardDialog::ID_PANEL3 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT12 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT13 = wxNewId();
const long NewBoardDialog::ID_SPINCTRLDOUBLE5 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT14 = wxNewId();
const long NewBoardDialog::ID_PANEL4 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT10 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT11 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT15 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT16 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT17 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT18 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT19 = wxNewId();
const long NewBoardDialog::ID_PANEL5 = wxNewId();
const long NewBoardDialog::ID_PANEL6 = wxNewId();
const long NewBoardDialog::ID_STATICTEXT4 = wxNewId();
const long NewBoardDialog::ID_TEXTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(NewBoardDialog,wxDialog)
	//(*EventTable(NewBoardDialog)
	//*)
END_EVENT_TABLE()

NewBoardDialog::NewBoardDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(NewBoardDialog)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer11;
	wxFlexGridSizer* FlexGridSizer12;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer9;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, wxID_ANY, _("New board"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(5, 1, 0, 0);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	Panel = new wxPanel(this, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	FlexGridSizer2 = new wxFlexGridSizer(3, 2, 0, 0);
	Image1 = new wxBitmapButton(Panel, ID_BITMAPBUTTON1, board1_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON1"));
	FlexGridSizer2->Add(Image1, 1, wxALL|wxEXPAND, 5);
	Button1 = new wxRadioButton(Panel, ID_RADIOBUTTON1, _("Empty working-space\nwithout PCB outline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	Button1->SetValue(true);
	FlexGridSizer2->Add(Button1, 1, wxALL|wxEXPAND, 5);
	Image2 = new wxBitmapButton(Panel, ID_BITMAPBUTTON2, board2_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON2"));
	FlexGridSizer2->Add(Image2, 1, wxALL|wxEXPAND, 5);
	Button2 = new wxRadioButton(Panel, ID_RADIOBUTTON2, _("Working-space with\nrectangle PCB outline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	FlexGridSizer2->Add(Button2, 1, wxALL|wxEXPAND, 5);
	Image3 = new wxBitmapButton(Panel, ID_BITMAPBUTTON3, board3_xpm, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTON3"));
	FlexGridSizer2->Add(Image3, 1, wxALL|wxEXPAND, 5);
	Button3 = new wxRadioButton(Panel, ID_RADIOBUTTON3, _("Working-space with\nround PCB outline"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	FlexGridSizer2->Add(Button3, 1, wxALL|wxEXPAND, 5);
	Panel->SetSizer(FlexGridSizer2);
	FlexGridSizer2->Fit(Panel);
	FlexGridSizer2->SetSizeHints(Panel);
	BoxSizer1->Add(Panel, 1, wxALL|wxEXPAND, 5);
	Panel5 = new wxPanel(this, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	FlexGridSizer3 = new wxFlexGridSizer(4, 1, 0, 0);
	Panel1 = new wxPanel(Panel5, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	FlexGridSizer4 = new wxFlexGridSizer(2, 1, 0, 0);
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Size of working-space:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer4->Add(StaticText1, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer5 = new wxFlexGridSizer(2, 3, 0, 0);
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Width:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer5->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SizeW = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE1, _T("160"), wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05, _T("ID_SPINCTRLDOUBLE1"));
	SizeW->SetValue(_T("160"));
	FlexGridSizer5->Add(SizeW, 1, wxALL|wxEXPAND, 0);
	StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer5->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer5->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SizeH = new wxSpinCtrlDouble(Panel1, ID_SPINCTRLDOUBLE2, _T("100"), wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05, _T("ID_SPINCTRLDOUBLE2"));
	SizeH->SetValue(_T("100"));
	FlexGridSizer5->Add(SizeH, 1, wxALL|wxEXPAND, 0);
	StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer5->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxEXPAND, 5);
	Panel1->SetSizer(FlexGridSizer4);
	FlexGridSizer4->Fit(Panel1);
	FlexGridSizer4->SetSizeHints(Panel1);
	FlexGridSizer3->Add(Panel1, 1, wxALL|wxEXPAND, 5);
	Panel2 = new wxPanel(Panel5, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	FlexGridSizer8 = new wxFlexGridSizer(2, 1, 0, 0);
	StaticText7 = new wxStaticText(Panel2, ID_STATICTEXT7, _("Round board outline:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer8->Add(StaticText7, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer10 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText8 = new wxStaticText(Panel2, ID_STATICTEXT8, _("Diameter:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer10->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Diameter = new wxSpinCtrlDouble(Panel2, ID_SPINCTRLDOUBLE3, _T("100"), wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05, _T("ID_SPINCTRLDOUBLE3"));
	Diameter->SetValue(_T("100"));
	FlexGridSizer10->Add(Diameter, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticText9 = new wxStaticText(Panel2, ID_STATICTEXT9, _("mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer10->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8->Add(FlexGridSizer10, 1, wxALL|wxEXPAND, 5);
	Panel2->SetSizer(FlexGridSizer8);
	FlexGridSizer8->Fit(Panel2);
	FlexGridSizer8->SetSizeHints(Panel2);
	FlexGridSizer3->Add(Panel2, 1, wxALL|wxEXPAND, 5);
	Panel3 = new wxPanel(Panel5, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	Panel3->Hide();
	FlexGridSizer6 = new wxFlexGridSizer(2, 1, 0, 0);
	StaticText12 = new wxStaticText(Panel3, ID_STATICTEXT12, _("Distance from working-space:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer6->Add(StaticText12, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer9 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText13 = new wxStaticText(Panel3, ID_STATICTEXT13, _("Distance:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer9->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BorderSize = new wxSpinCtrlDouble(Panel3, ID_SPINCTRLDOUBLE5, _T("20"), wxDefaultPosition, wxDefaultSize, 0, 0, 99.99, 0, 0.05, _T("ID_SPINCTRLDOUBLE5"));
	BorderSize->SetValue(_T("20"));
	FlexGridSizer9->Add(BorderSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 0);
	StaticText14 = new wxStaticText(Panel3, ID_STATICTEXT14, _("mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer9->Add(StaticText14, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer9, 1, wxALL|wxEXPAND, 5);
	Panel3->SetSizer(FlexGridSizer6);
	FlexGridSizer6->Fit(Panel3);
	FlexGridSizer6->SetSizeHints(Panel3);
	FlexGridSizer3->Add(Panel3, 1, wxALL|wxEXPAND, 5);
	Panel4 = new wxPanel(Panel5, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	FlexGridSizer11 = new wxFlexGridSizer(2, 1, 0, 0);
	StaticText10 = new wxStaticText(Panel4, ID_STATICTEXT10, _("Resulting working-space:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer11->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer12 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText11 = new wxStaticText(Panel4, ID_STATICTEXT11, _("Width:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer12->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	AllSizeW = new wxStaticText(Panel4, ID_STATICTEXT15, _("200"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer12->Add(AllSizeW, 1, wxALL|wxEXPAND, 5);
	StaticText16 = new wxStaticText(Panel4, ID_STATICTEXT16, _("mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	FlexGridSizer12->Add(StaticText16, 1, wxALL|wxEXPAND, 5);
	StaticText17 = new wxStaticText(Panel4, ID_STATICTEXT17, _("Height:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	FlexGridSizer12->Add(StaticText17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	AllSizeH = new wxStaticText(Panel4, ID_STATICTEXT18, _("140"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	FlexGridSizer12->Add(AllSizeH, 1, wxALL|wxEXPAND, 5);
	StaticText19 = new wxStaticText(Panel4, ID_STATICTEXT19, _("mm"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	FlexGridSizer12->Add(StaticText19, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer11->Add(FlexGridSizer12, 1, wxALL|wxEXPAND, 5);
	Panel4->SetSizer(FlexGridSizer11);
	FlexGridSizer11->Fit(Panel4);
	FlexGridSizer11->SetSizeHints(Panel4);
	FlexGridSizer3->Add(Panel4, 1, wxALL|wxEXPAND, 5);
	Panel5->SetSizer(FlexGridSizer3);
	FlexGridSizer3->Fit(Panel5);
	FlexGridSizer3->SetSizeHints(Panel5);
	BoxSizer1->Add(Panel5, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer7->Add(StaticText4, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	TextInput = new wxTextCtrl(this, ID_TEXTCTRL1, _("New board"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextInput->SetMaxLength(30);
	FlexGridSizer7->Add(TextInput, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer7, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	FlexGridSizer1->Add(StdDialogButtonSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_BITMAPBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewBoardDialog::OnImage1Click);
	Connect(ID_RADIOBUTTON1,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&NewBoardDialog::OnImage1Click);
	Connect(ID_BITMAPBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewBoardDialog::OnImage2Click);
	Connect(ID_RADIOBUTTON2,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&NewBoardDialog::OnImage2Click);
	Connect(ID_BITMAPBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&NewBoardDialog::OnImage3Click);
	Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&NewBoardDialog::OnImage3Click);
	Connect(ID_SPINCTRLDOUBLE1,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_SPINCTRLDOUBLE2,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_SPINCTRLDOUBLE3,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_SPINCTRLDOUBLE5,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&NewBoardDialog::ChangeText);
	//*)
	SetType(BoardType::Empty);
}

NewBoardDialog::~NewBoardDialog()
{
	//(*Destroy(NewBoardDialog)
	//*)
}

void NewBoardDialog::SetType(BoardType type){
	board.type=type;
	switch(type){
	case BoardType::Empty:
		Button1->SetValue(true);
		Panel1->Show();
		Panel2->Hide();
		Panel3->Hide();
		Panel4->Hide();
		break;
	case BoardType::Rectangle:
		Button2->SetValue(true);
		Panel1->Show();
		Panel2->Hide();
		Panel3->Show();
		Panel4->Show();
		break;
	case BoardType::Round:
		Button3->SetValue(true);
		Panel1->Hide();
		Panel2->Show();
		Panel3->Show();
		Panel4->Show();
		break;
	}
	Layout();
}

void NewBoardDialog::OnImage1Click(wxCommandEvent& event){SetType(BoardType::Empty);}
void NewBoardDialog::OnImage2Click(wxCommandEvent& event){SetType(BoardType::Rectangle);}
void NewBoardDialog::OnImage3Click(wxCommandEvent& event){SetType(BoardType::Round);}

template<typename T>
static string to_str(T value) {
    stringstream s;
    s<<value;
    return s.str();
}
void NewBoardDialog::RecalcSize(wxSpinDoubleEvent& event){
	board.border=BorderSize->GetValue()*10000;
	if(board.type==BoardType::Round){
		board.width=board.height=Diameter->GetValue()*10000;
	}else{
		board.width=SizeW->GetValue()*10000;
		board.height=SizeH->GetValue()*10000;
	}
	AllSizeW->SetLabel(to_str((board.width+board.border*2)/10000.0));
	AllSizeH->SetLabel(to_str((board.height+board.border*2)/10000.0));
	Layout();
}

void NewBoardDialog::ChangeText(wxCommandEvent& event){
	strcpy(board.name,TextInput->GetValue().c_str());
}
