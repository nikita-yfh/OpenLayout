#include "Text.h"
#include "GLUtils.h"
#include "Utils.h"
#include "../Gerber.h"

#include <map>
#include <cstring>
#include <cmath>

// ---------------------------------------------------------------------------
// Built-in stroke font
//
// Glyphs live on a 4-wide, 6-tall grid with the origin at the baseline / left
// edge and Y pointing up. Each glyph is a list of polylines (strokes). Lower
// case maps to upper case; unknown characters render blank. Monospaced advance.
// ---------------------------------------------------------------------------

static const float kCapHeight = 6.0f;
static const float kAdvance   = 5.0f;

typedef std::vector<std::vector<Vec2>> Glyph;

static const Glyph &GlyphFor(char c) {
	static const Glyph empty;
	static std::map<char, Glyph> font;
	if(font.empty()) {
		font['A'] = {{{0,0},{2,6},{4,0}}, {{1,2},{3,2}}};
		font['B'] = {{{0,0},{0,6},{3,6},{4,5},{4,4},{3,3},{0,3}},
		             {{3,3},{4,2},{4,1},{3,0},{0,0}}};
		font['C'] = {{{4,1},{3,0},{1,0},{0,1},{0,5},{1,6},{3,6},{4,5}}};
		font['D'] = {{{0,0},{0,6},{3,6},{4,5},{4,1},{3,0},{0,0}}};
		font['E'] = {{{4,6},{0,6},{0,0},{4,0}}, {{0,3},{3,3}}};
		font['F'] = {{{4,6},{0,6},{0,0}}, {{0,3},{3,3}}};
		font['G'] = {{{4,5},{3,6},{1,6},{0,5},{0,1},{1,0},{3,0},{4,1},{4,3},{2,3}}};
		font['H'] = {{{0,0},{0,6}}, {{4,0},{4,6}}, {{0,3},{4,3}}};
		font['I'] = {{{1,6},{3,6}}, {{2,6},{2,0}}, {{1,0},{3,0}}};
		font['J'] = {{{4,6},{4,1},{3,0},{1,0},{0,1}}};
		font['K'] = {{{0,0},{0,6}}, {{4,6},{0,3},{4,0}}};
		font['L'] = {{{0,6},{0,0},{4,0}}};
		font['M'] = {{{0,0},{0,6},{2,3},{4,6},{4,0}}};
		font['N'] = {{{0,0},{0,6},{4,0},{4,6}}};
		font['O'] = {{{1,0},{0,1},{0,5},{1,6},{3,6},{4,5},{4,1},{3,0},{1,0}}};
		font['P'] = {{{0,0},{0,6},{3,6},{4,5},{4,4},{3,3},{0,3}}};
		font['Q'] = {{{1,0},{0,1},{0,5},{1,6},{3,6},{4,5},{4,1},{3,0},{1,0}}, {{2,2},{4,0}}};
		font['R'] = {{{0,0},{0,6},{3,6},{4,5},{4,4},{3,3},{0,3}}, {{2,3},{4,0}}};
		font['S'] = {{{4,5},{3,6},{1,6},{0,5},{0,4},{1,3},{3,3},{4,2},{4,1},{3,0},{1,0},{0,1}}};
		font['T'] = {{{0,6},{4,6}}, {{2,6},{2,0}}};
		font['U'] = {{{0,6},{0,1},{1,0},{3,0},{4,1},{4,6}}};
		font['V'] = {{{0,6},{2,0},{4,6}}};
		font['W'] = {{{0,6},{1,0},{2,3},{3,0},{4,6}}};
		font['X'] = {{{0,0},{4,6}}, {{0,6},{4,0}}};
		font['Y'] = {{{0,6},{2,3},{4,6}}, {{2,3},{2,0}}};
		font['Z'] = {{{0,6},{4,6},{0,0},{4,0}}};

		font['0'] = {{{1,0},{0,1},{0,5},{1,6},{3,6},{4,5},{4,1},{3,0},{1,0}}, {{1,1},{3,5}}};
		font['1'] = {{{1,4},{2,6},{2,0}}, {{1,0},{3,0}}};
		font['2'] = {{{0,5},{1,6},{3,6},{4,5},{4,4},{0,0},{4,0}}};
		font['3'] = {{{0,6},{4,6},{2,3}}, {{2,3},{4,2},{4,1},{3,0},{1,0},{0,1}}};
		font['4'] = {{{3,0},{3,6},{0,2},{4,2}}};
		font['5'] = {{{4,6},{0,6},{0,3},{3,3},{4,2},{4,1},{3,0},{1,0},{0,1}}};
		font['6'] = {{{4,5},{3,6},{1,6},{0,5},{0,1},{1,0},{3,0},{4,1},{4,2},{3,3},{0,3}}};
		font['7'] = {{{0,6},{4,6},{2,0}}};
		font['8'] = {{{1,3},{0,4},{0,5},{1,6},{3,6},{4,5},{4,4},{3,3},{1,3},{0,2},{0,1},{1,0},{3,0},{4,1},{4,2},{3,3}}};
		font['9'] = {{{0,1},{1,0},{3,0},{4,1},{4,5},{3,6},{1,6},{0,5},{0,4},{1,3},{4,3}}};

		font['.'] = {{{2,0},{2,1}}};
		font[','] = {{{2,1},{2,0},{1,-1}}};
		font['-'] = {{{1,3},{3,3}}};
		font['_'] = {{{0,0},{4,0}}};
		font['+'] = {{{2,1},{2,5}}, {{0,3},{4,3}}};
		font['='] = {{{0,2},{4,2}}, {{0,4},{4,4}}};
		font['/'] = {{{0,0},{4,6}}};
		font['\\']= {{{0,6},{4,0}}};
		font[':'] = {{{2,1},{2,2}}, {{2,4},{2,5}}};
		font['('] = {{{3,6},{1,4},{1,2},{3,0}}};
		font[')'] = {{{1,6},{3,4},{3,2},{1,0}}};
		font['#'] = {{{1,0},{1,6}}, {{3,0},{3,6}}, {{0,2},{4,2}}, {{0,4},{4,4}}};
		font['*'] = {{{2,2},{2,6}}, {{0,3},{4,5}}, {{4,3},{0,5}}};
		font['!'] = {{{2,6},{2,2}}, {{2,1},{2,0}}};
		font['?'] = {{{0,5},{1,6},{3,6},{4,5},{4,4},{2,3},{2,2}}, {{2,1},{2,0}}};
	}

	if(c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	auto it = font.find(c);
	return it == font.end() ? empty : it->second;
}

// ---------------------------------------------------------------------------

Text::Text(uint8_t _layer, float _groundDistance, float _width, const Vec2 &_position,
           float _height, const char *_text, bool _mirror)
	: LineObject(_layer, _groundDistance, _width), height(_height),
	  angle(0.0f), mirror(_mirror), position(_position) {
	strncpy(text, _text, sizeof(text) - 1);
	text[sizeof(text) - 1] = '\0';
}

Text *Text::Clone() const {
	return new Text(*this);
}

void Text::BuildSegments(std::vector<Seg> &segs) const {
	float scale = height / kCapHeight;
	int len = strlen(text);
	float total = len * kAdvance;

	auto transform = [&](float gridX, float gridY) -> Vec2 {
		if(mirror)
			gridX = total - gridX;
		Vec2 p(gridX * scale, -gridY * scale);   // grid Y is up; board Y is down
		return position + p.Rotate(angle);
	};

	float pen = 0.0f;
	for(int i = 0; i < len; i++) {
		const Glyph &glyph = GlyphFor(text[i]);
		for(const std::vector<Vec2> &stroke : glyph)
			for(size_t j = 0; j + 1 < stroke.size(); j++)
				segs.push_back({ transform(pen + stroke[j].x,   stroke[j].y),
				                 transform(pen + stroke[j+1].x, stroke[j+1].y) });
		pen += kAdvance;
	}
}

void Text::Draw(float halfWidth) const {
	std::vector<Seg> segs;
	BuildSegments(segs);
	for(const Seg &s : segs) {
		if(s.a != s.b) {
			Vec2 n = (s.b - s.a).Normal(halfWidth);
			glBegin(GL_QUADS);
			glutils::Vertex(s.a - n);
			glutils::Vertex(s.a + n);
			glutils::Vertex(s.b + n);
			glutils::Vertex(s.b - n);
			glEnd();
		}
		glutils::DrawCircle(s.a, halfWidth);   // round caps / joints
		glutils::DrawCircle(s.b, halfWidth);
	}
}

void Text::DrawObject() const {
	Draw(width / 2.0f);
}

void Text::DrawGroundDistance() const {
	Draw(width / 2.0f + groundDistance);
}

AABB Text::GetAABB() const {
	std::vector<Seg> segs;
	BuildSegments(segs);
	if(segs.empty())
		return AABB(position, position);
	AABB box = AABB::Invalid();
	float hw = width / 2.0f;
	for(const Seg &s : segs) {
		box |= s.a;
		box |= s.b;
	}
	return box.Expand(hw);
}

Vec2 Text::GetPosition() const {
	return position;
}

bool Text::TestPoint(const Vec2 &point) const {
	std::vector<Seg> segs;
	BuildSegments(segs);
	float hw = width / 2.0f;
	if(hw < 0.15f) hw = 0.15f;   // keep thin labels easy to click
	for(const Seg &s : segs)
		if(utils::PointInPolySegment(point, s.a, s.b, hw))
			return true;
	return false;
}

void Text::Rotate(const Vec2 &center, float a) {
	position = position.Rotate(a, center);
	angle += a;
}

void Text::MirrorHorizontal(float x) {
	position.x = 2.0f * x - position.x;
	mirror = !mirror;
	angle = -angle;
}

void Text::MirrorVertical(float y) {
	position.y = 2.0f * y - position.y;
	mirror = !mirror;
	angle = M_PI - angle;
}

void Text::Move(const Vec2 &d) {
	position += d;
}

void Text::ExportGerber(GerberWriter &w) const {
	std::vector<Seg> segs;
	BuildSegments(segs);
	if(segs.empty())
		return;
	w.select(w.circle(width));
	for(const Seg &s : segs) {
		w.moveTo(s.a);
		w.lineTo(s.b);
	}
}

void Text::SaveObject(File &file) const {
	file.Write<uint8_t>(Object::TEXT);
	position.SavePosition(file);
	file.WriteMm<uint32_t>(height);
	file.WriteMm<uint32_t>(width);
	file.WriteAngle(angle);
	file.Write<uint8_t>(layer + 1);
	file.Write<uint8_t>(mirror);
	file.Write<uint16_t>(componentID);
	file.WriteMm<uint32_t>(groundDistance);
	file.Write<uint8_t>(soldermask);
	file.WriteString(marker);
	file.WriteString(text);

	SaveGroups(file);
}

void Text::LoadObject(File &file) {
	position.LoadPosition(file);
	height = file.ReadMm<uint32_t>();
	width = file.ReadMm<uint32_t>();
	angle = file.ReadAngle();
	layer = file.Read<uint8_t>() - 1;
	mirror = file.Read<uint8_t>();
	componentID = file.Read<uint16_t>();
	groundDistance = file.ReadMm<uint32_t>();
	soldermask = file.Read<uint8_t>();
	file.ReadString(marker);
	file.ReadString(text);

	LoadGroups(file);
}
