#pragma once
#include <stdint.h>
#include "File.h"

class Object {
public:
	virtual void SaveObject(File &file) const = 0;
	virtual void LoadObject(File &file) = 0;

	void Save(File &file) const;
	static Object *Load(File &file);
protected:
	enum {
		THT_PAD = 2,
		POLY = 4,
		CIRCLE = 5,
		LINE = 6,
		TEXT = 7,
		SMD_PAD = 8
	};
	char marker[256];
	uint8_t layer;
	float groundDistance;
	bool soldermask;
	uint16_t componentID;

	bool selected;
};
