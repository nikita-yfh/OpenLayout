#pragma once
#include "Tool.h"

class PadTool : public Tool {
public:
    PadTool(Board *board, const Settings &settings)
        : Tool(board, settings), prevMousePos(Vec2::Invalid()) {}
    virtual ~PadTool();

    virtual bool OnLeftDown     (const Vec2 &mousePos) override;
    virtual bool OnMouseMoved   (const Vec2 &mousePos) override;

    virtual Pad *CreatePad(const Vec2 &position) = 0;
private:
    Vec2 prevMousePos;
};

class SMDPadTool : public PadTool {
public:
    SMDPadTool(Board *board, const Settings &settings)
        : PadTool(board, settings) {}
    virtual Pad *CreatePad(const Vec2 &position) override;
};

class THTPadTool : public PadTool {
public:
    THTPadTool(Board *board, const Settings &settings)
        : PadTool(board, settings) {}
    virtual Pad *CreatePad(const Vec2 &position) override;
};

