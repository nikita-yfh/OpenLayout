#pragma once
#include "File.h"

struct PadSize {
	float out;
	float in;

	PadSize() {}
	PadSize(float _out, float _in) {
		in = _in;
		out = _out;
	}

	void Set(float _out, float _in) {
		if(_in > _out) {
			if(out == _out)
				in = out = _in;
			else
				in = out = _out;
		} else {
			in = _in;
			out = _out;
		}
	}

	bool IsHole() const {
		return in == out;
	}

	bool operator==(const PadSize &other) const {
		return other.in == in && other.out == out;
	}

	const PadSize &operator=(const PadSize &other) {
		Set(other.out, other.in);
		return *this;
	}

	bool operator<(const PadSize &other) const {
		if(out < other.out)
			return true;
		else if(out == other.out && in < other.in)
			return true;
		return false;
	}

	void Save(File &file) const {
		file.WriteMm<float>(out / 2.0f);
		file.WriteMm<float>(in / 2.0f);
	}

	void Load(File &file) {
		out = file.ReadMm<float>() * 2.0f;
		in  = file.ReadMm<float>() * 2.0f;
	}
};
