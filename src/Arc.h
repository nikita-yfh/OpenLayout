#pragma once
#include "Object.h"

class Arc : public Object {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual AABB GetAABB() const override;
protected:
	float width;
	uint8_t style;
	uint8_t cutoff;
};
