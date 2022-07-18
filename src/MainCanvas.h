#pragma once
#include <wx/glcanvas.h>
#include "Board.h"
#include "Settings.h"

class MainCanvas : public wxGLCanvas {
public:
	MainCanvas(wxWindow *parent, Board *board, Settings &settings);
	void PlaceObjectGroup(const ObjectGroup &objects);
private:
	Board *board;
	Settings &settings;
	bool shift;
	bool ctrl;
	Vec2 mousePosition;
	Vec2 placePosition;
	bool placeObjectGroup;

	Vec2 GetMousePos(const wxMouseEvent&) const;
	Vec2 GetPos(const wxMouseEvent&) const;

	void SwitchEditTool();

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
