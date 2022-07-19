#pragma once
#include "Pad.h"
#include "PadSize.h"

class THTPad : public Pad {
public:
	THTPad() : THTPad(0, 0.0f, Vec2(0.0f, 0.0f), PadSize(0.0f, 0.0f)) {}
	THTPad(uint8_t layer, float groundDistance, const Vec2 &position, const PadSize &_size)
		: Pad(layer, groundDistance, position), size(_size), shape(0), through(false) {}

	virtual THTPad *Clone() const override;

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;
	virtual void DrawDrillings() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return THT_PAD;
	}
	virtual AABB GetAABB() const override;
	virtual bool TestPoint(const Vec2 &point) const;

	const PadSize &GetSize() const;
	void SetSize(const PadSize &size);
	bool HasMetallization() const;
private:
	void Draw(float halfSize, float distance) const;
	uint8_t CalcPoints(Vec2 *points) const;
	PadSize size;
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

	friend class ObjectGroup;
};

inline const PadSize &THTPad::GetSize() const {
	return size;
}
inline void THTPad::SetSize(const PadSize &newSize) {
	size = newSize;
}
inline bool THTPad::HasMetallization() const {
	return through;
}
	

