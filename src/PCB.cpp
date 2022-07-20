#include "PCB.h"
#include "Utils.h"

static const uint32_t magic = 0xFFAA3306;

void PCB::AddBoard(Board *board) {
	activeTab = Size();
	Add(board);
}

void PCB::DeleteSelectedBoard() {
	RemoveIndex(activeTab);
	if(activeTab == Size())
		activeTab--;
}

void PCB::MoveSelectedBoardLeft() {
	utils::Swap(items[activeTab], items[activeTab - 1]);
	activeTab--;
}

void PCB::MoveSelectedBoardRight() {
	utils::Swap(items[activeTab], items[activeTab + 1]);
	activeTab++;
}

void PCB::SetSelectedBoardLeft() {
	utils::Swap(items[activeTab], First());
	activeTab = 0;
}

void PCB::SetSelectedBoardRight() {
	utils::Swap(items[activeTab], Last());
	activeTab = Size() - 1;
}

void PCB::Save(File &file) const{
	file.Write<uint32_t>(magic);
	file.Write<uint32_t>(Size());
	for(int i = 0; i < Size(); i++)
		items[i]->Save(file);
	file.Write<uint32_t>(activeTab);
	info.Save(file);
}
bool PCB::Load(File &file) {
	if(file.Read<uint32_t>() != magic)
		return false;
	uint32_t count = file.Read<uint32_t>();
	Init(count);
	for(int i = 0; i < count; i++) {
		items[i] = new Board();
		items[i]->Load(file);
	}
	activeTab = file.Read<uint32_t>();
	info.Load(file);
	return true;
}
