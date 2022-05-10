#pragma once
#include <wx/glcanvas.h>
#include "PCB.h"
#include "Settings.h"

class MainCanvas : public wxGLCanvas {
public:
	MainCanvas(wxWindow *parent, PCB *pcb, Settings *settings);
private:
	PCB *pcb;
	Settings *settings;
	bool shift;
	bool ctrl;

	void Draw(wxPaintEvent&);
	void OnKillFocus(wxFocusEvent&);
	void OnMouseWheel(wxMouseEvent&);
	void OnMouseMotion(wxMouseEvent&);
	void OnMiddleDown(wxMouseEvent&e);
	void OnLeftDown(wxMouseEvent&e);
	void OnLeftUp(wxMouseEvent&e);
	void OnKey(wxKeyEvent&e);

	wxDECLARE_EVENT_TABLE();
};

