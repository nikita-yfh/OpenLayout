#pragma once

#include "Board.h"
#include "Settings.h"
#include "Vec2.h"
#include "Array.h"

class Tool;
typedef Tool *(*toolCreate_t)(Board*, const Settings&);
typedef const char **xpm_t;

#define REGISTER_TOOL(TOOL, ID, NAME, XPM)                                  \
    static Tool *_CreateTool(Board *board, const Settings &settings) {      \
        return new TOOL(board, settings);                                   \
    }                                                                       \
    static int _dummyId = Tool::RegisterTool(_CreateTool, ID, NAME, XPM);  


class Tool {
public:
    Tool(Board *_board, const Settings &_settings)
        : board(_board), settings(_settings) {}
    virtual ~Tool() {}

    virtual void OnDraw(const Vec2 &currentSize) const;

    virtual bool OnLeftDown         (const Vec2 &mousePos)                { return false; }
    virtual bool OnLeftUp           (const Vec2 &mousePos)                { return false; }
    virtual bool OnRightDown        (const Vec2 &mousePos)                { return false; }
    virtual bool OnRightUp          (const Vec2 &mousePos)                { return false; }
    virtual bool OnMouseMoved       (const Vec2 &mousePos)                { return false; }
    virtual bool OnModifiersChanged (bool shift, bool ctrl)               { return false; }
    virtual bool OnSpaceDown        ()                                    { return false; }
    virtual bool OnWheelRotated     (const Vec2 &mousePos, int direction) { return false; }

    static int RegisterTool(toolCreate_t func, int id, const char *name, xpm_t icon);

    struct Uncreated {
        toolCreate_t func;
        int id;
        const char *name;
        xpm_t icon;
    };

    static inline const Array<Uncreated> &GetTools() {
        return tools;
    }
protected:
    Board *board;
    const Settings &settings;

private:
    static Array<Uncreated> tools;
};
