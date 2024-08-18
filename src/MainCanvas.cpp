#include "MainCanvas.h"

#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"
#include "Circle.h"
#include "Utils.h"

MainCanvas::MainCanvas(Board *_board, Settings &_settings, QWidget *parent)
                : board(_board), settings(_settings), QOpenGLWidget(parent),
                  placedPointCount(0), lastPlacedPoint(Vec2::Invalid()),
                  mousePosition(Vec2::Invalid()), shiftFlag(false), ctrlFlag(false) {
    installEventFilter(this);
    setMouseTracking(true);
    grabKeyboard();
}

void MainCanvas::initializeGL() {
}

void MainCanvas::paintGL() {
    board->Draw(settings, currentSize);
}

void MainCanvas::resizeGL(int w, int h) {
    currentSize.x = w;
    currentSize.y = h;
}

void MainCanvas::FinishCreating() {
	if(placedPointCount >= 3 || (placedPointCount == 2 && settings.selectedTool == TOOL_TRACK)) {
		((PolygonBase*) board->GetFirstPlaced())->points.Resize(placedPointCount);
		board->UnselectAll();
	} else
		board->CancelPlacing();
	placedPointCount = 0;
	lastPlacedPoint = Vec2::Invalid();
    repaint();
}

bool MainCanvas::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->buttons() & Qt::LeftButton)
                OnLeftDownEvent(mouseEvent);
            if(mouseEvent->buttons() & Qt::RightButton)
                OnRightDownEvent(mouseEvent);
            if(mouseEvent->buttons() & Qt::MiddleButton)
                OnMiddleDownEvent(mouseEvent);
            }
            break;
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
                OnLeftUpEvent(mouseEvent);
            }
            break;
        case QEvent::MouseMove:
            OnMouseMotionEvent(static_cast<QMouseEvent*>(event));
            break;
        case QEvent::Wheel:
            OnWheelEvent(static_cast<QWheelEvent*>(event));
            break;
        case QEvent::KeyPress:
            OnKeyPressEvent(static_cast<QKeyEvent*>(event));
            break;
        case QEvent::KeyRelease:
            OnKeyReleaseEvent(static_cast<QKeyEvent*>(event));
            break;
        case QEvent::Leave:
            OnLeaveWindowEvent();
            break;
    }
    return false;
}

static const float zoomRatioWheel = 1.3f;
static const float zoomRatioButtons = 1.4f;

void MainCanvas::OnWheelEvent(QWheelEvent *event) {
	float ratio = zoomRatioWheel;
	if(event->pixelDelta().y() < 0)
		ratio = 1.0f / ratio;

	board->Zoom(ratio, event->position());

    repaint();
}

void MainCanvas::OnLeftDownEvent(QMouseEvent *event) {
    Vec2 mouse(board->ConvertToCoords(event->pos()));
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(zoomRatioButtons, event->pos());
	else if(settings.selectedTool == TOOL_RECTANGLE || settings.selectedTool == TOOL_CIRCLE)
		lastPlacedPoint = board->GetFirstPlaced()->GetPosition();
	else if(settings.selectedTool == TOOL_TRACK || settings.selectedTool == TOOL_ZONE) {
		const Array<Vec2> &points = ((PolygonBase*) board->GetFirstPlaced())->points;
		if(settings.selectedTool == TOOL_ZONE && placedPointCount > 1 && (points.First() - points.Last()).LengthSq() < utils::Sq(settings.trackSize / 2.0f)) {
			placedPointCount = points.Size() - 1;
			FinishCreating();
		} else {
			lastPlacedPoint = points.Last();
			placedPointCount = points.Size();
		}
	} else {
		if(board->GetFirstPlaced()) {
			if(!board->GetFirstPlaced()->groups.Empty())
				settings.selectedTool = TOOL_EDIT;
			board->UnselectAll();
		} else if(settings.selectedTool == TOOL_EDIT) {
			Object *object = board->TestPoint(mouse);
			if(!object || !object->IsSelected() || shiftFlag) {
				if(!shiftFlag)
					board->UnselectAll();
				if(object)
					board->InvertSelectionGroup(object);
			}
			if(object) {
				mousePosition = lastPlacedPoint = object->GetNearestPoint(mouse);
				mouseDelta = mouse - mousePosition;
			}
		}
	}
    repaint();
}

void MainCanvas::OnMiddleDownEvent(QMouseEvent *event) {
	dragPosition = board->ConvertToCoords(event->pos());
}

void MainCanvas::OnRightDownEvent(QMouseEvent *event) {
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(1.0f / zoomRatioButtons, event->pos());
	else if(settings.selectedTool != TOOL_EDIT) {
		if(placedPointCount == 0)
            emit ToolChanged(TOOL_EDIT);
		FinishCreating();
    }
    repaint();
}

void MainCanvas::OnLeftUpEvent(QMouseEvent *event) {
	if(settings.selectedTool == TOOL_RECTANGLE || settings.selectedTool == TOOL_CIRCLE) {
		if(mousePosition == lastPlacedPoint)
			board->CancelPlacing();
		board->UnselectAll();
	} else if(settings.selectedTool == TOOL_EDIT) {
		lastPlacedPoint = Vec2::Invalid();
        repaint();
	}
}

void MainCanvas::OnMouseMotionEvent(QMouseEvent *event) {
    Vec2 mouse(board->ConvertToCoords(event->pos()));
	if(settings.selectedTool == TOOL_EDIT && (event->buttons() & Qt::LeftButton)) {
		if(board->IsSelected()) {
			Vec2 _mouse = board->ToActiveGrid(mouse - mouseDelta, lastPlacedPoint);
			Vec2 delta = _mouse - mousePosition;
			mousePosition = _mouse;
			board->MoveSelected(delta);
		}
	} else if(board->GetFirstPlaced()) {
		if(event->buttons() & Qt::LeftButton) {
			mousePosition = board->ToActiveGrid(mouse);
			if(settings.selectedTool == TOOL_RECTANGLE)
				BuildRect();
			else if(settings.selectedTool == TOOL_CIRCLE)
				BuildCircle();
		} else if(placedPointCount) {
			mousePosition = board->ToActiveGrid(mouse);
			BuildTrackEnd();
		} else {
			Vec2 _mouse = board->ToActiveGrid(mouse, lastPlacedPoint);
			Vec2 delta = _mouse - mousePosition;
			mousePosition = _mouse;
			board->MovePlaced(delta);
		}
	} else if(settings.selectedTool >= TOOL_TRACK && settings.selectedTool <= TOOL_ZONE) {
		mousePosition = board->ToActiveGrid(mouse);
		Object *creating = nullptr;
		if(settings.selectedTool == TOOL_THT_PAD)
			creating = new THTPad(board->GetSelectedLayer(), settings.groundDistance, mousePosition, settings.padSize, settings.padShape, settings.metallization);
		else if(settings.selectedTool == TOOL_SMD_PAD)
			creating = new SMDPad(board->GetSelectedLayer(), settings.groundDistance, mousePosition, settings.smdSize);
		else if(settings.selectedTool == TOOL_TRACK || (settings.selectedTool == TOOL_RECTANGLE && !settings.rectFill))
			creating = new Track(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1);
		else if(settings.selectedTool == TOOL_ZONE)
			creating = new Poly(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1, false);
		else if(settings.selectedTool == TOOL_RECTANGLE && settings.rectFill)
			creating = new Poly(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, &mousePosition, 1, true);
		else if(settings.selectedTool == TOOL_CIRCLE)
			creating = new Circle(board->GetSelectedLayer(), settings.groundDistance, settings.trackSize, mousePosition, 0.0f, 0.0f, 0.0f);
		if(creating)
			PlaceObject(creating);
	} else
		mousePosition = mouse;
	if(event->buttons() & Qt::MiddleButton) {
		Vec2 delta = mouse - dragPosition;
		board->UpdateCamera(delta);
	}
    dragPosition = board->ConvertToCoords(event->pos());
    repaint();
}

void MainCanvas::OnKeyPressEvent(QKeyEvent *event) {
    UpdateBoardGrid(event);
	if(!event->modifiers()) {
        int key = event->key();
		if(key >= Qt::Key_0 && key <= Qt::Key_9)
			board->SetGrid(settings.gridBind[key - Qt::Key_0]);
		if(key == Qt::Key_Space && placedPointCount) {
            PolygonBase::ChangeBendMode();
			BuildTrackEnd();
		}
	}
    repaint();
}

void MainCanvas::OnKeyReleaseEvent(QKeyEvent *event) {
    UpdateBoardGrid(event);
    repaint();
}

void MainCanvas::OnLeaveWindowEvent() {
	if(board->GetFirstPlaced() && board->GetFirstPlaced()->groups.Empty() && !placedPointCount) {
		board->CancelPlacing();
		lastPlacedPoint = Vec2::Invalid();
        repaint();
	}
}

void MainCanvas::UpdateBoardGrid(QKeyEvent *event) {
	shiftFlag = event->modifiers() & Qt::ShiftModifier;
	ctrlFlag  = event->modifiers() & Qt::ControlModifier;
	board->UpdateGrid(shiftFlag, ctrlFlag);
}

void MainCanvas::BuildTrackEnd() {
    PolygonBase *poly = static_cast<PolygonBase*>(board->GetFirstPlaced());
    poly->BuildTrackEnd(mousePosition, placedPointCount);
}

void MainCanvas::BuildRect() {
    PolygonBase *poly = static_cast<PolygonBase*>(board->GetFirstPlaced());
    poly->BuildRect(mousePosition);
}

void MainCanvas::BuildCircle() {
    Circle *circle = static_cast<Circle*>(board->GetFirstPlaced());
    circle->BuildCircle(mousePosition);
}

void MainCanvas::PlaceObject(Object *object) {
	mousePosition = lastPlacedPoint = board->ToGrid(mousePosition);
	board->PlaceObject(object);
}

void MainCanvas::PlaceObjectGroup(const ObjectGroup &objects) {
	lastPlacedPoint = board->ToActiveGrid(mousePosition);
	board->PlaceGroup(objects, mousePosition);
    repaint();
}

