#pragma once
#include <stdint.h>
#include "File.h"
#include "AABB.h"

class Groups {
public:
	Groups();
	~Groups();

	void Add(uint32_t index);
	void Remove(uint32_t index);
	bool Has(uint32_t index) const;

	void Save(File &file) const;
	void Load(File &file);

private:
	uint32_t *Find(uint32_t index);
	uint32_t count;
	uint32_t *groups;
};

class Object {
public:
	Object();

	virtual void SaveObject(File &file) const = 0;
	virtual void LoadObject(File &file) = 0;

	virtual void SaveConnections(const Object *objects, File &file) const {}
	virtual void LoadConnections(Object *objects, File &file) {}

	void Save(File &file) const;
	static Object *Load(File &file);

	Object *GetNext();
	const Object *GetNext() const;
	uint32_t GetNumber() const;

	virtual uint8_t GetType() const = 0;
	virtual AABB GetAABB() const = 0;

	bool IsSelected() const;
	void Select();
	void Unselect();

	Groups groups;

	enum {
		THT_PAD = 2,
		POLY = 4,
		CIRCLE = 5,
		TRACK = 6,
		TEXT = 7,
		SMD_PAD = 8
	};
protected:
	char marker[256];
	uint8_t layer;
	float groundDistance;
	bool soldermask;
	uint16_t componentID;

	bool selected;
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
inline bool Object::IsSelected() const {
	return selected;
}
inline void Object::Select() {
	selected = true;
}
inline void Object::Unselect() {
	selected = false;
}
