#pragma once
#include "Pad.h"
#include "PadSize.h"

class THTPad : public Pad {
public:
	THTPad() {}
	THTPad(uint8_t layer, float groundDistance, const Vec2 &position, const PadSize &_size, uint8_t _shape, bool metallization)
			: Pad(layer, groundDistance, position), size(_size), shape(_shape), through(metallization) {
		if(shape > S_SQUARE_H) {
			shape -= 3;
			angle += M_PI / 2.0f;
		}
	}

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

	enum {
		S_CIRCLE,
		S_OCTAGON,
		S_SQUARE,
		S_CIRCLE_H,
		S_OCTAGON_H,
		S_SQUARE_H,
		S_CIRCLE_V,
		S_OCTAGON_V,
		S_SQUARE_V,

        S_COUNT
	};
private:
	void Draw(float halfSize, float distance) const;
	uint8_t CalcPoints(Vec2 *points) const;
	PadSize size;
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
	

