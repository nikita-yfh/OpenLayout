#include "MainCanvas.h"
#include "LeftPanel.h"
#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"
#include "Circle.h"
#include "Utils.h"

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
		: wxGLCanvas(parent, wxID_ANY, attribList), board(_board), settings(_settings),
		placedPoints(0), bendMode(0), lastPlacedPoint(Vec2::Invalid()), mousePosition(Vec2::Invalid()) {
	SetFocus();
}

void MainCanvas::DrawCrosshair(const Vec2 &position) const {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4ub(255, 255, 255, 255);
	glVertex2f(-1000000.0f, position.y);
	glVertex2f( 1000000.0f, position.y);
	glVertex2f(position.x, -1000000.0f);
	glVertex2f(position.x,  1000000.0f);
	glColor4ub(255, 255, 255, 100);
	glVertex2f(position.x - 1000000.0f, position.y - 1000000.0f);
	glVertex2f(position.x + 1000000.0f, position.y + 1000000.0f);
	glVertex2f(position.x + 1000000.0f, position.y - 1000000.0f);
	glVertex2f(position.x - 1000000.0f, position.y + 1000000.0f);
	glEnd();
	glDisable(GL_BLEND);
}

void MainCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	board->Draw(settings, Vec2(GetSize().x, GetSize().y));
	if(lastPlacedPoint.IsValid())
		DrawCrosshair(mousePosition);
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
		lastPlacedPoint = Vec2::Invalid();
		Refresh();
	}
}

void MainCanvas::OnLeftDown(wxMouseEvent &e) {
	Vec2 mouse = GetPos(e);
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(zoomRatioButtons, GetMousePos(e));
	else if(settings.selectedTool == TOOL_RECTANGLE || settings.selectedTool == TOOL_CIRCLE)
		lastPlacedPoint = board->GetFirstPlaced()->GetPosition();
	else if(settings.selectedTool == TOOL_TRACK || settings.selectedTool == TOOL_ZONE) {
		const Array<Vec2> &points = ((PolygonBase*) board->GetFirstPlaced())->points;
		if(settings.selectedTool == TOOL_ZONE && placedPoints > 1 && (points.First() - points.Last()).LengthSq() < utils::Sq(settings.trackSize / 2.0f)) {
			placedPoints = points.Size() - 1;
			FinishCreating();
		} else {
			lastPlacedPoint = points.Last();
			placedPoints = points.Size();
		}
	} else {
		if(board->GetFirstPlaced()) {
			if(!board->GetFirstPlaced()->groups.Empty())
				settings.selectedTool = TOOL_EDIT;
			board->UnselectAll();
		} else if(settings.selectedTool == TOOL_EDIT) {
			Object *object = board->TestPoint(mouse);
			if(!object || !object->IsSelected() || shift) {
				if(!shift)
					board->UnselectAll();
				if(object)
					board->InvertSelectionGroup(object);
			}
			if(object) {
				mousePosition = lastPlacedPoint = object->GetNearestPoint(mouse);
				mouseDelta = mouse - mousePosition;
			}
		}
	}
	Refresh();
	e.Skip();
}
void MainCanvas::OnLeftUp(wxMouseEvent &e) {
	if(settings.selectedTool == TOOL_RECTANGLE || settings.selectedTool == TOOL_CIRCLE) {
		if(mousePosition == lastPlacedPoint)
			board->CancelPlacing();
		else if(settings.selectedTool == TOOL_RECTANGLE && settings.rectFill && ((PolygonBase*) board->GetFirstPlaced())->points.Size() == 5)
			((PolygonBase*) board->GetFirstPlaced())->points.RemoveLast();
		board->UnselectAll();
	} else if(settings.selectedTool == TOOL_EDIT) {
		lastPlacedPoint = Vec2::Invalid();
		Refresh();
	}
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
	Vec2 mouse = GetPos(e);
	if(settings.selectedTool == TOOL_EDIT && e.LeftIsDown()) {
		if(board->IsSelected()) {
			Vec2 _mouse = board->ToActiveGrid(mouse - mouseDelta, lastPlacedPoint);
			Vec2 delta = _mouse - mousePosition;
			mousePosition = _mouse;
			board->MoveSelected(delta);
		}
	} else if(board->GetFirstPlaced()) {
		if(e.LeftIsDown()) {
			mousePosition = board->ToActiveGrid(mouse);
			if(settings.selectedTool == TOOL_RECTANGLE)
				BuildRect();
			else if(settings.selectedTool == TOOL_CIRCLE)
				BuildCircle();
		} else if(placedPoints) {
			mousePosition = board->ToActiveGrid(mouse);
			BuildTrackEnd();
		} else {
			Vec2 _mouse = board->ToActiveGrid(mouse, lastPlacedPoint);
			Vec2 delta = _mouse - mousePosition;
			mousePosition = _mouse;
			board->MovePlaced(delta);
		}
	} else if(settings.selectedTool >= TOOL_TRACK && settings.selectedTool <= TOOL_ZONE) {
		mousePosition = board->ToActiveGrid(mouse);
		Object *creating = nullptr;
		if(settings.selectedTool == TOOL_THT_PAD)
			creating = new THTPad(board->GetSelectedLayer(), settings.groundDistance, mousePosition, settings.padSize, settings.padShape, settings.metallization);
		else if(settings.selectedTool == TOOL_SMD_PAD)
			creating = new SMDPad(board->GetSelectedLayer(), settings.groundDistance, mousePosition, settings.smdSize);
		else if(settings.selectedTool == TOOL_TRACK || (settings.selectedTool == TOOL_RECTANGLE && !settings.rectFill))
			creating = new Track(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1);
		else if(settings.selectedTool == TOOL_ZONE || (settings.selectedTool == TOOL_RECTANGLE && settings.rectFill))
			creating = new Poly(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1);
		else if(settings.selectedTool == TOOL_CIRCLE)
			creating = new Circle(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, mousePosition, 0.0f, 0.0f, 0.0f);
		if(creating)
			PlaceObject(creating);
	} else
		mousePosition = mouse;
	if(e.MiddleIsDown()) {
		Vec2 delta = mouse - dragPosition;
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

void MainCanvas::BuildRect() {
	Array<Vec2> &points = ((PolygonBase*) board->GetFirstPlaced())->points;
	uint8_t pointCount = 4;
	if(mousePosition == lastPlacedPoint)
		points.Resize(1);
	else if(mousePosition.x == lastPlacedPoint.x || mousePosition.y == lastPlacedPoint.y) {
		points.Resize(2);
		points.Last() = mousePosition;
	} else {
		points.Resize(5);
		points[1].Set(lastPlacedPoint.x, mousePosition.y);
		points[2] = mousePosition;
		points[3].Set(mousePosition.x, lastPlacedPoint.y);
		points[4] = lastPlacedPoint;
	}
	points.First() = lastPlacedPoint;
}

void MainCanvas::BuildCircle() {
	Vec2 delta = (mousePosition - lastPlacedPoint).Abs();
	float radius = utils::Max(delta.x, delta.y);
	((Circle*) board->GetFirstPlaced())->SetDiameter(radius * 2.0f);
}

void MainCanvas::PlaceObject(Object *object) {
	mousePosition = lastPlacedPoint = board->ToGrid(mousePosition);
	board->PlaceObject(object);
}

void MainCanvas::PlaceObjectGroup(const ObjectGroup &objects) {
	lastPlacedPoint = board->ToActiveGrid(mousePosition);
	board->PlaceGroup(objects, mousePosition);
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
