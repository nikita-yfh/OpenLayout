#include "ObjectGroup.h"
#include "GLUtils.h"
#include "THTPad.h"

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

void ObjectGroup::PlaceObject(Object *object) {
	object->SetPlaced();
	AddObjectBegin(object);
}

void ObjectGroup::PlaceGroup(const ObjectGroup &group, const Vec2 &position) {
	uint32_t freeGroup = GetFreeGroup();
	Object *last = GetLast();
	Object *beginGroup = nullptr;
	for(const Object *object = group.objects; object; object = object->next) {
		last = AddObjectEnd(object->Clone(), last);
		if(!beginGroup)
			beginGroup = last;
	}
	for(Object *object = beginGroup; object; object = object->next) {
		object->UpdateConnections(beginGroup);
		for(int i = 0; i < object->groups.Size(); i++)
			object->groups[i] += freeGroup;
		object->groups.Add(freeGroup);
		object->SetPlaced();
		object->Move(position);
	}
}

Object *ObjectGroup::GetFirstPlaced() {
	for(Object *object = objects; object; object = object->next)
		if(object->IsPlaced())
			return object;
	return nullptr;
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
	return false; }

bool ObjectGroup::IsSelectedTwo() const {
	bool first = false;
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected()) {
			if(first)
				return true;
			first = true;
		}
	return false;
}

Object *ObjectGroup::TestPoint(const Vec2 &point) {
	for(Object *object = objects; object; object = object->next)
		if(object->GetAABB().TestPoint(point) && object->TestPoint(point))
			return object;
	return nullptr;
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
		if(!object->IsPlaced())
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

void ObjectGroup::CancelPlacing() {
	for(Object *object = objects; object;) {
		if(object->IsPlaced()) {
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
	const Object *first = GetFirstSelected();
	if(!first)
		return false;
	for(const Object *object = first->next; object; object = object->next)
		if(object->IsSelected() && (first->groups.Empty() || object->groups.Empty() || object->groups.Last() != first->groups.Last()))
			return true;
	return false;
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

void ObjectGroup::MovePlaced(const Vec2 &d) {
	for(Object *object = objects; object; object = object->next)
		if(object->IsPlaced())
			object->Move(d);
}

void ObjectGroup::MoveSelected(const Vec2 &d) {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->Move(d);
}

void ObjectGroup::RotateSelected(float angle) {
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->Rotate(center, angle);
}

void ObjectGroup::MirrorSelectedHorizontal() {
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->MirrorHorizontal(center.x);
}

void ObjectGroup::MirrorSelectedVertical() {
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->MirrorVertical(center.y);
}

void ObjectGroup::AlignSelected(Vec2(*callback)(const AABB&, const AABB&)) {
	uint32_t max = GetMaxSelectedGroup();
	AABB aabb[max + 1];
	for(int i = 0; i <= max; i++)
		aabb[i] = AABB::Invalid();
	AABB all = GetSelectedAABB();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected() && !object->groups.Empty())
			aabb[object->groups.Last()] |= object->GetAABB();
	for(Object *object = objects; object; object = object->GetNext())
		if(object->IsSelected()) {
			if(object->groups.Empty())
				object->Move(callback(all, object->GetAABB()));
			else
				object->Move(callback(all, aabb[object->groups.Last()]));
		}
}

AABB ObjectGroup::GetObjectsAABB() const {
	if(!objects)
		return AABB::Invalid();
	AABB aabb = objects->GetAABB();
	for(const Object *object = objects->next; object; object = object->next)
		aabb |= object->GetAABB();
	return aabb;
}

AABB ObjectGroup::GetSelectedAABB() const {
	const Object *first = GetFirstSelected();
	AABB aabb = first->GetAABB();
	for(const Object *object = first->next; object; object = object->next)
		if(object->IsSelected())
			aabb |= object->GetAABB();
	return aabb;
}

Vec2 ObjectGroup::GetSelectedCenter() const {
	return GetSelectedAABB().GetCenter();
}

void ObjectGroup::DrawObjects(const ColorScheme &colors, uint8_t activeLayer, bool selected) const {
	const uint8_t layers[7][7] = {
		{LAYER_I1, LAYER_I2, LAYER_C2, LAYER_C1, LAYER_S2, LAYER_S1, LAYER_O},
		{LAYER_C1, LAYER_C2, LAYER_I2, LAYER_I1, LAYER_S1, LAYER_S2, LAYER_O},
		{LAYER_C1, LAYER_C2, LAYER_I1, LAYER_I2, LAYER_S1, LAYER_S2, LAYER_O},
		{LAYER_I1, LAYER_I2, LAYER_C1, LAYER_C2, LAYER_S1, LAYER_S2, LAYER_O}
	};
	for(int i = 0; i < 7; i++) {
		uint8_t layer = layers[3][i];
		if(activeLayer == LAYER_C1 || activeLayer == LAYER_S1)
			layer = layers[0][i];
		else if(activeLayer == LAYER_I1)
			layer = layers[1][i];
		else if(activeLayer == LAYER_I2)
			layer = layers[2][i];
		colors.SetColor(COLOR_C1 + layer);
		for(const Object *object = objects; object; object = object->GetNext())
			if(object->GetLayer() == layer && (!object->IsSelected() || selected) &&
					!(object->GetType() == Object::THT_PAD && ((THTPad*) object)->HasMetallization()))
				object->DrawObject();
	}
	colors.SetColor(COLOR_VIA);
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->GetType() == Object::THT_PAD && !object->IsSelected() && ((THTPad*) object)->HasMetallization())
			object->DrawObject();
}

void ObjectGroup::DrawGroundDistance(uint8_t activeLayer) const {
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->GetLayer() == activeLayer)
			object->DrawGroundDistance();
}

void ObjectGroup::DrawConnections() const {
	glLineWidth(1.5f);
	glBegin(GL_LINES);
	for(const Object *object = objects; object; object = object->GetNext()) 
		object->DrawConnections();
	glEnd();
}

void ObjectGroup::DrawSelected() const {
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->IsSelected())
			object->DrawObject();
}

void ObjectGroup::DrawDrillings() const {
	for(const Object *object = objects; object; object = object->GetNext())
		object->DrawDrillings();
}


