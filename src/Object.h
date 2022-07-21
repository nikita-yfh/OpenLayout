#pragma once
#include <stdint.h>
#include "File.h"
#include "AABB.h"
#include "Array.h"

class Object {
public:
	Object() {}
	Object(uint8_t _layer, float _groundDistance)
		: prev(nullptr), next(nullptr), marker(""), componentID(0),
		groundDistance(_groundDistance), soldermask(false), selected(false), layer(_layer) {}

	virtual ~Object() {}

	virtual Object *Clone() const = 0;

	virtual void DrawGroundDistance() const = 0;
	virtual void DrawObject() const = 0;
	virtual void DrawDrillings() const {}
	virtual void DrawConnections() const {}

	virtual void SaveObject(File &file) const = 0;
	virtual void LoadObject(File &file) = 0;

	virtual void SaveConnections(const Object *objects, File &file) const {}
	virtual void LoadConnections(Object *objects, File &file) {}

	virtual void UpdateConnections(Object *objects) {}

	void Save(File &file) const;
	static Object *Load(File &file);

	Object *GetNext();
	const Object *GetNext() const;
	const Object *GetPrev() const;
	uint8_t GetLayer() const;

	virtual uint8_t GetType() const = 0;
	virtual AABB GetAABB() const = 0;
	virtual Vec2 GetPosition() const = 0;
	virtual Vec2 GetNearestPoint(const Vec2 &point) const;
	virtual bool TestPoint(const Vec2 &point) const = 0;
	virtual void Rotate(const Vec2 &center, float angle) = 0;
	virtual void MirrorHorizontal(float x) = 0;
	virtual void MirrorVertical(float y) = 0;
	virtual void Move(const Vec2 &d) = 0;

	void SetGroundDistance(float distance);
	float GetGroundDistance() const;

	bool IsPlaced() const;
	bool IsSelected() const;
	void SetPlaced();
	void Select();
	void Unselect();
	void InvertSelection();

	UniqueArray<uint32_t> groups;

	enum {
		THT_PAD = 2,
		POLY = 4,
		CIRCLE = 5,
		TRACK = 6,
		TEXT = 7,
		SMD_PAD = 8
	};
protected:
	void SaveGroups(File &file) const;
	void LoadGroups(File &file);

	char marker[256];
	uint8_t layer;
	float groundDistance;
	bool soldermask;
	uint16_t componentID;

	uint8_t selected;
private:
	Object *prev;
	Object *next;

	friend class ObjectGroup;
};

inline Object *Object::GetNext() {
	return next;
}
inline const Object *Object::GetNext() const {
	return next;
}
inline const Object *Object::GetPrev() const {
	return prev;
}
inline bool Object::IsPlaced() const {
	return selected == 2;
}
inline bool Object::IsSelected() const {
	return selected == 1;
}
inline void Object::SetPlaced() {
	selected = 2;
}
inline void Object::Select() {
	selected = 1;
}
inline void Object::Unselect() {
	selected = 0;
}
inline void Object::InvertSelection() {
	selected = !selected;
}
inline uint8_t Object::GetLayer() const {
	return layer;
}
inline void Object::SetGroundDistance(float distance) {
	groundDistance = distance;
}
inline float Object::GetGroundDistance() const {
	return groundDistance;
}
