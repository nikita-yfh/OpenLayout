#pragma once
#include "Object.h"

class Arc : public Object {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual AABB GetAABB() const override;
protected:
	Vec2 position;
	float width;
	float beginAngle;
	float endAngle;
	float diameter;
	uint8_t style;
	uint8_t cutoff;
	uint8_t fill;
};
