#pragma once
#include <wx/glcanvas.h>
#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/stattext.h>
#include "Settings.h"
#include "PCBFile.h"
#include "Tooltips.h"

enum { //selection
	SEL_NONE,
	SEL_RECT,
	SEL_OBJECT,
	SEL_MEASURE
};

class Canvas : public wxGLCanvas {
public:
	Canvas(wxWindow *parent);
	void UpdateSettings();
protected:
	Vec2 clickmousepos;
	Vec2 clickboardpos;
	Vec2 mousepos;

	const float zoom_ratio=1.3f;
	uint8_t selection=SEL_NONE;
	Rect4 sel_rect;

	void OnDragScroll(wxTimerEvent&);
	wxTimer dragscroll_timer;

	void StabilizeCamera();
	bool shift=false;
	bool ctrl=false;

	void OnKillFocus(wxFocusEvent&);
	void OnMouseWheel(wxMouseEvent&);
	void OnMouseMotion(wxMouseEvent&);
	void OnMiddleDown(wxMouseEvent&e);
	void OnLeftDown(wxMouseEvent&e);
	void OnLeftUp(wxMouseEvent&e);
	void OnKey(wxKeyEvent&e);
	Vec2 GetPos(Vec2i mouse);
	Vec2i GetMousePos(wxMouseEvent&);

	void MoveObjects(Vec2 mouse);
	TextTooltip *tip;
	MeasureTooltip *measure;

};
