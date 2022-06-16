#pragma once
#include <stdint.h>
#include "ProjectInfo.h"
#include "Board.h"
#include "File.h"
#include "Array.h"

class PCB : public Array<Board*> {
public:
	PCB();

	void Save(File &file) const;
	bool Load(File &file);

	void AddBoard(Board *board);
	void DeleteSelectedBoard();
	void MoveSelectedBoardLeft();
	void MoveSelectedBoardRight();
	void SetSelectedBoardLeft();
	void SetSelectedBoardRight();

	bool CanMoveLeft() const;
	bool CanMoveRight() const;

	void SetTab(uint32_t n);
	uint32_t GetTab() const;
	Board *GetSelectedBoard();

	bool GetMetallization() const;
	bool ToggleMetallization();

	uint8_t GetPadShape() const;
	void SetPadShape(uint8_t shape);

	const Vec2 &GetMousePosition() const;
	
	ProjectInfo info;
private:
	uint32_t activeTab;

	bool capture;
	bool metallization;
	uint8_t rubberband;
	uint8_t padShape;
	Vec2 mousePosition;
};

inline Board *PCB::GetSelectedBoard() {
	return items[activeTab];
}
inline bool PCB::CanMoveLeft() const {
	return activeTab != 0;
}
inline bool PCB::CanMoveRight() const {
	return activeTab != Size() - 1;
}
inline void PCB::SetTab(uint32_t n) {
	activeTab = n;
}
inline uint32_t PCB::GetTab() const {
	return activeTab;
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

