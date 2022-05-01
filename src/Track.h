#pragma once
#include "PolygonBase.h"

class Track : public PolygonBase {
public:
	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual AABB GetAABB() const override;
private:
	inline bool GetBeginStyle() const {
		return style & 1;
	}
	inline bool GetEndStyle() const {
		return style & 2;
	}
	inline void SetBeginStyle(bool s) {
		if(s ^ GetBeginStyle())
			style ^= 1;
	}
	inline void SetEndStyle(bool s) {
		if(s ^ GetEndStyle())
			style ^= 2;
	}

	float width;
	uint8_t style;
	uint8_t cutoff;
};
