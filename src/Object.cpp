#include "Object.h"
#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"
#include "Circle.h"


Object::Object(uint8_t _layer) {
	prev = nullptr;
	next = nullptr;
	*marker = '\0';
	componentID = 0;
	groundDistance = 0.0f;
	soldermask = false;
	selected = false;
	layer = _layer;
}

void Object::SaveGroups(File &file) const {
	file.Write<uint32_t>(groups.Size());
	for(int i = 0; i < groups.Size(); i++)
		file.Write<uint32_t>(groups[i]);
}
void Object::LoadGroups(File &file) {
	uint32_t count = file.Read<uint32_t>();
	groups.Init(count);
	for(int i = 0; i < count; i++)
		groups[i] = file.Read<uint32_t>();
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
		case CIRCLE:
			object = new Circle();
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

