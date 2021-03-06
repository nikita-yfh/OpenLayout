#pragma once
#include "LineObject.h"
#include "Array.h"

class PolygonBase : public LineObject {
public:
	PolygonBase() {}
	PolygonBase(uint8_t layer, float groundDistance, float width, const Vec2 *array, uint32_t count)
		: LineObject(layer, groundDistance, width), points(array, count), cutout(false) {}

	virtual AABB GetAABB() const override;
	virtual Vec2 GetPosition() const override;
	virtual Vec2 GetNearestPoint(const Vec2 &point) const override;
	virtual void Rotate(const Vec2 &center, float angle) override;
	virtual void MirrorHorizontal(float x) override;
	virtual void MirrorVertical(float y) override;
	virtual void Move(const Vec2 &d) override;

	Array<Vec2> points;
protected:
	void LoadPoints(File &file, bool onlySoldermask = false);
	void SavePoints(File &file, bool onlySoldermask = false) const;

	bool cutout;

	friend class ObjectGroup;
};
