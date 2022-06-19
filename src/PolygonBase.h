#pragma once
#include "LineObject.h"
#include "Array.h"

class PolygonBase : public LineObject {
public:
	PolygonBase() {}
	PolygonBase(uint8_t layer, float width, const Vec2 *points, uint32_t count);

	virtual AABB GetAABB() const override;
	virtual AABB GetPointsAABB() const override;
	virtual void Rotate(const Vec2 &center, float angle) override;
	virtual void MirrorHorizontal(float x) override;
	virtual void MirrorVertical(float y) override;
	virtual void ToGrid(double grid, const Vec2 &origin) override;
protected:
	void LoadPoints(File &file, bool onlySoldermask = false);
	void SavePoints(File &file, bool onlySoldermask = false) const;

	Array<Vec2> points;
	bool cutout;

	friend class ObjectGroup;
};
