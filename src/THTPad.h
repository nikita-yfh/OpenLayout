#pragma once
#include "Pad.h"

class THTPad : public Pad {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual AABB GetAABB() const override;
private:
	float inDiameter;
	float outDiameter;
	enum{
		CIRCLE = 1,
		OCTAGON,
		SQUARE,
		CIRCLE_E,
		OCTAGON_E,
		SQUARE_E,
	};
	uint8_t shape;
	bool through;
};

	

