#include "PolygonBase.h"
#include "GLUtils.h"

PolygonBase::PolygonBase(uint8_t layer, float width, const Vec2 *_points, uint32_t _count)
						: LineObject(layer, width) {
	cutoff = false;
	count = _count;
	if(count) {
		points = (Vec2*) malloc(sizeof(Vec2) * count);
		memcpy(points, _points, sizeof(Vec2) * count);
	} else
		points = nullptr;
}

PolygonBase::~PolygonBase() {
	if(count)
		free(points);
}

void PolygonBase::LoadPoints(File &file, bool onlySoldermask) {
	count = file.Read<uint32_t>() - onlySoldermask;
	points = (Vec2*) malloc(sizeof(Vec2) * count);
	for(int i = 0; i < count; i++)
		points[i].LoadPosition(file);
}

void PolygonBase::SavePoints(File &file, bool onlySoldermask) const {
	file.Write<uint32_t>(count + onlySoldermask);
	for(int i = 0; i < count; i++)
		points[i].SavePosition(file);
}


