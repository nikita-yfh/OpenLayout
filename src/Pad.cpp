#include "Pad.h"

template<typename T>
static inline void Swap(T &a, T &b) {
	T c = a;
	a = b;
	b = c;
}

void Pad::WriteArray(File &file, const Vec2 *points, uint32_t count, const Vec2 &shift) {
	file.Write<uint32_t>(count);
	for(int i = 0; i < count; i++)
		(points[i] - shift).Save<float>(file);
}
uint32_t Pad::ReadArray(File &file, Vec2 *points, const Vec2 &shift) {
	uint32_t count = file.Read<uint32_t>();
	for(int i = 0; i < count; i++) {
		points[i].Load<float>(file);
		points[i] -= shift;
	}
	bool wrap = false;
	if(count >= 3) {
		float a[3] = {
			points[0].Angle(),
			points[1].Angle(),
			points[2].Angle()
		};
		if(a[2] > a[0])
			wrap = (a[1] > a[0] && a[1] < a[2]);
		else 
			wrap = (a[1] > a[0] || a[1] < a[2]);
		if(wrap)
			for(int i = 0; i < count / 2; i++)
				Swap(points[i], points[count - i - 1]);
	}
	return count;
}

void Pad::WriteSymmetricalArray(File &file, const Vec2 *points, uint32_t count, const Vec2 &shift) {
	file.Write<uint32_t>(count * 2);
	for(int i = 0; i < count * 2; i++) {
		if(i < count)
			(points[i] + shift).Save<float>(file);
		else
			(shift - points[(i+count) % count]).Save<float>(file);
	}
}
