#pragma once
#include <stdint.h>
#include "ProjectInfo.h"
#include "Board.h"
#include "File.h"

enum{
	RUBBERBAND_DISABLED,
	RUBBERBAND_SMALL,
	RUBBERBAND_LARGE
};

class PCB {
public:
	PCB();
	~PCB();

	void Save(File &file) const;
	bool Load(File &file);

	void AddBoard(Board *board);
	uint32_t GetBoardCount() const;
	void SetBoard(uint32_t n);
	Board *GetSelectedBoard();

	bool GetCaptureMode() const;
	bool ToggleCaptureMode();

	uint8_t GetRubberbandMode() const;
	uint8_t ToggleRubberbandMode();

	bool GetMetallization() const;
	bool ToggleMetallization();

	uint8_t GetPadShape() const;
	void SetPadShape(uint8_t shape);

	const Vec2 &GetMousePosition() const;
	
	ProjectInfo info;
private:
	Board *GetLastBoard();

	Board *boards;
	uint32_t activeTab;

	bool capture;
	bool metallization;
	uint8_t rubberband;
	uint8_t padShape;
	Vec2 mousePosition;

	Board *current;
};

inline bool PCB::GetCaptureMode() const {
	return capture;
}
inline bool PCB::ToggleCaptureMode() {
	capture = !capture;
	return capture;
}
inline uint8_t PCB::GetRubberbandMode() const {
	return rubberband;
}
inline uint8_t PCB::ToggleRubberbandMode() {
	rubberband = (rubberband + 1) % 3;
	return rubberband;
}
inline bool PCB::GetMetallization() const {
	return metallization;
}
inline bool PCB::ToggleMetallization() {
	metallization = !metallization;
	return metallization;
}
inline uint8_t PCB::GetPadShape() const {
	return padShape;
}
inline void PCB::SetPadShape(uint8_t shape) {
	padShape = shape;
}
inline const Vec2 &PCB::GetMousePosition() const {
	return mousePosition;
}
