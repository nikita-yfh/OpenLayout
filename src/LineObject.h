#pragma once
#include "Object.h"

class LineObject : public Object {
public:
	LineObject() {}
	LineObject(uint8_t layer, float _width) : Object(layer) { width = _width; }

	float GetWidth() const;
	void SetWidth(float size);
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
	

