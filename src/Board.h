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

	void AddObject(Object *object);

	const char *GetName() const;

	bool IsMultilayer() const;
	bool ToggleMultilayer();

	bool IsLayerVisible(uint8_t layer) const;
	bool ToggleLayerVisible(uint8_t layer);

	bool GetCurrentLayerGround() const;
	bool ToggleCurrentLayerGround();

	uint8_t GetSelectedLayer() const;
	void SetSelectedLayer(uint8_t layer);

	Object *GetFirstSelected();

	double GetGrid() const;
	void SetGrid(double grid);
	void UpdateGrid(bool shift, bool ctrl);

	Object *GetObjects();

	ImageConfigs images;

	enum Layer {
		LAYER_C1,
		LAYER_S1,
		LAYER_C2,
		LAYER_S2,
		LAYER_I1,
		LAYER_I2,
		LAYER_O,
	};
private:
	char name[30];
	Vec2 size;
	uint8_t groundPane[7];
	uint8_t layerVisible[7];
	uint8_t activeLayer;
	bool multilayer;
	double grid;
	double activeGrid;
	double zoom;
	Vec2 camera;
	Vec2 anchor;

	Object *objects;
	uint32_t GetObjectCount() const;

	Board *next;

	friend class PCB;
};

inline Board *Board::GetNext() {
	return next;
}
inline const Board *Board::GetNext() const {
	return next;
}
inline const char *Board::GetName() const {
	return name;
}
inline uint8_t Board::GetSelectedLayer() const {
	return activeLayer;
}
inline void Board::SetSelectedLayer(uint8_t layer) {
	activeLayer = layer;
}
inline bool Board::IsMultilayer() const {
	return multilayer;
}
inline bool Board::ToggleMultilayer() {
	multilayer = !multilayer;
	return multilayer;
}
inline bool Board::IsLayerVisible(uint8_t layer) const {
	return layerVisible[layer];
}
inline bool Board::ToggleLayerVisible(uint8_t layer) {
	layerVisible[layer] = !layerVisible[layer];
	return layerVisible[layer];
}
inline bool Board::GetCurrentLayerGround() const {
	return groundPane[activeLayer];
}
inline bool Board::ToggleCurrentLayerGround() {
	groundPane[activeLayer] = !groundPane[activeLayer];
	return groundPane[activeLayer];
}
inline double Board::GetGrid() const {
	return grid;
}
inline void Board::SetGrid(double newGrid) {
	grid = activeGrid = newGrid;
}
inline Object *Board::GetObjects() {
	return objects;
}
