#pragma once
#include "Object.h"

class PolygonBase : public Object {
public:
	PolygonBase();
	~PolygonBase();

	float GetWidth() const;
	void SetWidth(float newWidth);
protected:
	void LoadPoints(File &file, bool onlySoldermask = false);
	void SavePoints(File &file, bool onlySoldermask = false) const;

	Vec2 *points;
	uint32_t count;
	float width;
};


inline float PolygonBase::GetWidth() const {
	return width;
}
inline void PolygonBase::SetWidth(float newWidth) {
	width = newWidth;
}
	

