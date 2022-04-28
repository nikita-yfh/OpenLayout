#pragma once
#include <stdint.h>
#include "File.h"
#include "AABB.h"

class Object;

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


class Object {
public:
	Object();

	virtual void SaveObject(File &file) const = 0;
	virtual void LoadObject(File &file) = 0;

	void Save(File &file) const;
	static Object *Load(File &file);

	Object *GetNext();
	const Object *GetNext() const;
	uint32_t GetNumber() const;

	virtual AABB GetAABB() const = 0;

	Connections connections;
protected:
	enum {
		THT_PAD = 2,
		POLY = 4,
		CIRCLE = 5,
		LINE = 6,
		TEXT = 7,
		SMD_PAD = 8
	};
	char marker[256];
	uint8_t layer;
	float groundDistance;
	bool soldermask;
	uint16_t componentID;

	bool selected;
protected:

	void LoadGroups(File &file);
	void SaveGroups(File &file) const;
private:
	Object *prev;
	Object *next;

	friend class Board;
};

inline Object *Object::GetNext() {
	return next;
}
inline const Object *Object::GetNext() const {
	return next;
}
