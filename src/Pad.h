#pragma once
#include "Object.h"
#include "Vec2.h"

class Pad : public Object{
protected:
	Vec2 position;
	enum{
		TH_STYLE_C1,
		TH_STYLE_C2,
		TH_STYLE_I1,
		TH_STYLE_I2,
	};
	bool thermal;
	uint8_t thermalStyle[4];
	float thermalSize;
	float angle;
};

	

