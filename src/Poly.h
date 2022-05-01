#pragma once
#include "PolygonBase.h"

class Poly : public PolygonBase {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual AABB GetAABB() const override;
private:
	float width;
	uint8_t onlySoldermask;
	uint8_t cutoff;
	uint8_t hatched;
	uint8_t customHatch;
	float hatchSize;
};
