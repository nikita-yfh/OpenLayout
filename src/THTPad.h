#pragma once
#include "Pad.h"

class THTPad : public Pad {
public:
	void SaveObject(File &file) const;
	void LoadObject(File &file);

	virtual AABB GetAABB() const override;
private:
	float inDiameter;
	float outDiameter;
	enum{
		CIRCLE,
		OCTAGON,
		SQUARE,
		CIRCLE_E,
		OCTAGON_E,
		SQUARE_E
	};
	uint8_t shape;
	bool through;
};

	

