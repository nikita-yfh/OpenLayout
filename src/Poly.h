#pragma once
#include "PolygonBase.h"

class Poly : public PolygonBase {
public:
	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return POLY;
	}
	virtual AABB GetAABB() const override;
private:
	void Draw(float halfWidth) const;

	bool onlySoldermask;
	uint8_t cutoff;
	bool hatched;
	uint8_t customHatch;
	float hatchSize;
};
