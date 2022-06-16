#pragma once
#include "Object.h"
#include "Vec2.h"
#include "Array.h"

class Pad : public Object{
public:
	Pad() : Pad(0, Vec2(0.0f, 0.0f)) {}
	Pad(uint8_t layer, const Vec2 &_position);
	~Pad();

	virtual void SaveConnections(const Object *objects, File &file) const override;
	virtual void LoadConnections(Object *objects, File &file) override;

	virtual void DrawConnections() const override;

	virtual void UpdateConnections(Object *objects) override;
protected:

	static void WriteArray(File &file, const Vec2 *arr, uint32_t count, const Vec2 &shift);
	static uint32_t ReadArray(File &file, Vec2 *arr, const Vec2 &shift);

	static void WriteSymmetricalArray(File &file, const Vec2 *arr, uint32_t count, const Vec2 &shift);

	Vec2 position;
	enum {
		TH_STYLE_C1,
		TH_STYLE_C2,
		TH_STYLE_I1,
		TH_STYLE_I2,
	};
	bool thermal;
	uint8_t thermalStyle[4];
	float thermalSize;
	float angle;

	UniqueArray<Pad*> connections;
};

	

