#include "MainCanvas.h"

wxBEGIN_EVENT_TABLE(MainCanvas, wxGLCanvas)
	EVT_PAINT(MainCanvas::Draw)
	EVT_CHAR_HOOK(MainCanvas::OnKey)
wxEND_EVENT_TABLE()

static const int attribList[] = {
	WX_GL_RGBA,
	WX_GL_SAMPLE_BUFFERS, GL_FALSE,
	WX_GL_DEPTH_SIZE, 24, 0, 0
};

MainCanvas::MainCanvas(wxWindow *parent, PCB *_pcb, Settings *_settings)
		: wxGLCanvas(parent, wxID_ANY, attribList), pcb(_pcb), settings(_settings) {
	SetFocus();
}

void MainCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);

	const ColorScheme &colors = settings->GetColorScheme();
	wxSize size = GetSize();
	colors.SetClearColor(COLOR_BGR);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, size.x, size.y);

	glLoadIdentity();
	glFlush();
	SwapBuffers();
}

void MainCanvas::OnKey(wxKeyEvent &e) {
	ctrl = e.ControlDown();
	shift = e.ShiftDown();
	if(e.GetKeyCode() >= WXK_NUMPAD1 && e.GetKeyCode() <= WXK_NUMPAD9) {
		pcb->GetSelectedBoard()->SetGrid(settings->gridBind[e.GetKeyCode() - WXK_NUMPAD1]);
		Refresh();
	} else if(e.GetKeyCode() >= '1' && e.GetKeyCode() <= '9') {
		pcb->GetSelectedBoard()->SetGrid(settings->gridBind[e.GetKeyCode() - '1']);
		Refresh();
	}
}

