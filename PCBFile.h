#ifndef PCBFILE_H
#define PCBFILE_H
#include "Board.h"
#include <vector>

struct ProjectInfo {
    char title[100];
    char author[100];
    char company[100];
    char comment[1900];
};

class PCBFile {
public:
    PCBFile();
    ~PCBFile();
    void SetDefaults();

    void AddBoard(Board b);
    void DeleteSelectedBoard();
    Board &GetSelectedBoard();
    void MoveBoardToLeft();
    void MoveBoardToRight();
    void MoveBoardToBegin();
    void MoveBoardToEnd();
    size_t GetNumBoards() const;
    void SelectBoard(size_t num);
    ProjectInfo info;

protected:

private:
    int selected_board=0;
    vector<Board>boards;
};

#endif // PCBFILE_H
