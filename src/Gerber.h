#pragma once
#include <sstream>
#include <ostream>
#include <string>
#include <map>
#include <cstdio>
#include <cmath>
#include "Vec2.h"

// Minimal RS-274X (Gerber) writer. The image body is buffered so apertures
// can be collected first and emitted in the header before they are used.
// Units are millimetres, format 3.6, Y flipped to bottom-up.
class GerberWriter {
public:
	GerberWriter(float boardHeight) : height(boardHeight) {}

	// Grow every aperture by this much (mm) — used for isolation toolpaths.
	void setInflate(float f) { inflate = f; }

	int circle(float dia)         { return aperture("C," + num(dia + inflate)); }
	int rect(float w, float h)    { return aperture("R," + num(w + inflate) + "X" + num(h + inflate)); }

	void select(int d) {
		if(d != current) {
			body << "D" << d << "*\n";
			current = d;
		}
	}
	void flash(const Vec2 &p)  { body << coord(p) << "D03*\n"; }
	void moveTo(const Vec2 &p) { body << coord(p) << "D02*\n"; }
	void lineTo(const Vec2 &p) { body << coord(p) << "D01*\n"; }

	void beginRegion() { body << "G36*\n"; }
	void endRegion()   { body << "G37*\n"; }

	void write(std::ostream &out) const {
		out << "%MOMM*%\n";
		out << "%FSLAX36Y36*%\n";
		out << "%LPD*%\n";
		for(const auto &kv : defs)
			out << "%ADD" << kv.second << kv.first << "*%\n";
		out << "G01*\n";
		out << body.str();
		out << "M02*\n";
	}

private:
	int aperture(const std::string &def) {
		auto it = defs.find(def);
		if(it != defs.end())
			return it->second;
		int d = nextD++;
		defs[def] = d;
		return d;
	}
	static std::string num(float mm) {
		char buf[32];
		snprintf(buf, sizeof(buf), "%.4f", mm);
		return buf;
	}
	std::string coord(const Vec2 &p) const {
		long x = lround(p.x * 1000000.0);
		long y = lround((height - p.y) * 1000000.0);
		char buf[64];
		snprintf(buf, sizeof(buf), "X%ldY%ld", x, y);
		return buf;
	}

	float height;
	float inflate = 0.0f;
	std::ostringstream body;
	std::map<std::string, int> defs;
	int nextD = 10;
	int current = -1;
};
