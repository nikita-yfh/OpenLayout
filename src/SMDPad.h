#pragma once
#include "Pad.h"

class SMDPad : public Pad {
public:
	void SaveObject(File &file) const;
	void LoadObject(File &file);

	virtual AABB GetAABB() const override;
private:
	Vec2 size;
};

	

