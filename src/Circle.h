#pragma once
#include "LineObject.h"
#include "AABB.h"

class Circle : public LineObject {
public:
	Circle() : Circle(0, 0.0f, Vec2(0.0f, 0.0f), 0.0f) {}
	Circle(uint8_t layer, float width, const Vec2 &position, float diameter);

	virtual Circle *Clone() const override;

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return CIRCLE;
	}
	virtual AABB GetAABB() const override;
	virtual AABB GetPointsAABB() const override;
	virtual Vec2 GetPosition() const override;
	virtual bool TestPoint(const Vec2 &point) const override;
	virtual void Rotate(const Vec2 &center, float angle) override;
	virtual void MirrorHorizontal(float x) override;
	virtual void MirrorVertical(float y) override;
	virtual void Move(const Vec2 &d);
private:
	void Draw(float halfwidth) const;

	Vec2 position;
	float beginAngle;
	float endAngle;
	float diameter;
	bool cutout;
	bool fill;

	friend class ObjectGroup;
};
