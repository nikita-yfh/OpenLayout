#pragma once
#include "Pad.h"

class SMDPad : public Pad {
public:
	SMDPad() : SMDPad(0, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)) {}
	SMDPad(uint8_t layer, const Vec2 &position, const Vec2 &size);

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return SMD_PAD;
	}
	virtual AABB GetAABB() const override;
	virtual bool TestPoint(const Vec2 &point) const override;

	const Vec2 &GetSize() const;
	void SetSize(const Vec2 &size);
private:
	Vec2 size;

	friend class ObjectGroup;
};

inline const Vec2 &SMDPad::GetSize() const {
	return size;
}
inline void SMDPad::SetSize(const Vec2 &newSize) {
	size = newSize;
}
	

