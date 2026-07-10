#pragma once
#include "Object.h"

class LineObject : public Object {
public:
	LineObject() {}
	LineObject(uint8_t layer, float groundDistance, float _width)
		: Object(layer, groundDistance), width(_width) {}

	float GetWidth() const;
	virtual void SetWidth(float size) override;
protected:
	float width;

	friend class ObjectGroup;
};

inline float LineObject::GetWidth() const {
	return width;
}
inline void LineObject::SetWidth(float newWidth) {
	width = newWidth;
}
	

