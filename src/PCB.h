#pragma once
#include <stdint.h>
#include "ProjectInfo.h"
#include "Board.h"
#include "File.h"

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
private:
	Board *GetLastBoard();

	const uint32_t magic=0xFFAA3306;
	Board *boards;
	uint32_t activeTab;
	ProjectInfo info;

	Board *current;
};

