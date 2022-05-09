#pragma once
#include "LineObject.h"

class Arc : public LineObject {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return ARC;
	}
	virtual AABB GetAABB() const override;
protected:
	Vec2 position;
	float beginAngle;
	float endAngle;
	float diameter;
	uint8_t style;
	uint8_t cutoff;
	uint8_t fill;
};
