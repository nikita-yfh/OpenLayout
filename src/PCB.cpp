#include "PCB.h"

static const uint32_t magic = 0xFFAA3306;

PCB::PCB() {
	boards = nullptr;
	current = nullptr;
	activeTab = 0;
	rubberband = RUBBERBAND_DISABLED;
	capture = true;
	metallization = false;
	padShape = 0;
}
PCB::~PCB() {
	while(boards) {
		Board *child = boards->next;
		delete boards;
		boards = child;
	}
}
uint32_t PCB::GetBoardCount() const{
	uint32_t n = 1;
	Board *tmp = boards;
	while(tmp = tmp->next) n++;
	return n;
}
bool PCB::HasOneBoard() const {
	return boards->next == nullptr;
}
void PCB::SetBoard(uint32_t n) {
	activeTab = n;
	current = boards;
	if(!n)
		return;
	while(--n) current = current->next;
}
Board *PCB::GetSelectedBoard() {
	return current;
}
Board *PCB::GetLastBoard() {
	Board *tmp = boards;
	if(tmp == nullptr)
		return nullptr;
	while(tmp->next) tmp = tmp->next;
	return tmp;
}
void PCB::AddBoard(Board *board) {
	board->next = boards;
	current = boards = board;
}

void PCB::DeleteSelectedBoard() {
	if(HasOneBoard())
		return;
	if(current == boards) {
		boards = current->next;
		delete current;
		current = boards;
	} else {
		for(Board *board = boards; board; board = board->next) {
			if(board->next == current) {
				board->next = current->next;
				delete current;
				current = board;
			}
		}
	}
}

void PCB::Save(File &file) const{
	file.Write<uint32_t>(magic);
	file.Write<uint32_t>(GetBoardCount());
	for(Board *board = boards; board; board = board->next)
		board->Save(file);
	file.Write<uint32_t>(activeTab);
	info.Save(file);
}
bool PCB::Load(File &file) {
	this->~PCB();
	if(file.Read<uint32_t>() != magic)
		return false;
	uint32_t count = file.Read<uint32_t>();
	for(int i = 0; i < count; i++) {
		Board *board = new Board();
		board->Load(file);
		AddBoard(board);
	}
	SetBoard(file.Read<uint32_t>());
	info.Load(file);
	return true;
}
