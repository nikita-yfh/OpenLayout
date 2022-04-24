#include "Object.h"
#include "THTPad.h"
#include "SMDPad.h"

Connections::Connections() {
	connections = nullptr;
	count = 0;
}
Connections::~Connections() {
	if(connections)
		free(connections);
	count = 0;
}
void Connections::Add(Object *object) {
	if(Find(object))
		return;
	connections = (Object**)realloc(connections, (count + 1) * sizeof(Object*));
	connections[count++] = object;
}
void Connections::Remove(const Object *object) {
	Object **i = Find(object);
	if(i) {
		count--;
		*i = connections[count]; // Move last item to deleted
		connections = (Object**)realloc(connections, count * sizeof(Object*));
	}
}
Object **Connections::Find(const Object *object) {
	for(Object **i = connections; *i; i++)
		if(*i == object)
			return i;
	return nullptr;
}
bool Connections::Has(const Object *object) const {
	for(Object **i = connections; *i; i++)
		if(*i == object)
			return true;
	return false;
}
void Connections::Save(const Object *objects, File &file) const {
	file.Write<uint32_t>(count);
	if(count) {
		for(Object **i = connections; *i; i++) {
			uint32_t index;
			for(const Object *j = objects; j; j = j->GetNext(), index++)
				if(j == *i) {
					file.Write<uint32_t>(index);
					break;
				}
		}
	}
}
void Connections::Load(Object *objects, File &file) {
	this->~Connections();
	count = file.Read<uint32_t>();
	if(count) {
		connections = (Object**)malloc(count * sizeof(Object*));
		for(int i = 0; i < count; i++) {
			int index = file.Read<uint32_t>();
			connections[i] = objects;
			while(index) {
				connections[i] = connections[i]->GetNext();
				index--;
			}
		}
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
			break;
		case CIRCLE:
			break;
		case LINE:
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
void Object::WriteArray(File &file, const Vec2 *arr, uint32_t count) {
	file.Write<uint32_t>(count);
	for(int i = 0; i < count; i++)
		arr[i].Save<float>(file);
}
uint32_t Object::ReadArray(File &file, Vec2 *arr) {
	uint32_t count = file.Read<uint32_t>();
	for(int i = 0; i < count; i++)
		arr[i].Load<float>(file);
	return count;
}


