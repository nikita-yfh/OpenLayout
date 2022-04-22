#pragma once
#include "Vec2.h"
#include "Object.h"
#include "ImageConfig.h"
#include "File.h"

class Board {
public:
	enum class Type{
		Empty,
		Rectangle,
		Round
	};

	Board(Type type, Vec2 innerSize, float border);
	Board();

	~Board();

	void Load(File &file);
	void Save(File &file) const;

	Board *GetNext();
	const Board *GetNext() const;

	ImageConfigs images;
private:
	enum Layer {
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

	Object *objects;

	Board *next;

	friend class PCB;
};

inline Board *Board::GetNext() {
	return next;
}
inline const Board *Board::GetNext() const {
	return next;
}
