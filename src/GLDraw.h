#pragma once
#include <wx/glcanvas.h>
#include <wx/timer.h>
#include "Settings.h"
#include "PCBFile.h"

enum{
	SEL_NONE,
	SEL_RECT,
	SEL_OBJECT
};

class GLCanvas : public wxGLCanvas{
public:
	GLCanvas(wxWindow *parent);
private:
	void SetColor(const Color &color);
	void SetColor(uint8_t color);
	void SetGroundColor(uint8_t color);
	void SetDrillingsColor();
	void SetLayerColor(const Object &o);

	void DrawGrid(const Board &board);
	void DrawConnections(const Board &board);
	void DrawCircle(Vec2 pos,float r);
	void DrawLine(Vec2 p1,Vec2 p2,float width,bool s1,bool s2);
	void DrawSquare(Vec2 pos,float r,float angle);
	void DrawObject(const Object &o,float d);
	void DrawPad(const Object &o,float d);
	void Draw(wxPaintEvent&);
	void DrawAnchor(const Board &board);
	void DrawSelection(const Board &board);

	void OnMouseWheel(wxMouseEvent&);
	void OnMouseMotion(wxMouseEvent&);
	void OnMiddleDown(wxMouseEvent&e);
	void OnLeftDown(wxMouseEvent&e);
	void OnLeftUp(wxMouseEvent&e);
	void OnKey(wxKeyEvent&e);
	Vec2 GetPos(Vec2 mouse);
	Vec2 GetMousePos(wxMouseEvent&);

	Vec2 clickmousepos;
	Vec2 clickboardpos;

    const float zoom_ratio=1.3f;
    uint8_t selection=SEL_NONE;
	Rect4 sel_rect;

    void OnDragScroll(wxTimerEvent&);
    wxTimer dragscroll_timer;

    void StabilizeCamera();
    bool shift=false;
    bool ctrl=false;
};
