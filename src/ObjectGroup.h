#pragma once
#include "Object.h"
#include "ColorScheme.h"
#include <vector>
#include <string>
#include <utility>

class ObjectGroup {
public:
	ObjectGroup(const ObjectGroup &other);
	ObjectGroup();

	~ObjectGroup();

	void AddObjectBegin(Object *object);
	void AddObjectEnd(Object *object);
	Object *AddObjectEnd(Object *object, Object *last);
	void RemoveObject(Object *object);   // unlink and delete a single object

	void PlaceObject(Object *object);
	void PlaceGroup(const ObjectGroup &group, const Vec2 &position);

	bool IsEmpty() const;

	Object *GetFirstPlaced();
	Object *GetFirstSelected();
	const Object *GetFirstSelected() const;
	bool IsSelected() const;
	bool IsSelectedTwo() const;

	Object *GetObjects();
	AABB GetObjectsAABB() const;

	// Returns a new heap group with clones of the selected objects, centered
	// on (0, 0) so it can be dropped anywhere via PlaceGroup. nullptr if the
	// selection is empty.
	ObjectGroup *CopySelected() const;

	void InvertSelectionGroup(Object *object);
	void SelectAll();
	void UnselectAll();
	void SelectInRect(const AABB &rect);   // select objects fully inside rect

	bool CanGroup() const;
	bool CanUngroup() const;
	void GroupSelected();
	void UngroupSelected();

	void MovePlaced(const Vec2 &d);
	void MoveSelected(const Vec2 &d);
	void RotateSelected(float angle);
	void MirrorSelectedHorizontal();
	void MirrorSelectedVertical();

	void AlignSelected(Vec2(*callback)(const AABB&, const AABB&));

	void SetSelectedToLayer(uint8_t layer); // move the selected objects to a layer
	void ChangeSide(float mirrorX);         // flip to the other board side
	void ResetSoldermask();                 // clear the soldermask flag on all objects
	void SelectConnected(Object *start);    // select the copper net touching `start` (continuity test)
	std::vector<std::pair<Vec2, std::string>> CheckDRC(float clearance) const;  // {position, message}
	void FillZone(Object *zone, float clearance, float lineWidth);  // copper-pour a Poly zone
	void ToggleSoldermask(Object *object);  // include/exclude one object from the solder mask
	void DrawSoldermaskMarked() const;      // overlay objects flagged for the solder mask
	void SetSelectedWidth(float width);     // set line width on the selected objects
	void ArraySelected(int cols, int rows, const Vec2 &spacing); // duplicate in a grid
	void DeleteSelected();
	void DeleteOutside(const AABB &keep);   // delete objects lying fully outside keep
	void RemoveAllConnections();            // drop every pad rubber-band connection
	void CancelPlacing();

	void DrawObjects(const ColorScheme &colors, uint8_t activeLayer, bool selected, const bool *layerVisible = nullptr) const;
	void DrawObjectsPhoto(uint8_t activeLayer, const bool *layerVisible = nullptr) const;   // opaque, photo-realistic
	void DrawGroundDistance(uint8_t activeLayer) const;
	void DrawDrillings(const bool *layerVisible = nullptr) const;

	enum Layer {
		LAYER_C1,
		LAYER_S1,
		LAYER_C2,
		LAYER_S2,
		LAYER_I1,
		LAYER_I2,
		LAYER_O,
	};
protected:
	uint32_t GetFreeGroup() const;
	uint32_t GetMaxSelectedGroup() const;
	Vec2 GetSelectedCenter() const;
	AABB GetSelectedAABB() const;

	Object *GetLast();
	uint32_t GetObjectCount() const;
	Object *GetObject(uint32_t n);

	Object *objects;
};

inline Object *ObjectGroup::GetObjects() {
	return objects;
}
inline bool ObjectGroup::IsEmpty() const {
	return objects == nullptr;
}
