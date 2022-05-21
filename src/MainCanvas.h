#pragma once
#include <wx/glcanvas.h>
#include "PCB.h"
#include "Settings.h"

class MainCanvas : public wxGLCanvas {
public:
	MainCanvas(wxWindow *parent, PCB &pcb, Settings &settings);
private:
	PCB &pcb;
	Settings &settings;
	bool shift;
	bool ctrl;
	Vec2 dragPosition;

	Vec2 GetMousePos(const wxMouseEvent&) const;
	Vec2 GetPos(const wxMouseEvent&) const;

	void Draw(wxPaintEvent&);
	void OnKillFocus(wxFocusEvent&);
	void OnMouseWheel(wxMouseEvent&);
	void OnMouseMotion(wxMouseEvent&);
	void OnMiddleDown(wxMouseEvent&);
	void OnLeftDown(wxMouseEvent&);
	void OnLeftUp(wxMouseEvent&);
	void OnRightDown(wxMouseEvent&);
	void OnKey(wxKeyEvent&e);

	wxDECLARE_EVENT_TABLE();
};

