#pragma once
#include "PolygonBase.h"

class Track : public PolygonBase {
public:
	Track() {}
	Track(uint8_t layer, float groundDistance, float width, const Vec2 *points, uint32_t count)
		: PolygonBase(layer, groundDistance, width, points, count), style(0) {}

	virtual Track *Clone() const override;

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override {
		return TRACK;
	}
	virtual bool TestPoint(const Vec2 &point) const;

    virtual void BuildRect(const Vec2 &mousePosition) override;
private:
	void Draw(float halfWidth) const;
	inline bool GetBeginStyle() const {
		return style & 0x01;
	}
	inline bool GetEndStyle() const {
		return style & 0x02;
	}
	inline void SetBeginStyle(bool s) {
		if(s ^ GetBeginStyle())
			style ^= 0x01;
	}
	inline void SetEndStyle(bool s) {
		if(s ^ GetEndStyle())
			style ^= 0x02;
	}

	uint8_t style;

	friend class ObjectGroup;
};
