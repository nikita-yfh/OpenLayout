#include "Object.h"
#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"

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

