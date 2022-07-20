#pragma once
#include <wx/glcanvas.h>
#include "Board.h"
#include "Settings.h"

class MainCanvas : public wxGLCanvas {
public:
	MainCanvas(wxWindow *parent, Board *board, Settings &settings);
	void PlaceObjectGroup(const ObjectGroup &objects);
	void PlaceObject(Object *object);

	const Vec2 &GetMousePosition() const;
private:
	Board *board;
	Settings &settings;
	bool shift;
	bool ctrl;
	Vec2 dragPosition;
	Vec2 mousePosition;
	Vec2 placePosition;
	Vec2 lastPlacedPoint;
	uint32_t placedPoints;

	uint8_t bendMode;
	void BuildTrackEnd();
	void BuildRect();
	void FinishCreating();

	Vec2 GetMousePos(const wxMouseEvent&) const;
	Vec2 GetPos(const wxMouseEvent&) const;

	void Draw(wxPaintEvent&);
	void OnLeaveWindow(wxMouseEvent&);
	void OnMouseWheel(wxMouseEvent&);
	void OnMouseMotion(wxMouseEvent&);
	void OnMiddleDown(wxMouseEvent&);
	void OnLeftDown(wxMouseEvent&);
	void OnLeftUp(wxMouseEvent&);
	void OnRightDown(wxMouseEvent&);
	void OnKeyDown(wxKeyEvent&e);
	void OnKeyUp(wxKeyEvent&e);

	wxDECLARE_EVENT_TABLE();
};

inline const Vec2 &MainCanvas::GetMousePosition() const {
	return mousePosition;
}
