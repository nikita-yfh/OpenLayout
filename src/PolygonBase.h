#pragma once
#include "LineObject.h"

class PolygonBase : public LineObject {
public:
	PolygonBase();
	PolygonBase(uint8_t layer, float width, const Vec2 *points, uint32_t count);

	~PolygonBase();
protected:
	void LoadPoints(File &file, bool onlySoldermask = false);
	void SavePoints(File &file, bool onlySoldermask = false) const;

	Vec2 *points;
	uint32_t count;
	bool cutoff;

	friend class Board;
};
