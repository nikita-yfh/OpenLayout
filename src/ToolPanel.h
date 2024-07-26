#pragma once
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

    inline bool getCurrentMetallization() {
        return currentMetallization;
    }

    inline PadType getCurrentPadType() {
        return currentPadType;
    }

    inline bool getCurrentRectFill() {
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

    void createPadTypeMenu();
    void createRectTypeMenu();
public slots:
    void onChangeOrientation(Qt::Orientation orientation);
    void onToggleMetallization(bool);
    void onSettedPadType(int);
    void onSettedRectTrack();
    void onSettedRectZone();
};
