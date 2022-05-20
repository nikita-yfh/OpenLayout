#pragma once
#include "Pad.h"

class SMDPad : public Pad {
public:
	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return SMD_PAD;
	}
	virtual AABB GetAABB() const override;

	const Vec2 &GetSize() const;
	void SetSize(const Vec2 &size);
private:
	Vec2 size;
};

inline const Vec2 &SMDPad::GetSize() const {
	return size;
}
inline void SMDPad::SetSize(const Vec2 &newSize) {
	size = newSize;
}
	

