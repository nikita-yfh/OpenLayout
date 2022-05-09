#pragma once
#include "Object.h"

class LineObject : public Object {
public:
	float GetWidth() const;
	void SetWidth(float size);
protected:
	float width;
};

inline float LineObject::GetWidth() const {
	return width;
}
inline void LineObject::SetWidth(float newWidth) {
	width = newWidth;
}
	

