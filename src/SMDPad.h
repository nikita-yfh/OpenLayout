#pragma once
#include "Pad.h"

class SMDPad : public Pad {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual AABB GetAABB() const override;
private:
	Vec2 size;
};

	

