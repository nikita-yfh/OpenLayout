#pragma once

#include "ObjectGroup.h"
#include "Settings.h"
#include "Vec2.h"

#include <QDialog>
#include <QOpenGLWidget>

class QTabWidget;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QRadioButton;
class QComboBox;
class QLabel;

// Small read-only canvas that fits and draws a generated ObjectGroup.
class FormPreview : public QOpenGLWidget {
public:
    FormPreview(const Settings &settings, uint8_t layer, QWidget *parent = nullptr);
    void SetGroup(const ObjectGroup *group) { this->group = group; update(); }

protected:
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    const Settings &settings;
    uint8_t layer;
    const ObjectGroup *group = nullptr;
    Vec2 currentSize;
};

// "Special form" generator: builds tracks/polys/circles for a regular polygon,
// a spiral, or a panelisation frame, previews them, and hands the result to the
// caller via GetObjects() so it can be placed on the board.
class FormDialog : public QDialog {
    Q_OBJECT
public:
    FormDialog(const Settings &settings, const Vec2 &boardSize, uint8_t layer,
               QWidget *parent = nullptr);
    ~FormDialog() override { delete group; }

    const ObjectGroup &GetObjects() const { return *group; }

private:
    void Regenerate();          // rebuild `group` from the active tab's inputs
    void AutosizeFrame();       // fit the frame to the board size

    QWidget *CreatePolygonTab();
    QWidget *CreateSpiralTab();
    QWidget *CreateFrameTab();

    const Settings &settings;
    Vec2 boardSize;
    uint8_t layer;
    ObjectGroup *group = nullptr;

    QTabWidget *tabs;
    FormPreview *preview;

    // Polygon
    QDoubleSpinBox *polyRadius, *polyWidth, *polyOffset;
    QSpinBox *polyCorners;
    QCheckBox *polyRays, *polyFill;

    // Spiral
    QDoubleSpinBox *spiralRadius, *spiralDistance, *spiralWidth, *spiralTurns;
    QRadioButton *spiralRound, *spiralSquare;
    QLabel *spiralDiameter;

    // Frame
    QSpinBox *frameColumns, *frameRows;
    QComboBox *frameColumnsDef, *frameRowsDef, *frameColumnsSide, *frameRowsSide;
    QDoubleSpinBox *frameWidth, *frameHeight;

    enum { TYPE_POLYGON, TYPE_SPIRAL, TYPE_FRAME };
    enum { SIDE_NO, SIDE_A, SIDE_B, SIDE_BOTH };   // column: top/bottom; row: left/right
};
