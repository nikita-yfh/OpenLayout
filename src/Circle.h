#pragma once
#include "LineObject.h"

class Circle : public LineObject {
public:
	Circle() : Circle(0, 0.0f, Vec2(0.0f, 0.0f), 0.0f) {}
	Circle(uint8_t layer, float width, const Vec2 &position, float diameter);

	virtual Circle *Clone() const override;

	void Draw(float halfwidth) const;
	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return CIRCLE;
	}
	virtual AABB GetAABB() const override;
	virtual bool TestPoint(const Vec2 &point) const;
protected:
	Vec2 position;
	float beginAngle;
	float endAngle;
	float diameter;
	bool cutout;
	bool fill;

	friend class ObjectGroup;
};
