#pragma once
#include "Object.h"
#include "Vec2.h"
#include "AABB.h"
#include "Array.h"

class Pad : public Object{
public:
	Pad() {}
	Pad(uint8_t layer, float groundDistance, const Vec2 &_position)
		: Object(layer, groundDistance), position(_position), thermal(false),
		thermalStyle{0x55, 0x55, 0x55, 0x55}, thermalSize(0.4f), angle(0.0f) {}

	~Pad();

	virtual void SaveConnections(const Object *objects, File &file) const override;
	virtual void LoadConnections(Object *objects, File &file) override;
	virtual void UpdateConnections(Object *objects) override;
    virtual Object *TestConnections(const Vec2 &pos, float radius) const override;
    virtual void RemoveConnections(Object *object) override;
	virtual void DrawConnections() const override;
	virtual void DrawConnectionsSelected() const override;

	virtual Vec2 GetPosition() const override;
	virtual void Rotate(const Vec2 &center, float angle) override;
	virtual void MirrorHorizontal(float x) override;
	virtual void MirrorVertical(float y) override;
	virtual void Move(const Vec2 &d) override;

    void AddConnection(Pad *object);
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

	

