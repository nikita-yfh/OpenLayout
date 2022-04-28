#pragma once
#include "Object.h"

class Track : public Object {
public:
	Track();
	~Track();

	void SaveObject(File &file) const;
	void LoadObject(File &file);

	virtual AABB GetAABB() const override;
private:
	float width;
	uint8_t shape;
	uint8_t cutoff;

	uint32_t count;
	Vec2 *points;
};
