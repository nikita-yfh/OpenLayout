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

bool ObjectGroup::IsSelected() const {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return true;
	return false;
}

bool ObjectGroup::SelectObject(const Vec2 &point) {
	for(Object *object = objects; object; object = object->next)
		if(object->GetAABB().TestPoint(point) && object->TestPoint(point)) {
			object->InvertSelection();
			return true;
		}
	return false;
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

