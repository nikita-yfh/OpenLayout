#pragma once
#include "Tool.h"

class EditTool : public Tool {
public:
    EditTool(Board *board, const Settings &settings)
        : Tool(board, settings), clickPosition(Vec2::Invalid()),
          drag(Vec2::Invalid()), shiftPressed(false), leftPressed(false) {}

    virtual bool OnLeftDown   (const Vec2 &mousePos) override;
    virtual bool OnLeftUp     (const Vec2 &mousePos) override;
    virtual bool OnMouseMoved (const Vec2 &mousePos) override;
    virtual bool OnModifiersChanged(bool shift, bool ctrl) override;

private:
    Vec2 clickPosition;
    Vec2 lastMousePos;
    Vec2 drag;
    bool shiftPressed;
    bool leftPressed;
};
