#pragma once

#include "Board.h"
#include "Settings.h"
#include "Vec2.h"

#include <QOpenGLWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class MainCanvas : public QOpenGLWidget {
    Q_OBJECT
public:
    MainCanvas(Board *board, Settings &settings, QWidget *parent);
    ~MainCanvas() {};

    void PlaceObject(Object *object);
    void PlaceObjectGroup(const ObjectGroup &objects);

public slots:
    void FinishCreating();
signals:
    void ToolChanged(int tool);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h);

    bool eventFilter(QObject *obj, QEvent *event);

private:
	bool shiftFlag;
	bool ctrlFlag;

	Vec2 dragPosition;
	Vec2 mousePosition;
	Vec2 lastPlacedPoint;
	Vec2 mouseDelta;
	uint32_t placedPointCount;

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
    Vec2 currentSize;
    Board *board;
    Settings &settings;
};
