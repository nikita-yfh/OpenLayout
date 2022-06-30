#include "PolygonBase.h"
#include "GLUtils.h"
#include "Utils.h"

PolygonBase::PolygonBase(uint8_t layer, float width, const Vec2 *array, uint32_t count)
			: LineObject(layer, width), points(array, count), cutout(false) {}

AABB PolygonBase::GetAABB() const {
	Vec2 min( FLT_MAX,  FLT_MAX);
	Vec2 max(-FLT_MAX, -FLT_MAX);
	for(int i = 0; i < points.Size(); i++) {
		max = Vec2::Max(points[i], max);
		min = Vec2::Min(points[i], min);
	}
	return AABB(min, max).Expand(width * 0.5f);
}

Vec2 PolygonBase::GetPosition() const {
	return points[0];
}

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

void PolygonBase::Rotate(const Vec2 &center, float delta) {
	Vec2::Rotate(&points[0], points.Size(), delta, center);
}

void PolygonBase::MirrorHorizontal(float x) {
	for(int i = 0; i < points.Size(); i++)
		points[i].MirrorHorizontal(x);
}

void PolygonBase::MirrorVertical(float y) {
	for(int i = 0; i < points.Size(); i++)
		points[i].MirrorVertical(y);
}

void PolygonBase::Move(const Vec2 &d) {
	for(int i = 0; i < points.Size(); i++)
		points[i] += d;
}



