#include "MainCanvas.h"

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

static const float zoomRatio = 1.3f;

MainCanvas::MainCanvas(wxWindow *parent, Board *_board, Settings &_settings)
		: wxGLCanvas(parent, wxID_ANY, attribList), board(_board), settings(_settings) {
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
	if(!shift)
		board->UnselectAll();
	board->SelectObject(GetPos(e));
	Refresh();
}
void MainCanvas::OnLeftUp(wxMouseEvent &e) {
}
void MainCanvas::OnMiddleDown(wxMouseEvent &e) {
	dragPosition = GetPos(e);
}
void MainCanvas::OnRightDown(wxMouseEvent &e) {
}
void MainCanvas::OnMouseMotion(wxMouseEvent &e) {
	Vec2 delta = dragPosition - GetPos(e);
	if(e.MiddleIsDown()) {
		board->UpdateCamera(delta);
		Refresh();
	}
	dragPosition = GetPos(e);
}
void MainCanvas::OnMouseWheel(wxMouseEvent &e) {
	float ratio = zoomRatio;
	if(e.GetWheelRotation() < 0)
		ratio = 1.0f / ratio;

	board->Zoom(ratio, GetMousePos(e));

	Refresh();
	e.Skip();
}

Vec2 MainCanvas::GetMousePos(const wxMouseEvent &e) const {
	long mx, my;
	e.GetPosition(&mx, &my);
	return Vec2(mx, my);
}

Vec2 MainCanvas::GetPos(const wxMouseEvent &e) const {
	return board->ConvertToCoords(GetMousePos(e));
}
