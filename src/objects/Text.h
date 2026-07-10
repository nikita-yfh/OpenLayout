#pragma once
#include "LineObject.h"
#include "Vec2.h"
#include <vector>

// A vector-font text label. The glyph geometry is generated on the fly from a
// built-in stroke font, so only the parameters are stored: the string, the cap
// height, the stroke width (from LineObject), an anchor position (baseline,
// left edge), a rotation angle and a mirror flag for bottom-side silkscreen.
class Text : public LineObject {
public:
	Text() : height(2.0f), angle(0.0f), mirror(false) { text[0] = '\0'; }
	Text(uint8_t layer, float groundDistance, float width, const Vec2 &position,
	     float height, const char *text, bool mirror = false);

	virtual Text *Clone() const override;

	virtual void DrawGroundDistance() const override;
	virtual void DrawObject() const override;

	virtual void SaveObject(File &file) const override;
	virtual void LoadObject(File &file) override;

	virtual uint8_t GetType() const override { return TEXT; }
	virtual AABB GetAABB() const override;
	virtual Vec2 GetPosition() const override;
	virtual bool TestPoint(const Vec2 &point) const override;
	virtual void Rotate(const Vec2 &center, float angle) override;
	virtual void MirrorHorizontal(float x) override;
	virtual void MirrorVertical(float y) override;
	virtual void Move(const Vec2 &d) override;
	virtual void ExportGerber(GerberWriter &w) const override;

	const char *GetText() const { return text; }
	float GetHeight() const { return height; }

private:
	struct Seg { Vec2 a, b; };
	void BuildSegments(std::vector<Seg> &segs) const;   // transformed mm-space strokes
	void Draw(float halfWidth) const;

	char text[256];
	float height;   // cap height, mm
	float angle;    // rotation, radians
	bool mirror;    // horizontally flipped (other board side)

	Vec2 position;  // anchor: baseline, left edge

	friend class ObjectGroup;
};
