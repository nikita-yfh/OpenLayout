#pragma once
#include "Pad.h"
#include "PadSize.h"

class THTPad : public Pad {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return THT_PAD;
	}
	virtual AABB GetAABB() const override;

	const PadSize &GetSize() const;
	void SetSize(const PadSize &size);
private:
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
};

inline const PadSize &THTPad::GetSize() const {
	return size;
}
inline void THTPad::SetSize(const PadSize &newSize) {
	size = newSize;
}
	

