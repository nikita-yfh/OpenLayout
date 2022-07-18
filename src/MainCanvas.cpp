#include "MainCanvas.h"
#include "LeftPanel.h"

wxBEGIN_EVENT_TABLE(MainCanvas, wxGLCanvas)
	EVT_PAINT(MainCanvas::Draw)
	EVT_KEY_UP(MainCanvas::OnKey)
	EVT_KEY_DOWN(MainCanvas::OnKey)
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
		: wxGLCanvas(parent, wxID_ANY, attribList), board(_board), settings(_settings), placeObjectGroup(false) {
	SetFocus();
}

void MainCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	board->Draw(settings, Vec2(GetSize().x, GetSize().y));
	glFlush();
	SwapBuffers();
}

void MainCanvas::OnKey(wxKeyEvent &e) {
	shift = e.ShiftDown();
	ctrl = e.ControlDown();
	board->UpdateGrid(shift, ctrl);
	if(!shift && !ctrl) {
		if(e.GetKeyCode() >= WXK_NUMPAD1 && e.GetKeyCode() <= WXK_NUMPAD9)
			board->SetGrid(settings.gridBind[e.GetKeyCode() - WXK_NUMPAD1]);
		else if(e.GetKeyCode() >= '1' && e.GetKeyCode() <= '9')
			board->SetGrid(settings.gridBind[e.GetKeyCode() - '1']);
	}
	Refresh();
	e.Skip();
}

void MainCanvas::OnLeftDown(wxMouseEvent &e) {
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(zoomRatioButtons, GetMousePos(e));
	else {
		if(!shift)
			board->UnselectAll();
		if(placeObjectGroup) {
			placeObjectGroup = false;
			settings.selectedTool = TOOL_EDIT;
		} else
			board->SelectObject(GetPos(e));
	}
	Refresh();
	e.Skip();
}
void MainCanvas::OnLeftUp(wxMouseEvent &e) {
	e.Skip();
}
void MainCanvas::OnMiddleDown(wxMouseEvent &e) {
	mousePosition = GetPos(e);
	e.Skip();
}
void MainCanvas::OnRightDown(wxMouseEvent &e) {
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW) {
		board->Zoom(1.0f / zoomRatioButtons, GetMousePos(e));
		Refresh();
	} else if(placeObjectGroup) {
		board->DeleteSelected();
		placeObjectGroup = false;
		settings.selectedTool = TOOL_EDIT;
		Refresh();
	}
	e.Skip();
}
void MainCanvas::OnMouseMotion(wxMouseEvent &e) {
	if(placeObjectGroup) {
		Vec2 mouse = board->ToActiveGrid(GetPos(e));
		Vec2 delta = mouse - placePosition;
		placePosition = mouse;
		board->MoveSelected(delta);
	}
	if(e.MiddleIsDown()) {
		Vec2 delta = GetPos(e) - mousePosition;
		board->UpdateCamera(delta);
	}
	mousePosition = GetPos(e);
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

void MainCanvas::PlaceObjectGroup(const ObjectGroup &objects) {
	placeObjectGroup = true;
	placePosition = board->ToGrid(mousePosition);
	board->AddGroup(objects, placePosition);
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
