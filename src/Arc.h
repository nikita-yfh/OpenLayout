#pragma once
#include "LineObject.h"

class Arc : public LineObject {
public:
	void Draw(float halfwidth) const;
	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return ARC;
	}
	virtual AABB GetAABB() const override;
	virtual bool TestPoint(const Vec2 &point) const;
protected:
	Vec2 position;
	float beginAngle;
	float endAngle;
	float diameter;
	uint8_t style;
	bool cutoff;
	bool fill;
};
