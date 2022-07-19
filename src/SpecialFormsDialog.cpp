#include "SpecialFormsDialog.h"
#include "GLUtils.h"
#include "SimpleCanvas.h"
#include "Track.h"
#include "Circle.h"
#include "Poly.h"
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>

enum {
	ID_TYPE,
	ID_POLYGON_RADIUS,
	ID_POLYGON_WIDTH,
	ID_POLYGON_CORNERS,
	ID_POLYGON_OFFSET,
	ID_POLYGON_RAYS,
	ID_POLYGON_FILL,
	ID_SPIRAL_RADIUS,
	ID_SPIRAL_DISTANCE,
	ID_SPIRAL_WIDTH,
	ID_SPIRAL_TURNS,
	ID_SPIRAL_ROUND,
	ID_SPIRAL_SQUARE,
	ID_SPIRAL_DIAMETER,
	ID_FRAME_COLUMNS,
	ID_FRAME_COLUMNS_DEF,
	ID_FRAME_COLUMNS_SIDE,
	ID_FRAME_ROWS,
	ID_FRAME_ROWS_DEF,
	ID_FRAME_ROWS_SIDE,
	ID_FRAME_WIDTH,
	ID_FRAME_HEIGHT,
	ID_FRAME_AUTOSIZE
};

enum {
	TYPE_POLYGON,
	TYPE_SPIRAL,
	TYPE_FRAME
};

enum {
	DEF_LETTERS,
	DEF_NUMBERS
};

enum {
	COLUMNS_NO,
	COLUMNS_TOP,
	COLUMNS_BOTTOM,
	COLUMNS_BOTH
};

enum {
	ROWS_NO,
	ROWS_LEFT,
	ROWS_RIGHT,
	ROWS_BOTH
};


wxBEGIN_EVENT_TABLE(SpecialFormsDialog, wxDialog)
	EVT_NOTEBOOK_PAGE_CHANGED(ID_TYPE,			SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_POLYGON_RADIUS,		SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_POLYGON_WIDTH,		SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRL(ID_POLYGON_CORNERS,			SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_POLYGON_OFFSET,		SpecialFormsDialog::UpdatePreview)
	EVT_CHECKBOX(ID_POLYGON_RAYS,				SpecialFormsDialog::UpdatePreview)
	EVT_CHECKBOX(ID_POLYGON_FILL,				SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_SPIRAL_RADIUS,		SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_SPIRAL_DISTANCE,		SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_SPIRAL_WIDTH,			SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_SPIRAL_TURNS,			SpecialFormsDialog::UpdatePreview)
	EVT_RADIOBUTTON(ID_SPIRAL_ROUND,			SpecialFormsDialog::UpdatePreview)
	EVT_RADIOBUTTON(ID_SPIRAL_SQUARE,			SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRL(ID_FRAME_COLUMNS,				SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRL(ID_FRAME_ROWS,					SpecialFormsDialog::UpdatePreview)
	EVT_CHOICE(ID_FRAME_COLUMNS_DEF,			SpecialFormsDialog::UpdatePreview)
	EVT_CHOICE(ID_FRAME_ROWS_DEF,				SpecialFormsDialog::UpdatePreview)
	EVT_CHOICE(ID_FRAME_COLUMNS_SIDE,			SpecialFormsDialog::UpdatePreview)
	EVT_CHOICE(ID_FRAME_ROWS_SIDE,				SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_FRAME_WIDTH,			SpecialFormsDialog::UpdatePreview)
	EVT_SPINCTRLDOUBLE(ID_FRAME_HEIGHT,			SpecialFormsDialog::UpdatePreview)
	EVT_BUTTON(ID_FRAME_AUTOSIZE,				SpecialFormsDialog::AutosizeFrame)
wxEND_EVENT_TABLE()

SpecialFormsDialog::SpecialFormsDialog(wxWindow *parent, const Settings &_settings, const Vec2 &_boardSize, uint8_t _layer)
			: wxDialog(parent, wxID_ANY, _("Special forms")), settings(_settings), boardSize(_boardSize), layer(_layer) {
	canvas = new SimpleCanvas(this, &objects, settings);
	wxNotebook *notebook = new wxNotebook(this, ID_TYPE);

	{
		wxPanel *panel = new wxPanel(notebook);
		wxFlexGridSizer *sizer = new wxFlexGridSizer(5, 3, 5, 5);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrlDouble(panel, ID_POLYGON_RADIUS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1.0, 249.9, 12.0, 0.5), 0, wxEXPAND);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Line width:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrlDouble(panel, ID_POLYGON_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 99.99, 0.4, 0.05), 0, wxEXPAND);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Number of corners:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrl(panel, ID_POLYGON_CORNERS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 3, 99, 6), 0, wxEXPAND);
		sizer->Add(0, 0);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _(L"Angle offset (\x00b0):")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrlDouble(panel, ID_POLYGON_OFFSET, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -180.0, 180.0, 0.0, 0.5), 0, wxEXPAND);
		sizer->Add(0, 0);

		sizer->Add(new wxCheckBox(panel, ID_POLYGON_RAYS, _("Rays")), 0, wxEXPAND | wxLEFT, 10);
		sizer->Add(new wxCheckBox(panel, ID_POLYGON_FILL, _("Filled")), 0, wxEXPAND | wxLEFT, 10);

		panel->SetSizer(sizer);
		notebook->AddPage(panel, _("Polygon"));
	}
	{
		wxPanel *panel = new wxPanel(notebook);
		wxFlexGridSizer *sizer = new wxFlexGridSizer(6, 3, 5, 5);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Start radius:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrlDouble(panel, ID_SPIRAL_RADIUS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.1, 249.9, 2.0, 0.5), 0, wxEXPAND);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Distance:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrlDouble(panel, ID_SPIRAL_DISTANCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.1, 99.9, 2.0, 0.1), 0, wxEXPAND);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Track width:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxSpinCtrlDouble(panel, ID_SPIRAL_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.0, 99.99, 0.4, 0.05), 0, wxEXPAND);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Turns:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		wxSpinCtrlDouble *spiralTurns = new wxSpinCtrlDouble(panel, ID_SPIRAL_TURNS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1.0, 100.0, 6.0, 0.25);
		spiralTurns->SetDigits(2);
		sizer->Add(spiralTurns, 0, wxEXPAND);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		sizer->Add(new wxRadioButton(panel, ID_SPIRAL_ROUND, _("Round")), 0, wxEXPAND | wxLEFT, 10);
		sizer->Add(new wxRadioButton(panel, ID_SPIRAL_SQUARE, _("Square")), 0, wxEXPAND | wxLEFT, 10);
		sizer->Add(0, 0);

		sizer->Add(new wxStaticText(panel, wxID_ANY, _("Diameter:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxLEFT, 10);
		sizer->Add(new wxStaticText(panel, ID_SPIRAL_DIAMETER, wxEmptyString), 0, wxALIGN_CENTER);
		sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

		panel->SetSizer(sizer);
		notebook->AddPage(panel, _("Spiral"));
	}
	{
		wxPanel *panel = new wxPanel(notebook);
		wxBoxSizer *vsizer = new wxBoxSizer(wxVERTICAL);

		{
			wxFlexGridSizer *sizer = new wxFlexGridSizer(5, 2, 5, 5);

			const wxString frameDefs[] = {"A, B, ...", "1, 2, ..."};
			const wxString frameColumnsSides[] = {"...", _("Top"), _("Bottom"), _("Top + Bottom")};
			const wxString frameRowsSides[] = {"...", _("Left"), _("Right"), _("Left + Right")};

			sizer->Add(new wxStaticText(panel, wxID_ANY, _("Columns:")), 0, wxALIGN_CENTER_VERTICAL);
			sizer->Add(new wxSpinCtrl(panel, ID_FRAME_COLUMNS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -99, 99, 8), 0, wxEXPAND);
			wxChoice *frameColumnsDef = new wxChoice(panel, ID_FRAME_COLUMNS_DEF, wxDefaultPosition, wxDefaultSize, 2, frameDefs);
			frameColumnsDef->SetSelection(0);
			sizer->Add(frameColumnsDef, 0, wxEXPAND);
			wxChoice *frameColumnsSide = new wxChoice(panel, ID_FRAME_COLUMNS_SIDE, wxDefaultPosition, wxDefaultSize, 4, frameColumnsSides);
			frameColumnsSide->SetSelection(1);
			sizer->Add(frameColumnsSide, 0, wxEXPAND);

			sizer->Add(0, 10);
			sizer->Add(0, 10);

			sizer->Add(new wxStaticText(panel, wxID_ANY, _("Rows:")), 0, wxALIGN_CENTER_VERTICAL);
			sizer->Add(new wxSpinCtrl(panel, ID_FRAME_ROWS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -99, 99, 8), 0, wxEXPAND);
			wxChoice *frameRowsDef = new wxChoice(panel, ID_FRAME_ROWS_DEF, wxDefaultPosition, wxDefaultSize, 2, frameDefs);
			frameRowsDef->SetSelection(1);
			sizer->Add(frameRowsDef, 0, wxEXPAND);
			wxChoice *frameRowsSide = new wxChoice(panel, ID_FRAME_ROWS_SIDE, wxDefaultPosition, wxDefaultSize, 4, frameRowsSides);
			frameRowsSide->SetSelection(1);
			sizer->Add(frameRowsSide, 0, wxEXPAND);

			vsizer->Add(sizer, 0, wxALL, 10);
		}
		{
			wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 3, 5, 5);

			sizer->Add(new wxStaticText(panel, wxID_ANY, _("Width:")), 0, wxALIGN_CENTER_VERTICAL);
			sizer->Add(new wxSpinCtrlDouble(panel, ID_FRAME_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10.0, 999.0, 90.0, 0.5), 0, wxEXPAND);
			sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL);

			sizer->Add(new wxStaticText(panel, wxID_ANY, _("Height:")), 0, wxALIGN_CENTER_VERTICAL);
			sizer->Add(new wxSpinCtrlDouble(panel, ID_FRAME_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10.0, 999.0, 40.0, 0.5), 0, wxEXPAND);
			sizer->Add(new wxStaticText(panel, wxID_ANY, _("mm")), 0, wxALIGN_CENTER_VERTICAL);

			vsizer->Add(sizer, 0, wxALL, 10);
		}

		vsizer->Add(new wxButton(panel, ID_FRAME_AUTOSIZE, _("Autosize")), 0, wxEXPAND | wxALL, 5);

		panel->SetSizer(vsizer);
		notebook->AddPage(panel, _("Frame"));
	}
	wxBoxSizer *content = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sideBox = new wxBoxSizer(wxVERTICAL);
	sideBox->Add(notebook, 1, wxEXPAND | wxALL, 5);
	sideBox->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);
	content->Add(sideBox, 2);
	content->Add(canvas, 3, wxEXPAND | wxALL, 5);
	SetSizerAndFit(content);
}

void SpecialFormsDialog::AutosizeFrame(wxCommandEvent&) {
	wxSpinCtrlDouble *width = (wxSpinCtrlDouble*) FindWindowById(ID_FRAME_WIDTH);
	wxSpinCtrlDouble *height = (wxSpinCtrlDouble*) FindWindowById(ID_FRAME_HEIGHT);
	width->SetValue(boardSize.x - 5.1f);
	height->SetValue(boardSize.y - 5.1f);
}

void SpecialFormsDialog::UpdatePreview(wxSpinDoubleEvent&) { UpdatePreview(); }
void SpecialFormsDialog::UpdatePreview(wxSpinEvent&) { UpdatePreview(); }
void SpecialFormsDialog::UpdatePreview(wxBookCtrlEvent&) { UpdatePreview(); }
void SpecialFormsDialog::UpdatePreview(wxCommandEvent&) { UpdatePreview(); }

void SpecialFormsDialog::UpdatePreview() {
	uint8_t type = ((wxNotebook*) FindWindowById(ID_TYPE))->GetSelection();
	objects.~ObjectGroup();
	if(type == TYPE_POLYGON) {
		float radius = ((wxSpinCtrlDouble*) FindWindowById(ID_POLYGON_RADIUS))->GetValue();
		float width = ((wxSpinCtrlDouble*) FindWindowById(ID_POLYGON_WIDTH))->GetValue();
		uint8_t corners = ((wxSpinCtrl*) FindWindowById(ID_POLYGON_CORNERS))->GetValue();
		float offset = glutils::DegToRad(((wxSpinCtrlDouble*) FindWindowById(ID_POLYGON_OFFSET))->GetValue());

		Vec2 points[corners + 1];
		for(int corner = 0; corner < corners; corner++) {
			float angle = offset + corner * M_PI * 2.0f / corners;
			points[corner] = Vec2(angle) * radius;
		}

		if(((wxCheckBox*) FindWindowById(ID_POLYGON_FILL))->IsChecked()) // Filled polygon
			objects.AddObjectEnd(new Poly(layer, settings.groundDistance, width, points, corners));
		else {
			points[corners] = points[0]; // Loop
			objects.AddObjectBegin(new Track(layer, settings.groundDistance, width, points, corners + 1));
			if(((wxCheckBox*) FindWindowById(ID_POLYGON_RAYS))->IsChecked()) { // Add rays
				for(int corner = 0; corner < corners; corner++) {
					Vec2 line[2] = {Vec2(0.0f, 0.0f), points[corner]};
					objects.AddObjectBegin(new Track(layer, settings.groundDistance, width, line, 2));
				}
			}
		}
	} else if(type == TYPE_SPIRAL) {
		float startRadius = ((wxSpinCtrlDouble*) FindWindowById(ID_SPIRAL_RADIUS))->GetValue();
		float distance = ((wxSpinCtrlDouble*) FindWindowById(ID_SPIRAL_DISTANCE))->GetValue();
		float width = ((wxSpinCtrlDouble*) FindWindowById(ID_SPIRAL_WIDTH))->GetValue();
		int qturns = ((wxSpinCtrlDouble*) FindWindowById(ID_SPIRAL_TURNS))->GetValue() * 4;

		float endDiameter;
		if(((wxRadioButton*) FindWindowById(ID_SPIRAL_SQUARE))->GetValue()) { // Square spiral
			Vec2 points[qturns + 1];
			points[0].SetZero();
			float delta = (distance + width) / 2.0f;
			if(startRadius <= delta / 2.0f)
				startRadius = delta / 2.0f + 0.1f;
			float length = startRadius * 2.0f - delta;
			for(int qturn = 0; qturn < qturns; qturn++) {
				points[qturn + 1] = points[qturn];
				if(qturn == qturns - 1) // End segment
					length -= delta * 2.0f;
				switch(qturn % 4) { // Direction
				case 0: points[qturn + 1].x -= length; break;
				case 1: points[qturn + 1].y -= length; break;
				case 2: points[qturn + 1].x += length; break;
				case 3: points[qturn + 1].y += length; break;
				}
				length += delta;
			}
			objects.AddObjectBegin(new Track(layer, settings.groundDistance, width, points, qturns + 1));
			endDiameter = length + width;
		} else { // Round spiral
			float delta = (distance + width) / 4.0f;
			if(startRadius <= delta * 2.0f)
				startRadius = delta * 2.0f + 0.1f;
			float radius = startRadius / 2.0f - delta;
			Vec2 centers[4] = {
				Vec2(-delta / 2.0f, -delta / 2.0f),
				Vec2(-delta / 2.0f,  delta / 2.0f),
				Vec2( delta / 2.0f,  delta / 2.0f),
				Vec2( delta / 2.0f, -delta / 2.0f)
			};
			for(int qturn = 0; qturn < qturns; qturn++) {
				float beginAngle = glutils::AngleMod(M_PI / 2.0f * qturn);
				float endAngle = glutils::AngleMod(M_PI / 2.0f * (qturn + 1));
				objects.AddObjectBegin(new Circle(layer, settings.groundDistance, width,
					centers[qturn % 4], radius * 2.0f, beginAngle, endAngle));
				radius += delta;
			}
			endDiameter = 2.0f * radius - 3.0f * delta + width;
		}
		((wxSpinCtrlDouble*) FindWindowById(ID_SPIRAL_RADIUS))->SetValue(startRadius);
		((wxStaticText*) FindWindowById(ID_SPIRAL_DIAMETER))->SetLabel(wxString::Format("%.3f", endDiameter));
	} else { // TYPE_FRAME
		uint8_t frameColumns = ((wxSpinCtrl*) FindWindowById(ID_FRAME_COLUMNS))->GetValue();
		uint8_t frameRows = ((wxSpinCtrl*) FindWindowById(ID_FRAME_COLUMNS))->GetValue();
		uint8_t frameColumnsDef = ((wxChoice*) FindWindowById(ID_FRAME_COLUMNS_DEF))->GetSelection();
		uint8_t frameRowsDef = ((wxChoice*) FindWindowById(ID_FRAME_ROWS_DEF))->GetSelection();
		uint8_t frameColumnsSide = ((wxChoice*) FindWindowById(ID_FRAME_COLUMNS_SIDE))->GetSelection();
		uint8_t frameRowsSide = ((wxChoice*) FindWindowById(ID_FRAME_ROWS_SIDE))->GetSelection();
		Vec2 frameSize(
			((wxSpinCtrlDouble*) FindWindowById(ID_FRAME_WIDTH))->GetValue(),
			((wxSpinCtrlDouble*) FindWindowById(ID_FRAME_HEIGHT))->GetValue()
		);
	}
	canvas->Refresh();
}

