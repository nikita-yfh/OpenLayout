#include "ObjectGroup.h"

ObjectGroup::ObjectGroup(const ObjectGroup &other) {
	Object *last = nullptr;
	for(const Object *object = other.objects; object; object = object->next)
		last = AddObjectEnd(object->Clone(), last);
	for(Object *object = objects; object; object = object->next)
		object->UpdateConnections(objects);
}

ObjectGroup::ObjectGroup() {
	objects = nullptr;
}

ObjectGroup::~ObjectGroup() {
	while(objects) {
		Object *next = objects->next;
		delete objects;
		objects = next;
	}
}

Object *ObjectGroup::GetLast() {
	if(IsEmpty())
		return nullptr;
	Object *object = objects;
	while(object->next) object = object->next;
	return object;
}

uint32_t ObjectGroup::GetObjectCount() const {
	uint32_t n = 0;
	Object *tmp = objects;
	while(tmp) {
		n++;
		tmp = tmp->GetNext();
	}
	return n;
}

void ObjectGroup::AddObjectEnd(Object *object) {
	object->next = nullptr;
	Object *last = GetLast();
	if(last)
		last->next = object;
	else
		objects = object;
	object->prev = last;
}

Object *ObjectGroup::AddObjectEnd(Object *object, Object *last) {
	if(last == nullptr)
		objects = object;
	else
		last->next = object;
	object->prev = last;
	object->next = nullptr;
	return object;
}

void ObjectGroup::AddObjectBegin(Object *object) {
	object->next = objects;
	object->prev = nullptr;
	if(objects)
		objects->prev = object;
	objects = object;
}

Object *ObjectGroup::GetFirstSelected() {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return object;
	return nullptr;
}

const Object *ObjectGroup::GetFirstSelected() const {
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return object;
	return nullptr;
}

bool ObjectGroup::IsSelected() const {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return true;
	return false;
}

bool ObjectGroup::SelectObject(const Vec2 &point) {
	for(Object *object = objects; object; object = object->next)
		if(object->GetAABB().TestPoint(point) && object->TestPoint(point)) {
			InvertSelectionGroup(object);
			return true;
		}
	return false;
}

void ObjectGroup::InvertSelectionGroup(Object *o1) {
	bool selected = !o1->selected;
	o1->selected = selected;
	for(Object *o2 = objects; o2; o2 = o2->next)
		for(int i1 = 0; i1 < o1->groups.Size(); i1++)
			for(int i2 = 0; i2 < o2->groups.Size(); i2++)
				if(o1->groups[i1] == o2->groups[i2])
					o2->selected = selected;
}

void ObjectGroup::SelectAll() {
	for(Object *object = objects; object; object = object->next)
		object->Select();
}

void ObjectGroup::UnselectAll() {
	for(Object *object = objects; object; object = object->next)
		object->Unselect();
}

void ObjectGroup::DeleteSelected() {
	for(Object *object = objects; object;) {
		if(object->IsSelected()) {
			if(object->next)
				object->next->prev = object->prev;
			if(object->prev)
				object->prev->next = object->next;
			else
				objects = object->next;
			Object *temp = object;
			object = object->next;
			delete temp;
		} else
			object = object->next;
	}
}

bool ObjectGroup::CanGroup() const {
	const Object *first = nullptr;
	uint32_t count = 0;
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected()) {
			if(!first)
				first = object;
			count++;
		}
	if(!first || count <= 1)
		return false;
	for(int i = 0; i < first->groups.Size(); i++) {
		bool all = true;
		for(const Object *object = first->next; object; object = object->next)
			if(object->IsSelected() && !object->groups.Has(first->groups[i])) {
				all = false;
				break;
			}
		if(all)
			return false;
	}
	return true;
}
bool ObjectGroup::CanUngroup() const {
	uint32_t max = GetMaxSelectedGroup();
	bool groups[max + 1] = {false};
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			for(int i = 0; i < object->groups.Size(); i++) {
				if(groups[object->groups[i]])
					return true;
				else
					groups[object->groups[i]] = true;
			}
	return false;
}

void ObjectGroup::GroupSelected() {
	uint32_t group = GetFreeGroup();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->groups.Add(group);
}

void ObjectGroup::UngroupSelected() {
	uint32_t max = GetMaxSelectedGroup();
	uint32_t groupCount[max + 1] = {0};
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			for(int i = 0; i < object->groups.Size(); i++)
				groupCount[object->groups[i]]++;
	uint32_t group;
	uint32_t maxCount = 0;
	for(int i = 0; i <= max; i++) {
		if(maxCount < groupCount[i]) {
			maxCount = groupCount[i];
			group = i;
		}
	}
	
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->groups.Remove(group);

}

uint32_t ObjectGroup::GetFreeGroup() const {
	uint32_t max = 0;
	for(const Object *object = objects; object; object = object->next)
		for(int i = 0; i < object->groups.Size(); i++)
			if(max < object->groups[i])
				max = object->groups[i];
	return max + 1;
}

uint32_t ObjectGroup::GetMaxSelectedGroup() const {
	uint32_t max = 0;
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			for(int i = 0; i < object->groups.Size(); i++)
				if(max < object->groups[i])
					max = object->groups[i];
	return max;
}

void ObjectGroup::RotateSelected(float angle) {
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->Rotate(center, angle);
}

Vec2 ObjectGroup::GetSelectedCenter() const {
	const Object *first = GetFirstSelected();
	if(!first)
		return Vec2(FLT_MAX, FLT_MAX);
	AABB aabb = first->GetPointsAABB();
	for(const Object *object = first->next; object; object = object->next)
		if(object->IsSelected())
			aabb |= object->GetPointsAABB();
	return aabb.GetCenter();
}


