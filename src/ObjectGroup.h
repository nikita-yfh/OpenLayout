#pragma once
#include "Object.h"

class ObjectGroup {
public:
	ObjectGroup(const ObjectGroup &other);
	ObjectGroup();

	~ObjectGroup();

	void AddObjectBegin(Object *object);
	void AddObjectEnd(Object *object);
	Object *AddObjectEnd(Object *object, Object *last);

	bool IsEmpty() const;

	Object *GetFirstSelected();
	bool IsSelected() const;

	Object *GetObjects();

	bool SelectObject(const Vec2 &point);
	void SelectAll();
	void UnselectAll();

	void DeleteSelected();

	enum Layer {
		LAYER_C1,
		LAYER_S1,
		LAYER_C2,
		LAYER_S2,
		LAYER_I1,
		LAYER_I2,
		LAYER_O,
	};
protected:
	Object *GetLast();
	uint32_t GetObjectCount() const;
	Object *GetObject(uint32_t n);

	Object *objects;
};

inline Object *ObjectGroup::GetObjects() {
	return objects;
}
inline bool ObjectGroup::IsEmpty() const {
	return objects == nullptr;
}
