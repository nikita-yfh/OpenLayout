#pragma once
#include <stdint.h>
#include "ProjectInfo.h"
#include "Board.h"
#include "File.h"
#include "Array.h"

class PCB : public Array<Board*> {
public:
	PCB() : activeTab(0) {}

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

	ProjectInfo info;
private:
	uint32_t activeTab;
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
