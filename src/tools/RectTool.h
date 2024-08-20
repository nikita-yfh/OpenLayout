#pragma once
#include "Tool.h"

class RectTool : public Tool {
public:
    RectTool(Board *board, const Settings &settings)
        : Tool(board, settings) {}

    virtual bool OnLeftDown     (const Vec2 &mousePos) override;
    virtual bool OnLeftUp       (const Vec2 &mousePos) override;
    virtual bool OnRightDown    (const Vec2 &mousePos) override;
    virtual bool OnWheelRotated (const Vec2 &mousePos, int direction) override;
private:
};
