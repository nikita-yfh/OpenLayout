#include "PolygonBase.h"

PolygonBase::PolygonBase() {
	points = nullptr;
	count = 0;
}

PolygonBase::~PolygonBase() {
	if(count)
		free(points);
}

void PolygonBase::LoadPoints(File &file, bool onlySoldermask) {
	count = file.Read<uint32_t>() - onlySoldermask;
	points = (Vec2*) malloc(sizeof(Vec2) * count);
	for(int i = 0; i < count; i++)
		points[i].Load(file);
}

void PolygonBase::SavePoints(File &file, bool onlySoldermask) const {
	file.Write<uint32_t>(count + onlySoldermask);
	for(int i = 0; i < count; i++)
		points[i].Save(file);
}


