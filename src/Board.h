#pragma once
#include "Vec2.h"
#include "Object.h"
#include "ObjectGroup.h"
#include "ImageConfig.h"
#include "Settings.h"
#include "File.h"

class Board : public ObjectGroup {
public:
	enum class Type{
		Empty,
		Rectangle,
		Round
	};

	Board(const char *name, Type type, Vec2 innerSize, float border, bool originTop);
	Board() {}

	void Load(File &file);
	void Save(File &file) const;

	void SetName(const char *name);
	const char *GetName() const;
	const Vec2 &GetSize() const;

	bool IsMultilayer() const;
	bool ToggleMultilayer();

	bool IsLayerVisible(uint8_t layer) const;
	bool ToggleLayerVisible(uint8_t layer);

	bool GetCurrentLayerGround() const;
	bool ToggleCurrentLayerGround();

	uint8_t GetSelectedLayer() const;
	bool IsSelectedLayerCopper() const;
	void SetSelectedLayer(uint8_t layer);

	Vec2 ToGrid(const Vec2 &vec) const;

	double GetGrid() const;
	void SetGrid(double grid);
	void UpdateGrid(bool shift, bool ctrl);
	void SnapSelectedToGrid();
	void UpdateCamera(const Vec2 &delta);
	void Zoom(float ratio, const Vec2 &mouse);
	void ZoomBoard(const Vec2 &screenSize);
	void ZoomObjects(const Vec2 &screenSize);
	void ZoomSelection(const Vec2 &screenSize);

	void Draw(const Settings &settings, const Vec2 &screenSize) const;
	void DrawGrid(const Settings &settings, const Vec2 &screenSize) const;

	Vec2 ConvertToCoords(const Vec2 &vec) const;
	Vec2 ConvertFromCoords(const Vec2 &vec) const;

	void SetOrigin(const Vec2 &origin);
	void SetOriginTop();
	void SetOriginBottom();

	ImageConfigs images;

private:
	void ZoomAABB(const Vec2 &screenSize, const AABB &aabb);

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
	Vec2 origin;
};

inline const char *Board::GetName() const {
	return name;
}
inline const Vec2 &Board::GetSize() const {
	return size;
}
inline void Board::SetName(const char *_name) {
	strncpy(name, _name, 30);
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
inline void Board::SetOrigin(const Vec2 &newOrigin) {
	origin = newOrigin;
}
inline void Board::SetOriginTop() {
	origin = Vec2(0.0f, 0.0f);
}
inline void Board::SetOriginBottom() {
	origin = Vec2(0.0f, size.y);
}
