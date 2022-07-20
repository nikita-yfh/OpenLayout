#include "MainCanvas.h"
#include "LeftPanel.h"
#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"

wxBEGIN_EVENT_TABLE(MainCanvas, wxGLCanvas)
	EVT_PAINT(MainCanvas::Draw)
	EVT_KEY_DOWN(MainCanvas::OnKeyDown)
	EVT_KEY_UP(MainCanvas::OnKeyUp)
	EVT_LEAVE_WINDOW(MainCanvas::OnLeaveWindow)
	EVT_LEFT_DOWN(MainCanvas::OnLeftDown)
	EVT_LEFT_UP(MainCanvas::OnLeftUp)
	EVT_MIDDLE_DOWN(MainCanvas::OnMiddleDown)
	EVT_RIGHT_DOWN(MainCanvas::OnRightDown)
	EVT_MOTION(MainCanvas::OnMouseMotion)
	EVT_MOUSEWHEEL(MainCanvas::OnMouseWheel)
wxEND_EVENT_TABLE()

static const int attribList[] = {
	WX_GL_RGBA,
	WX_GL_SAMPLE_BUFFERS, GL_TRUE,
	WX_GL_STENCIL_SIZE, 8, 0, 0
};

static const float zoomRatioWheel = 1.3f;
static const float zoomRatioButtons = 1.4f;

MainCanvas::MainCanvas(wxWindow *parent, Board *_board, Settings &_settings)
		: wxGLCanvas(parent, wxID_ANY, attribList), board(_board), settings(_settings), placedPoints(0), bendMode(0), lastPlacedPoint(Vec2::Invalid()) {
	SetFocus();
}

void MainCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	board->Draw(settings, Vec2(GetSize().x, GetSize().y));
	glFlush();
	SwapBuffers();
}

void MainCanvas::OnKeyDown(wxKeyEvent &e) {
	shift = e.ShiftDown();
	ctrl = e.ControlDown();
	board->UpdateGrid(shift, ctrl);
	if(!shift && !ctrl) {
		if(e.GetKeyCode() >= WXK_NUMPAD1 && e.GetKeyCode() <= WXK_NUMPAD9)
			board->SetGrid(settings.gridBind[e.GetKeyCode() - WXK_NUMPAD1]);
		else if(e.GetKeyCode() >= '1' && e.GetKeyCode() <= '9')
			board->SetGrid(settings.gridBind[e.GetKeyCode() - '1']);
		if(e.GetKeyCode() == ' ' && placedPoints) {
			bendMode = (bendMode + 1) % 5;
			BuildTrackEnd();
		}
	}
	Refresh();
	e.Skip();
}

void MainCanvas::OnKeyUp(wxKeyEvent &e) {
	shift = e.ShiftDown();
	ctrl = e.ControlDown();
	board->UpdateGrid(shift, ctrl);
	Refresh();
	e.Skip();
}

void MainCanvas::OnLeaveWindow(wxMouseEvent &e) {
	if(board->GetFirstPlaced() && board->GetFirstPlaced()->groups.Empty() && !placedPoints) {
		board->CancelPlacing();
		Refresh();
	}
}

void MainCanvas::OnLeftDown(wxMouseEvent &e) {
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(zoomRatioButtons, GetMousePos(e));
	else if(settings.selectedTool == TOOL_TRACK || settings.selectedTool == TOOL_ZONE) {
		const Array<Vec2> &points = ((PolygonBase*) board->GetFirstPlaced())->points;
		if(settings.selectedTool == TOOL_ZONE && placedPoints > 1 && points.First() == points.Last())
			FinishCreating();
		else {
			lastPlacedPoint = points.Last();
			placedPoints = points.Size();
		}
	} else {
		if(board->GetFirstPlaced()) {
			if(!board->GetFirstPlaced()->groups.Empty())
				settings.selectedTool = TOOL_EDIT;
			board->UnselectAll();
		} else if(settings.selectedTool == TOOL_EDIT) {
			if(!shift)
				board->UnselectAll();
			board->SelectObject(GetPos(e));
		}
	}
	Refresh();
	e.Skip();
}
void MainCanvas::OnLeftUp(wxMouseEvent &e) {
	e.Skip();
}
void MainCanvas::OnMiddleDown(wxMouseEvent &e) {
	dragPosition = GetPos(e);
	e.Skip();
}
void MainCanvas::OnRightDown(wxMouseEvent &e) {
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(1.0f / zoomRatioButtons, GetMousePos(e));
	else if(settings.selectedTool != TOOL_EDIT)
		FinishCreating();
	Refresh();
	e.Skip();
}
void MainCanvas::OnMouseMotion(wxMouseEvent &e) {
	mousePosition = board->ToActiveGrid(GetPos(e));
	if(board->GetFirstPlaced()) {
		if(placedPoints)
			BuildTrackEnd();
		else {
			Vec2 delta = mousePosition - placePosition;
			placePosition = mousePosition;
			board->MovePlaced(delta);
		}
	} else if(settings.selectedTool >= TOOL_TRACK && settings.selectedTool <= TOOL_ZONE) {
		Object *creating = nullptr;
		if(settings.selectedTool == TOOL_THT_PAD)
			creating = new THTPad(board->GetSelectedLayer(), settings.groundDistance, mousePosition, settings.padSize, settings.padShape, settings.metallization);
		else if(settings.selectedTool == TOOL_SMD_PAD)
			creating = new SMDPad(board->GetSelectedLayer(), settings.groundDistance, mousePosition, settings.smdSize);
		else if(settings.selectedTool == TOOL_TRACK)
			creating = new Track(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1);
		else if(settings.selectedTool == TOOL_ZONE)
			creating = new Poly(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1);
		if(creating) {
			placePosition = mousePosition;
			board->PlaceObject(creating);
		}
	} else
		mousePosition = GetPos(e);
	if(e.MiddleIsDown()) {
		Vec2 delta = GetPos(e) - dragPosition;
		board->UpdateCamera(delta);
	}
	dragPosition = GetPos(e);
	Refresh();
	e.Skip();
}

void MainCanvas::OnMouseWheel(wxMouseEvent &e) {
	float ratio = zoomRatioWheel;
	if(e.GetWheelRotation() < 0)
		ratio = 1.0f / ratio;

	board->Zoom(ratio, GetMousePos(e));

	Refresh();
	e.Skip();
}

void MainCanvas::FinishCreating() {
	if(placedPoints >= 3 || (placedPoints == 2 && settings.selectedTool == TOOL_TRACK)) {
		((PolygonBase*) board->GetFirstPlaced())->points.Resize(placedPoints);
		board->UnselectAll();
	} else {
		if(placedPoints == 0)
			settings.selectedTool = TOOL_EDIT;
		board->CancelPlacing();
		Refresh();
	}
	placedPoints = 0;
	lastPlacedPoint = Vec2::Invalid();
}

void MainCanvas::BuildTrackEnd() {
	Array<Vec2> &points = ((PolygonBase*) board->GetFirstPlaced())->points;
	Vec2 d = (points[placedPoints - 1] - mousePosition).Abs();
	uint8_t newPointsCount = 2;
	if(d.x == 0.0f && d.y == 0.0f)
		newPointsCount = 0;
	else if(d.x == 0.0f || d.y == 0.0f || (abs(d.x - d.y) < 0.00001f && bendMode <= 1) || bendMode == 4)
		newPointsCount = 1;
	points.Resize(placedPoints + newPointsCount);
	if(newPointsCount) {
		points.Last() = mousePosition;
		if(newPointsCount == 2) {
			Vec2 &center = points[placedPoints];
			switch(bendMode) {
			case 0:
				if(d.x > d.y)
					center.Set(mousePosition.x + ((mousePosition.x > lastPlacedPoint.x) ? -d.y : d.y), lastPlacedPoint.y);
				else
					center.Set(lastPlacedPoint.x, mousePosition.y + ((mousePosition.y > lastPlacedPoint.y) ? -d.x : d.x));
				break;
			case 1:
				if(d.x > d.y)
					center.Set(lastPlacedPoint.x + ((mousePosition.x < lastPlacedPoint.x) ? -d.y : d.y), mousePosition.y);
				else
					center.Set(mousePosition.x, lastPlacedPoint.y + ((mousePosition.y < lastPlacedPoint.y) ? -d.x : d.x));
				break;
			case 2: center.Set(mousePosition.x, lastPlacedPoint.y); break;
			case 3: center.Set(lastPlacedPoint.x, mousePosition.y); break;
			}
		}
	}
}

void MainCanvas::PlaceObjectGroup(const ObjectGroup &objects) {
	placePosition = board->ToGrid(mousePosition);
	board->PlaceGroup(objects, placePosition);
	Refresh();
}

Vec2 MainCanvas::GetMousePos(const wxMouseEvent &e) const {
	long mx, my;
	e.GetPosition(&mx, &my);
	return Vec2(mx, my);
}

Vec2 MainCanvas::GetPos(const wxMouseEvent &e) const {
	return board->ConvertToCoords(GetMousePos(e));
}
