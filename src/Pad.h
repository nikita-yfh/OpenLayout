#pragma once
#include "Object.h"
#include "Vec2.h"

class Pad;

class Connections {
public:
	Connections();
	~Connections();

	void Add(Pad *object);
	void Remove(const Pad *object);
	bool Has(const Pad *object) const;

	inline const Pad *operator[](uint32_t index) const {
		return connections[index];
	}
	inline uint32_t Count() const {
		return count;
	}

	void Save(const Object *objects, File &file) const;
	void Load(Object *objects, File &file);

private:
	Pad **Find(const Pad *object);
	uint32_t count;
	Pad **connections;
};

class Pad : public Object{
public:
	Pad() : Pad(0, Vec2(0.0f, 0.0f)) {}
	Pad(uint8_t layer, const Vec2 &_position);

	virtual void SaveConnections(const Object *objects, File &file) const override;
	virtual void LoadConnections(Object *objects, File &file) override;

	virtual void DrawConnections() const override;
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

	Connections connections;
};

	

