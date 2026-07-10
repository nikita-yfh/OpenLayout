#pragma once

#include "Board.h"
#include "Settings.h"
#include "Vec2.h"
#include "Pad.h"

#include <QOpenGLWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class MainCanvas : public QOpenGLWidget {
    Q_OBJECT
public:
    MainCanvas(Board *board, Settings &settings, QWidget *parent);
    ~MainCanvas() { delete clipboard; }

    void PlaceObject(Object *object);
    void PlaceObjectGroup(const ObjectGroup &objects);
    void CenterOn(const Vec2 &point) { board->CenterOn(point, currentSize); update(); }

    void SetBoard(Board *_board) {
        board = _board;
        // Reset interaction state so it can't dangle against the new board.
        placedPointCount = 0;
        lastPlacedPoint = Vec2::Invalid();
        mousePosition = Vec2::Invalid();
        selecting = false;
        dragStarted = false;
        firstConnectionPad = secondConnectionPad = nullptr;
        firstConnectionPointSelected = false;
        update();
    }

    void Copy();
    void Cut();
    void Paste();
    void Duplicate();

public slots:
    void FinishCreating();
signals:
    void ToolChanged(int tool);
    void BeforeChange();        // emitted before an interactive edit (for undo)
    void Measured(const QString &text);   // measure-tool readout for the status bar

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h);

    bool eventFilter(QObject *obj, QEvent *event);

private:
	Vec2 dragPosition;
	Vec2 mousePosition;
	Vec2 lastPlacedPoint;
	Vec2 mouseDelta;
	uint32_t placedPointCount;
    Pad *firstConnectionPad;
    Pad *secondConnectionPad;
    bool firstConnectionPointSelected;

    void OnWheelEvent(QWheelEvent *event);
    void OnLeftDownEvent(QMouseEvent *event);
    void OnLeftUpEvent(QMouseEvent *event);
    void OnRightDownEvent(QMouseEvent *event);
    void OnMiddleDownEvent(QMouseEvent *event);
    void OnMouseMotionEvent(QMouseEvent *event);
    void OnKeyPressEvent(QKeyEvent *event);
    void OnKeyReleaseEvent(QKeyEvent *event);
    void OnLeaveWindowEvent();
    void UpdateBoardGrid(QKeyEvent *event);

    void BuildTrackEnd();
    void BuildRect();
    void BuildCircle();
    void DrawSelectionRect() const;
    void DrawMeasure() const;
    void EmitMeasure();

    bool selecting = false;
    bool dragStarted = false;
    bool measuring = false;
    Vec2 measureStart;
    Vec2 measureEnd;
    Vec2 selectStart;
    Vec2 selectEnd;
    Vec2 currentSize;
    Board *board;
    Settings &settings;
    ObjectGroup *clipboard = nullptr;
};
