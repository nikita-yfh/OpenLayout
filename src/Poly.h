#pragma once
#include "PolygonBase.h"

class Poly : public PolygonBase {
public:
	Poly() {}
	Poly(uint8_t layer, float groundDistance, float width, const Vec2 *points, uint32_t count)
		: PolygonBase(layer, groundDistance, width, points, count), onlySoldermask(false),
		cutout(false), hatched(false), customHatch(false), hatchSize(0.8f) {}

	virtual Poly *Clone() const override;

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return POLY;
	}
	virtual bool TestPoint(const Vec2 &point) const;
private:
	void Draw(float halfWidth) const;

	bool onlySoldermask;
	uint8_t cutout;
	bool hatched;
	uint8_t customHatch;
	float hatchSize;

	friend class ObjectGroup;
};
