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
	const Object *GetFirstSelected() const;
	bool IsSelected() const;
	bool IsSelectedTwo() const;

	Object *GetObjects();

	bool SelectObject(const Vec2 &point);
	void SelectAll();
	void UnselectAll();

	bool CanGroup() const;
	bool CanUngroup() const;
	void GroupSelected();
	void UngroupSelected();

	void RotateSelected(float angle);
	void MirrorSelectedHorizontal();
	void MirrorSelectedVertical();

	void AlignSelected(Vec2(*callback)(const AABB&, const AABB&));

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
	void InvertSelectionGroup(Object *object);
	uint32_t GetFreeGroup() const;
	uint32_t GetMaxSelectedGroup() const;
	Vec2 GetSelectedCenter() const;
	AABB GetSelectedAABB() const;

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
