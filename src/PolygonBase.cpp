#include "PolygonBase.h"
#include "GLUtils.h"

PolygonBase::PolygonBase(uint8_t layer, float width, const Vec2 *array, uint32_t count)
			: LineObject(layer, width), points(array, count), cutoff(false) {}

void PolygonBase::LoadPoints(File &file, bool onlySoldermask) {
	uint32_t count = file.Read<uint32_t>() - onlySoldermask;
	points.Init(count);
	for(int i = 0; i < count; i++)
		points[i].LoadPosition(file);
}

void PolygonBase::SavePoints(File &file, bool onlySoldermask) const {
	file.Write<uint32_t>(points.Size() + onlySoldermask);
	for(int i = 0; i < points.Size(); i++)
		points[i].SavePosition(file);
}


