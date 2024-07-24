#include <QToolBar>

class ToolPanel : public QToolBar {
    Q_OBJECT
public:
    ToolPanel(QWidget *parent = nullptr);
    ~ToolPanel() {}

    enum {
        Edit,
        Zoom,
        Track,
        Pad,
        SMDPad,
        Circle,
        Rect,
        Zone,
        SpecialForm,
        Text,
        SolderMask,
        Connections,
        Autoroute,
        Test,
        Measure,
        Photoview,

        TOOL_COUNT
    };
private:
    QAction *actions[TOOL_COUNT];
public slots:
    void onChangeOrientation(Qt::Orientation orientation);
};
