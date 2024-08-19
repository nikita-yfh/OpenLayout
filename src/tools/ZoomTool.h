#pragma once
#include "Tool.h"

class ZoomTool : public Tool {
public:
    ZoomTool(Board *board, const Settings &settings)
        : Tool(board, settings) {}

    virtual bool OnLeftDown     (const Vec2 &mousePos) override;
    virtual bool OnRightDown    (const Vec2 &mousePos) override;
    virtual bool OnWheelRotated (const Vec2 &mousePos, int direction) override;
private:
    const float zoomRatioWheel = 1.3f;
    const float zoomRatioButtons = 1.4f;
};
