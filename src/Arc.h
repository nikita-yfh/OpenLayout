#pragma once
#include "Object.h"

class Arc : public Object {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return CIRCLE;
	}
	virtual AABB GetAABB() const override;

	float GetWidth() const;
	void SetWidth(float newWidth);
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

inline float Arc::GetWidth() const {
	return width;
}
inline void Arc::SetWidth(float newWidth) {
	width = newWidth;
}
	

