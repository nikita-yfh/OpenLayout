#include "NewBoardDialog.h"

#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>

extern "C" {
#include "newboard/empty.xpm"
#include "newboard/rectangle.xpm"
#include "newboard/round.xpm"
}

NewBoardDialog::NewBoardDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, _("New board"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER) {
	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer *content = new wxBoxSizer(wxHORIZONTAL);
		{
			wxFlexGridSizer *buttons = new wxFlexGridSizer(3, 2, 10, 10);
			wxString text[3]= {
				_("Empty working-space\nwithout PCB outline"),
				_("Working-space with\nrectangle PCB outline"),
				_("Working-space with\nround PCB outline")
			};
			char ** images[]= {
				empty_xpm,
				rectangle_xpm,
				round_xpm
			};

			wxBitmapButton *bitmaps[3];
			for(int q = 0; q < 3; q++){
				types[q] = new wxRadioButton(this, wxID_ANY, text[q]);
				bitmaps[q] = new wxBitmapButton(this, wxID_ANY, images[q]);
				buttons->Add(bitmaps[q], 1, wxALL|wxEXPAND);
				buttons->Add(types[q], 1, wxALL|wxEXPAND);
			}

			bitmaps[0]->Bind(wxEVT_BUTTON, &NewBoardDialog::SetEmpty, this);
			bitmaps[1]->Bind(wxEVT_BUTTON, &NewBoardDialog::SetRectangle, this);
			bitmaps[2]->Bind(wxEVT_BUTTON, &NewBoardDialog::SetRound, this);
			types[0]->Bind(wxEVT_RADIOBUTTON, &NewBoardDialog::SetEmpty, this);
			types[1]->Bind(wxEVT_RADIOBUTTON, &NewBoardDialog::SetRectangle, this);
			types[2]->Bind(wxEVT_RADIOBUTTON, &NewBoardDialog::SetRound, this);

			wxBoxSizer *panelBox = new wxBoxSizer(wxVERTICAL);
			{
				panels[0] = new wxPanel(this);
				wxBoxSizer *all = new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box = new wxFlexGridSizer(2, 3, 3, 20);

				box->Add(new wxStaticText(panels[0], wxID_ANY, _("Width:")), 0, wxALIGN_CENTER);
				width = new wxSpinCtrlDouble(panels[0], wxID_ANY, "160", wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05);
				width->Bind(wxEVT_SPINCTRLDOUBLE, &NewBoardDialog::RecalcSize, this);
				box->Add(width, wxEXPAND);
				box->Add(new wxStaticText(panels[0], wxID_ANY, _("mm")), 0, wxALIGN_CENTER);
				box->Add(new wxStaticText(panels[0], wxID_ANY, _("Heigth:")), 0, wxALIGN_CENTER);
				height = new wxSpinCtrlDouble(panels[0], wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, 0, 1, 500, 0, 0.05);
				height->Bind(wxEVT_SPINCTRLDOUBLE, &NewBoardDialog::RecalcSize, this);
				box->Add(height, wxEXPAND);
				box->Add(new wxStaticText(panels[0], wxID_ANY, _("mm")), 0, wxALIGN_CENTER);

				all->Add(new wxStaticText(panels[0], wxID_ANY, _("Size of working-space:")), wxALIGN_LEFT);
				all->Add(box, wxEXPAND);
				panels[0]->SetSizerAndFit(all);
				panelBox->Add(panels[0], 0, wxALL|wxEXPAND, 10);
			}
			{
				panels[1] = new wxPanel(this);
				wxBoxSizer *all = new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box = new wxFlexGridSizer(3, 0, 20);

				box->Add(new wxStaticText(panels[1], wxID_ANY, _("Diameter:")), 0, wxALIGN_CENTER);
				diameter = new wxSpinCtrlDouble(panels[1], wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, 0, 0, 500, 0, 0.05);
				diameter->Bind(wxEVT_SPINCTRLDOUBLE, &NewBoardDialog::RecalcSize, this);
				box->Add(diameter, wxEXPAND);
				box->Add(new wxStaticText(panels[1], wxID_ANY, _("mm")), 0, wxALIGN_CENTER);

				all->Add(new wxStaticText(panels[1], wxID_ANY, _("Round outline:")), wxALIGN_LEFT);
				all->Add(box, wxEXPAND);
				panels[1]->SetSizerAndFit(all);
				panelBox->Add(panels[1], 0, wxALL|wxEXPAND, 10);
			}
			{
				panels[2] = new wxPanel(this);
				wxBoxSizer *all = new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box = new wxFlexGridSizer(3, 0, 20);

				box->Add(new wxStaticText(panels[2], wxID_ANY, _("Distance:")), 0, wxALIGN_CENTER);
				border = new wxSpinCtrlDouble(panels[2], wxID_ANY, "20", wxDefaultPosition, wxDefaultSize, 0, 0, 500, 0, 0.05);
				border->Bind(wxEVT_SPINCTRLDOUBLE, &NewBoardDialog::RecalcSize, this);
				box->Add(border, 0, wxALL|wxEXPAND);
				box->Add(new wxStaticText(panels[2], wxID_ANY, _("mm")), 0, wxALIGN_CENTER);

				all->Add(new wxStaticText(panels[2], wxID_ANY, _("Distance from working-space:")), wxALIGN_LEFT);
				all->Add(box, wxEXPAND);
				panels[2]->SetSizerAndFit(all);
				panelBox->Add(panels[2], 0, wxALL|wxEXPAND, 10);
			}
			{
				panels[3] = new wxPanel(this);
				wxBoxSizer *all = new wxBoxSizer(wxVERTICAL);
				wxFlexGridSizer *box = new wxFlexGridSizer(2, 3, 3, 20);

				box->Add(new wxStaticText(panels[3], wxID_ANY, _("Width:")), 0, wxALIGN_CENTER);
				allWidth = new wxStaticText(panels[3], wxID_ANY, wxEmptyString);
				box->Add(allWidth, 0, wxALIGN_CENTER);
				box->Add(new wxStaticText(panels[3], wxID_ANY, _("mm")), 0, wxALIGN_CENTER);
				box->Add(new wxStaticText(panels[3], wxID_ANY, _("Height:")), 0, wxALIGN_CENTER);
				allHeight = new wxStaticText(panels[3], wxID_ANY, wxEmptyString);
				box->Add(allHeight, 0, wxALIGN_CENTER);
				box->Add(new wxStaticText(panels[3], wxID_ANY, _("mm")), 0, wxALIGN_CENTER);

				all->Add(new wxStaticText(panels[3], wxID_ANY, _("Resulting working-space:")), wxALIGN_LEFT);
				all->Add(box, wxEXPAND);
				panels[3]->SetSizerAndFit(all);
				panelBox->Add(panels[3], 0, wxALL|wxEXPAND, 10);
			}

			panelBox->Add(-1, -1, 1, wxALL|wxEXPAND, 5);
			name = new wxTextCtrl(this, wxID_ANY, "New board");
			panelBox->Add(name, 0, wxEXPAND, 5);
			content->Add(buttons, 1, wxALL|wxEXPAND, 5);
			content->Add(panelBox, 1, wxALL|wxEXPAND, 5);
		}
		content->Add(content, 1, wxALL|wxEXPAND, 5);
	}


	content->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);
	SetSizerAndFit(content);
	SetAutoLayout(true);
	panels[1]->Hide();
	panels[2]->Hide();
	panels[3]->Hide();
}

void NewBoardDialog::SetEmpty(wxCommandEvent &e) {
	types[0]->SetValue(true);
	panels[0]->Show();
	panels[1]->Hide();
	panels[2]->Hide();
	panels[3]->Hide();
	Layout();
}
void NewBoardDialog::SetRectangle(wxCommandEvent &e) {
	types[1]->SetValue(true);
	panels[0]->Show();
	panels[1]->Hide();
	panels[2]->Show();
	panels[3]->Show();
	RecalcSize(e);
	Layout();
}
void NewBoardDialog::SetRound(wxCommandEvent &e) {
	types[2]->SetValue(true);
	panels[0]->Hide();
	panels[1]->Show();
	panels[2]->Show();
	panels[3]->Show();
	RecalcSize(e);
	Layout();
}
void NewBoardDialog::RecalcSize(wxCommandEvent&) {
	float newWidth;
	float newHeight;

	if(!types[2]->GetValue()){
		newWidth = width->GetValue();
		newHeight = height->GetValue();
	}else
		newWidth = newHeight = diameter->GetValue();

	if(!types[0]->GetValue()){
		newWidth += border->GetValue() * 2.0f;
		newHeight += border->GetValue() * 2.0f;
	}

	allWidth->SetLabel(wxString::Format("%.2f", newWidth));
	allHeight->SetLabel(wxString::Format("%.2f", newHeight));
}

Board *NewBoardDialog::CreateBoard(bool originTop) const {
	Vec2 boardSize;
	if(!types[2]->GetValue()){
		boardSize.x = width->GetValue();
		boardSize.y = height->GetValue();
	}else
		boardSize.x = boardSize.y = diameter->GetValue();
	float boardBorder = border->GetValue();

	Board::Type boardType(Board::Type::Empty);
	if(types[1]->GetValue())
		boardType = Board::Type::Rectangle;
	else if(types[2]->GetValue())
		boardType = Board::Type::Round;

	return new Board(name->GetValue().c_str(), boardType, boardSize, boardBorder, originTop);
}




