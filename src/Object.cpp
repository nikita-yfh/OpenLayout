#include "Object.h"
#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"

Groups::Groups() {
	groups = nullptr;
	count = 0;
}
Groups::~Groups() {
	if(groups)
		free(groups);
	count = 0;
}
void Groups::Add(uint32_t index) {
	if(Find(index))
		return;
	groups = (uint32_t*) realloc(groups, (count + 1) * sizeof(uint32_t));
	groups[count++] = index;
}
void Groups::Remove(uint32_t index) {
	uint32_t *i = Find(index);
	if(i) {
		count--;
		*i = groups[count]; // Move last item to deleted
		groups = (uint32_t*) realloc(groups, count * sizeof(uint32_t));
	}
}
uint32_t *Groups::Find(uint32_t index) {
	for(int i = 0; i < count; i++)
		if(groups[i] == index)
			return &groups[i];
	return nullptr;
}
bool Groups::Has(uint32_t index) const {
	for(int i = 0; i < count; i++)
		if(groups[i] == index)
			return true;
	return false;
}
void Groups::Save(File &file) const {
	file.Write<uint32_t>(count);
	for(int i = 0; i < count; i++)
		file.Write<uint32_t>(groups[i]);
}
void Groups::Load(File &file) {
	this->~Groups();
	count = file.Read<uint32_t>();
	if(count) {
		groups = (uint32_t*) malloc(count * sizeof(uint32_t));
		for(int i = 0; i < count; i++)
			groups[i] = file.Read<uint32_t>();
	}
}

Object::Object() {
	prev = nullptr;
	next = nullptr;
	*marker = '\0';
	componentID = 0;
	groundDistance = 0.0f;
}
uint32_t Object::GetNumber() const {
	uint32_t count = 0;
	for(Object *i = prev; i; i = i->next)
		count++;
	return count;
}
	
void Object::Save(File &file) const{
	SaveObject(file);
}
Object *Object::Load(File &file){
	Object *object;
	uint8_t type = file.Read<uint8_t>();
	switch(type){
		case THT_PAD:
			object = new THTPad();
			break;
		case POLY:
			object = new Poly();
			break;
			break;
		case CIRCLE:
			break;
		case TRACK:
			object = new Track();
			break;
		case TEXT:
			break;
		case SMD_PAD:
			object = new SMDPad();
			break;
	}
	object->LoadObject(file);
	return object;
}

