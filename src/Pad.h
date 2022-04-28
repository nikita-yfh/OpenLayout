#pragma once
#include "Object.h"
#include "Vec2.h"

class Connections {
public:
	Connections();
	~Connections();

	void Add(Object *object);
	void Remove(const Object *object);
	bool Has(const Object *object) const;

	void Save(const Object *objects, File &file) const;
	void Load(Object *objects, File &file);

private:
	Object **Find(const Object *object);
	uint32_t count;
	Object **connections;
};


class Pad : public Object{
public:
	void SaveConnections(const Object *objects, File &file) const;
	void LoadConnections(Object *objects, File &file);
protected:
	static void WriteArray(File &file, const Vec2 *arr, uint32_t count, const Vec2 &shift);
	static uint32_t ReadArray(File &file, Vec2 *arr, const Vec2 &shift);

	static void WriteSymmetricalArray(File &file, const Vec2 *arr, uint32_t count, const Vec2 &shift);

	Vec2 position;
	enum{
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

	

