#pragma once
#include "Tool.h"
#include "Pad.h"

class ConnectionsTool : public Tool {
public:
    ConnectionsTool(Board *board, const Settings &settings)
        : Tool(board, settings), firstConnectionPad(nullptr),
          secondConnectionPad(nullptr), firstConnectionPointSelected(false) {}

    virtual bool OnLeftDown   (const Vec2 &mousePos) override;
    virtual bool OnMouseMoved (const Vec2 &mousePos) override;
private:
    Pad *firstConnectionPad;
    Pad *secondConnectionPad;
    bool firstConnectionPointSelected;
};
