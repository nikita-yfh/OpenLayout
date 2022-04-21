#pragma once
#include "Vec2.h"
#include "ImageConfig.h"
#include "File.h"

class Board {
public:
	void Load(File &file);
	void Save(File &file) const;

private:
	enum {
		LAYER_C1=1,
		LAYER_S1,
		LAYER_C2,
		LAYER_S2,
		LAYER_I1,
		LAYER_I2,
		LAYER_O,
	};
	char name[30];
	Vec2 size;
	uint8_t groundPane[7];
	uint8_t layerVisible[7];
	uint32_t activeLayer;
	uint8_t isMultilayer;
	double grid;
	double zoom;
	Vec2 camera;
	Vec2 anchor;
	ImageConfigs images;

	uint8_t __pad0[4];
	uint8_t __pad1[8];

	Board *next;

	friend class PCB;
};
