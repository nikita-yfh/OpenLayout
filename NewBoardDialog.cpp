#include "NewBoardDialog.h"
#include <sstream>
#include <vector>
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
#define WIDGET(type,id)\
	static_cast<type>(FindWindowById(id))
	
const long NewBoardDialog::ID_TYPES[3] = {wxNewId(),wxNewId(),wxNewId()};
const long NewBoardDialog::ID_WIDTH = wxNewId();
const long NewBoardDialog::ID_HEIGHT = wxNewId();
const long NewBoardDialog::ID_ALL_WIDTH = wxNewId();
const long NewBoardDialog::ID_ALL_HEIGHT = wxNewId();
const long NewBoardDialog::ID_PANELS[4] = {wxNewId(),wxNewId(),wxNewId(),wxNewId()};
const long NewBoardDialog::ID_DIAMETER = wxNewId();
const long NewBoardDialog::ID_BORDER = wxNewId();
const long NewBoardDialog::ID_INPUT_NAME = wxNewId();

NewBoardDialog::NewBoardDialog(wxWindow* parent)
{

	Create(parent, wxID_ANY, _("New board"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	wxFlexGridSizer *all_box = new wxFlexGridSizer(5, 1, 0, 0);
	{
		wxBoxSizer *content_box = new wxBoxSizer(wxHORIZONTAL);
		{
			wxFlexGridSizer *buttons=new wxFlexGridSizer(3, 2, 0, 0);
			wxString text[3]={
				_("Empty working-space\nwithout PCB outline"),
				_("Working-space with\nrectangle PCB outline"),
				_("Working-space with\nround PCB outline")
			};
			const char**images[]={
				board1_xpm,
				board2_xpm,
				board3_xpm
			};
			auto f1=[&](wxCommandEvent&){SetType(BoardType::Empty);};
			auto f2=[&](wxCommandEvent&){SetType(BoardType::Rectangle);};
			auto f3=[&](wxCommandEvent&){SetType(BoardType::Round);};
			vector<function<void(wxCommandEvent&)>>func={f1,f2,f3};
			for(int q=0;q<3;q++){
				wxBitmapButton *bitmap=new wxBitmapButton(this, wxID_ANY, images[q]);
				bitmap->Bind(wxEVT_BUTTON,func[q]);
				buttons->Add(bitmap,1,wxALL|wxEXPAND);
				wxRadioButton *radio=new wxRadioButton(this,ID_TYPES[q],text[q]);
				radio->Bind(wxEVT_RADIOBUTTON,func[q]);
				buttons->Add(radio,1,wxALL|wxEXPAND);

			}
			wxBoxSizer *panels=new wxBoxSizer(wxVERTICAL);
			{
				wxPanel *panel=new wxPanel(this,ID_PANELS[0]);
				wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);
		
				box->Add(new wxStaticText(panel,wxID_ANY, _("Width: ")), 0, CENTER, 5);
				box->Add(new wxSpinCtrlDouble(panel, ID_WIDTH, "160", wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05), 0, wxALL|wxEXPAND, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("mm")), 0, CENTER, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("Heighth: ")), 0, CENTER, 5);
				box->Add(new wxSpinCtrlDouble(panel, ID_HEIGHT, "100", wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05), 0, wxALL|wxEXPAND, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("mm")), 0, CENTER, 5);
	
				all->Add(new wxStaticText(panel,wxID_ANY, _("Size of working-space:")), 0, LEFT, 5);
				all->Add(box,0,wxALL|wxEXPAND,0);
				panel->SetSizerAndFit(all);
				panels->Add(panel,0,wxALL|wxEXPAND,0);
			}
			{
				wxPanel *panel=new wxPanel(this,ID_PANELS[1]);
				wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);
		
				box->Add(new wxStaticText(panel,wxID_ANY, _("Diameter: ")), 0, CENTER, 5);
				box->Add(new wxSpinCtrlDouble(panel, ID_DIAMETER, "100", wxDefaultPosition, wxDefaultSize, 0, 0, 500, 0, 0.05), 0, wxALL|wxEXPAND, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("mm")), 0, CENTER, 5);
	
				all->Add(new wxStaticText(panel,wxID_ANY, _("Round board outline:")), 0, LEFT, 5);
				all->Add(box,0,wxALL|wxEXPAND,0);
				panel->SetSizerAndFit(all);
				panels->Add(panel,0,wxALL|wxEXPAND,0);
			}
			{
				wxPanel *panel=new wxPanel(this,ID_PANELS[2]);
				wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);
		
				box->Add(new wxStaticText(panel,wxID_ANY, _("Distance: ")), 0, CENTER, 5);
				box->Add(new wxSpinCtrlDouble(panel, ID_BORDER, "20", wxDefaultPosition, wxDefaultSize, 0, 0, 500, 0, 0.05), 0, wxALL|wxEXPAND, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("mm")), 0, CENTER, 5);
	
				all->Add(new wxStaticText(panel,wxID_ANY, _("Distance from working-space:")), 0, LEFT, 5);
				all->Add(box,0,wxALL|wxEXPAND,0);
				panel->SetSizerAndFit(all);
				panels->Add(panel,0,wxALL|wxEXPAND,0);
			}
			{
				wxPanel *panel=new wxPanel(this,ID_PANELS[3]);
				wxBoxSizer *all=new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box=new wxFlexGridSizer(2,3,0,0);
		
				box->Add(new wxStaticText(panel,wxID_ANY, _("Width:")), 0, CENTER, 5);
				box->Add(new wxStaticText(panel,ID_ALL_WIDTH, _("200")), 0, CENTER, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("mm")), 0, CENTER, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("Height:")), 0, CENTER, 5);
				box->Add(new wxStaticText(panel,ID_ALL_HEIGHT, _("140")), 0, CENTER, 5);
				box->Add(new wxStaticText(panel,wxID_ANY, _("mm")), 0, CENTER, 5);
	
				all->Add(new wxStaticText(panel,wxID_ANY, _("Resulting working-space:")), 0, LEFT, 5);
				all->Add(box,0,wxALL|wxEXPAND,0);
				panel->SetSizerAndFit(all);
				panels->Add(panel,0,wxALL|wxEXPAND,0);
			}
			
			panels->Add(-1,-1,1, wxALL|wxEXPAND, 5);
			panels->Add(new wxTextCtrl(this,ID_INPUT_NAME,"New board"),0,wxEXPAND,5);
			content_box->Add(buttons,1,wxALL|wxEXPAND,5);
			content_box->Add(panels,1,wxALL|wxEXPAND,5);
		}
		all_box->Add(content_box,1,wxALL|wxEXPAND,5);

	}

	
	wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
	buttons->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	buttons->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	buttons->Realize();
	all_box->Add(buttons, 1, wxALL|wxEXPAND, 5);
	SetSizerAndFit(all_box);

	Connect(ID_WIDTH,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_HEIGHT,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_DIAMETER,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_BORDER,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&NewBoardDialog::RecalcSize);
	Connect(ID_INPUT_NAME,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&NewBoardDialog::ChangeText);

	SetType(BoardType::Empty);
}

template<typename T>
static string to_str(T value) {
    stringstream s;
    s<<value;
    return s.str();
}
void NewBoardDialog::RecalcSize(wxSpinDoubleEvent& event){
	board.border=WIDGET(wxSpinCtrlDouble*,ID_BORDER)->GetValue()*10000;
	if(board.type==BoardType::Round){
		board.width=board.height=WIDGET(wxSpinCtrlDouble*,ID_DIAMETER)->GetValue()*10000;
	}else{
		board.width=WIDGET(wxSpinCtrlDouble*,ID_WIDTH)->GetValue()*10000;
		board.height=WIDGET(wxSpinCtrlDouble*,ID_HEIGHT)->GetValue()*10000;
	}
	WIDGET(wxStaticText*,ID_ALL_WIDTH)->SetLabel(to_str((board.width+board.border*2)/10000.0));
	WIDGET(wxStaticText*,ID_ALL_HEIGHT)->SetLabel(to_str((board.height+board.border*2)/10000.0));	
	Layout();
}

void NewBoardDialog::ChangeText(wxCommandEvent& event){
	strcpy(board.name,WIDGET(wxTextCtrl*,ID_INPUT_NAME)->GetValue().c_str());
}
void NewBoardDialog::SetType(const BoardType type){
	WIDGET(wxRadioButton*,ID_TYPES[(int)type])->SetValue(true);
	board.type=type;
	switch(type){
	case BoardType::Empty:
		WIDGET(wxPanel*,ID_PANELS[0])->Show();
		WIDGET(wxPanel*,ID_PANELS[1])->Hide();
		WIDGET(wxPanel*,ID_PANELS[2])->Hide();
		WIDGET(wxPanel*,ID_PANELS[3])->Hide();
		break;
	case BoardType::Rectangle:
		WIDGET(wxPanel*,ID_PANELS[0])->Show();
		WIDGET(wxPanel*,ID_PANELS[1])->Hide();
		WIDGET(wxPanel*,ID_PANELS[2])->Show();
		WIDGET(wxPanel*,ID_PANELS[3])->Show();
		break;
	case BoardType::Round:
		WIDGET(wxPanel*,ID_PANELS[0])->Hide();
		WIDGET(wxPanel*,ID_PANELS[1])->Show();
		WIDGET(wxPanel*,ID_PANELS[2])->Hide();				
		WIDGET(wxPanel*,ID_PANELS[3])->Show();
		break;
	}
	Layout();			
}
