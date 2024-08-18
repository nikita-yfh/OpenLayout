#include "PolygonBase.h"
#include "GLUtils.h"
#include "Utils.h"

int PolygonBase::bendMode = BEND_45_1;

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

Vec2 PolygonBase::GetNearestPoint(const Vec2 &point) const {
	Vec2 nearestPoint;
	float minDistanceSq = FLT_MAX;
	for(int i = 0; i < points.Size(); i++) {
		float distance = (point - points[i]).LengthSq();
		if(distance < minDistanceSq) {
			nearestPoint = points[i];
			minDistanceSq = distance;
		}
	}
	return nearestPoint;
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

int PolygonBase::GetPlacePointCount(const Vec2 &d){
	if(d.x < FLT_EPSILON || d.y < FLT_EPSILON)
        return 0;
	else if(d.x < FLT_EPSILON || d.y < FLT_EPSILON)
        return 1;
    else if(abs(d.x - d.y) < 0.00001f && (bendMode == BEND_45_1 || bendMode == BEND_45_2))
        return 1;
    else if(bendMode == BEND_DIRECT)
        return 1;
    return 2;
}

void PolygonBase::BuildTrackEnd(const Vec2 &mousePosition, int placedPointCount) {
    const Vec2 &lastPlacedPoint = points[placedPointCount - 1];
	Vec2 d = (lastPlacedPoint - mousePosition).Abs();
	uint8_t newPointsCount = GetPlacePointCount(d);
	points.Resize(placedPointCount + newPointsCount);
	if(newPointsCount) {
		points.Last() = mousePosition;
		if(newPointsCount == 2) {
			Vec2 &center = points[placedPointCount];
			switch(PolygonBase::GetBendMode()) {
                case BEND_45_1:
				if(d.x > d.y)
					center.Set(mousePosition.x + ((mousePosition.x > lastPlacedPoint.x) ? -d.y : d.y), lastPlacedPoint.y);
				else
					center.Set(lastPlacedPoint.x, mousePosition.y + ((mousePosition.y > lastPlacedPoint.y) ? -d.x : d.x));
				break;
            case BEND_45_2:
				if(d.x > d.y)
					center.Set(lastPlacedPoint.x + ((mousePosition.x < lastPlacedPoint.x) ? -d.y : d.y), mousePosition.y);
				else
					center.Set(mousePosition.x, lastPlacedPoint.y + ((mousePosition.y < lastPlacedPoint.y) ? -d.x : d.x));
				break;
            case BEND_90_1:
                center.Set(mousePosition.x, lastPlacedPoint.y);
                break;
            case BEND_90_2:
                center.Set(lastPlacedPoint.x, mousePosition.y);
                break;
			}
		}
	}
}

