#include "Object.h"
#include "THTPad.h"

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
			break;
	}
	object->LoadObject(file);
	return object;
}
