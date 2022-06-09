#pragma once
#include "Object.h"

class ObjectGroup {
public:
	ObjectGroup();
	~ObjectGroup();

	void AddObjectBegin(Object *object);
	void AddObjectEnd(Object *object);
	bool IsEmpty() const;

	Object *GetFirstSelected();
	bool IsSelected() const;

	Object *GetObjects();

	bool SelectObject(const Vec2 &point);
	void UnselectAll();

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

	Object *objects;
};

inline Object *ObjectGroup::GetObjects() {
	return objects;
}
inline bool ObjectGroup::IsEmpty() const {
	return objects == nullptr;
}
