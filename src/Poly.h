#pragma once
#include "PolygonBase.h"

class Poly : public PolygonBase {
public:
	Poly() : Poly(0, 0.0f, nullptr, 0) {}
	Poly(uint8_t layer, float width, const Vec2 *points, uint32_t count)
		: PolygonBase(layer, width, points, count) {}

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return POLY;
	}
	virtual AABB GetAABB() const override;
	virtual bool TestPoint(const Vec2 &point) const;
private:
	void Draw(float halfWidth) const;

	bool onlySoldermask;
	uint8_t cutoff;
	bool hatched;
	uint8_t customHatch;
	float hatchSize;

	friend class ObjectGroup;
};
