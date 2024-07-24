#include <QToolBar>
#include <QToolButton>

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
    void onButtonClicked();
};

class MyToolButton : public QToolButton {
    Q_OBJECT
public:
    MyToolButton(QWidget *parent, QIcon icon, const char *text);
public slots:
    void onChangeOrientation(Qt::Orientation orientation);
};
