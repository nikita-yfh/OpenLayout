#pragma once
#include "Canvas.h"

class GLCanvas : public Canvas{
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
	void DrawDrillings(const Object &o);
	void Draw(wxPaintEvent&);
	void DrawAnchor(const Board &board);
	void DrawSelection(const Board &board);

	const float anchor_size=15.0f;
};
