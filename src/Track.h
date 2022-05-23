#pragma once
#include "PolygonBase.h"

class Track : public PolygonBase {
public:
	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return TRACK;
	}
	virtual AABB GetAABB() const override;
private:
	void Draw(float halfWidth) const;
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

	uint8_t style;
};
