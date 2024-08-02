#pragma once

#include "Locale.h"

#include <QToolBar>
#include <QSignalMapper>

class ToolPanel : public QToolBar {
    Q_OBJECT
public:
    ToolPanel(QWidget *parent = nullptr);
    ~ToolPanel() {}

    enum ToolType {
        TOOL_EDIT,
        TOOL_ZOOM,
        TOOL_TRACK,
        TOOL_PAD,
        TOOL_SMD_PAD,
        TOOL_CIRCLE,
        TOOL_RECT,
        TOOL_ZONE,
        TOOL_SPECIAL_FORM,
        TOOL_TEXT,
        TOOL_SOLDER_MASK,
        TOOL_CONNECTIONS,
        TOOL_AUTOROUTE,
        TOOL_TEST,
        TOOL_MEASURE,
        TOOL_PHOTOVIEW,

        TOOL_COUNT
    };

    static const char *toolNames[TOOL_COUNT];

    enum PadType {
        PAD_CIRCLE,
        PAD_OCTAGON,
        PAD_SQUARE,
        PAD_CIRCLE_H,
        PAD_OCTAGON_H,
        PAD_SQUARE_H,
        PAD_CIRCLE_V,
        PAD_OCTAGON_V,
        PAD_SQUARE_V,

        PAD_COUNT
    };

    inline bool GetCurrentMetallization() const {
        return currentMetallization;
    }

    inline PadType GetCurrentPadType() const {
        return currentPadType;
    }

    inline bool GetCurrentRectFill() const {
        return currentRectFill;
    }
private:
    QAction *actions[TOOL_COUNT];
    QAction *padTypes[PAD_COUNT];
    QAction *metallization;

    QSignalMapper *padMapper;

    bool currentRectFill;
    bool currentMetallization;
    PadType currentPadType;

    void CreatePadTypeMenu();
    void CreateRectTypeMenu();
public slots:
    void OnChangeOrientation(Qt::Orientation orientation);
    void OnToggleMetallization(bool);
    void OnSettedPadType(int);
    void OnSettedRectTrack();
    void OnSettedRectZone();
};
