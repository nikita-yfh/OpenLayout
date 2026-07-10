#include "MainCanvas.h"

#include "GLUtils.h"
#include "THTPad.h"
#include "SMDPad.h"
#include "Track.h"
#include "Poly.h"
#include "Circle.h"
#include "Utils.h"

MainCanvas::MainCanvas(Board *_board, Settings &_settings, QWidget *parent)
                : board(_board), settings(_settings), QOpenGLWidget(parent),
                  placedPointCount(0), lastPlacedPoint(Vec2::Invalid()), mousePosition(Vec2::Invalid()),
                  firstConnectionPad(nullptr), secondConnectionPad(nullptr), firstConnectionPointSelected(false) {
    installEventFilter(this);
    setMouseTracking(true);
    // Receive key events via focus instead of a global grabKeyboard(): the
    // grab routes events to this widget even while a modal dialog is open,
    // which floods GTK with WIDGET_REALIZED_FOR_EVENT warnings.
    setFocusPolicy(Qt::StrongFocus);
}

void MainCanvas::initializeGL() {
}

void MainCanvas::paintGL() {
    board->Draw(settings, currentSize);
    if(selecting)
        DrawSelectionRect();
    if(settings.selectedTool == TOOL_MEASURE && measuring)
        DrawMeasure();
}

void MainCanvas::DrawMeasure() const {
    // Board::Draw leaves the camera/zoom projection set, so we draw in mm.
    glColor3f(0.0f, 0.45f, 0.85f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glutils::Vertex(measureStart);
    glutils::Vertex(measureEnd);
    glEnd();
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glutils::Vertex(measureStart);
    glutils::Vertex(measureEnd);
    glEnd();
}

void MainCanvas::EmitMeasure() {
    Vec2 d = measureEnd - measureStart;
    // Board Y grows downward; negate it so the reported angle is mathematical.
    float angle = atan2f(-d.y, d.x) * 180.0f / M_PI;
    emit Measured(QString("%1: %2 mm    dx: %3    dy: %4    %5: %6°")
        .arg(QString::fromUtf8("Δ"))
        .arg(d.Length(), 0, 'f', 3)
        .arg(d.x, 0, 'f', 3)
        .arg(-d.y, 0, 'f', 3)
        .arg(QString::fromUtf8("∠"))
        .arg(angle, 0, 'f', 1));
}

void MainCanvas::DrawSelectionRect() const {
    // Drawn in board coordinates: Board::Draw leaves the projection set up
    // with the camera/zoom transform.
    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glutils::Vertex(Vec2(selectStart.x, selectStart.y));
    glutils::Vertex(Vec2(selectEnd.x,   selectStart.y));
    glutils::Vertex(Vec2(selectEnd.x,   selectEnd.y));
    glutils::Vertex(Vec2(selectStart.x, selectEnd.y));
    glEnd();
}

void MainCanvas::resizeGL(int w, int h) {
    currentSize.x = w;
    currentSize.y = h;
}

void MainCanvas::FinishCreating() {
	Object *placed = board->GetFirstPlaced();
	if(placed && (placedPointCount >= 3 || (placedPointCount == 2 && settings.selectedTool == TOOL_TRACK))) {
		((PolygonBase*) placed)->points.Resize(placedPointCount);
		board->UnselectAll();
	} else
		board->CancelPlacing();
	placedPointCount = 0;
	lastPlacedPoint = Vec2::Invalid();
    update();
}

bool MainCanvas::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonDblClick: {
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
	if(event->angleDelta().y() < 0)
		ratio = 1.0f / ratio;

	board->Zoom(ratio, event->position());

    update();
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
	} else if(settings.selectedTool == TOOL_CONNECTIONS) {
        if(firstConnectionPointSelected && secondConnectionPad) {
            firstConnectionPad->AddConnection(secondConnectionPad);
            secondConnectionPad->AddConnection(firstConnectionPad);
            firstConnectionPad = secondConnectionPad = nullptr;
            firstConnectionPointSelected = false;
            board->UnselectAll();
        } else {
            if(firstConnectionPad)
                firstConnectionPointSelected = true;
        }
    } else if(settings.selectedTool == TOOL_MEASURE) {
        measuring = true;
        measureStart = measureEnd = board->ToActiveGrid(mouse);
        EmitMeasure();
    } else if(settings.selectedTool == TOOL_SOLDER_MASK) {
        Object *object = board->TestPoint(mouse);
        if(object) {
            emit BeforeChange();        // snapshot for undo
            board->ToggleSoldermask(object);
        }
    } else if(settings.selectedTool == TOOL_TEST) {
        board->SelectConnected(board->TestPoint(mouse));
    } else {
		if(board->GetFirstPlaced()) {
			if(!board->GetFirstPlaced()->groups.Empty())
				settings.selectedTool = TOOL_EDIT;
			board->UnselectAll();
		} else if(settings.selectedTool == TOOL_EDIT) {
			Object *object = board->TestPoint(mouse);
            bool shift = event->modifiers() & Qt::ShiftModifier;
			if(!object || !object->IsSelected() || shift) {
				if(!shift)
					board->UnselectAll();
				if(object)
					board->InvertSelectionGroup(object);
			}
			if(object) {
				mousePosition = lastPlacedPoint = object->GetNearestPoint(mouse);
				mouseDelta = mouse - mousePosition;
				dragStarted = true;       // arm undo snapshot for a potential move
			} else {
				// Empty space: begin a rubber-band selection.
				selecting = true;
				selectStart = selectEnd = mouse;
			}
		}
	}
    update();
}

void MainCanvas::OnMiddleDownEvent(QMouseEvent *event) {
	dragPosition = board->ConvertToCoords(event->pos());
}

void MainCanvas::OnRightDownEvent(QMouseEvent *event) {
	if(settings.selectedTool == TOOL_ZOOM || settings.selectedTool == TOOL_PHOTOVIEW)
		board->Zoom(1.0f / zoomRatioButtons, event->pos());
	else if(settings.selectedTool == TOOL_MEASURE) {
		measuring = false;
		emit Measured(QString());
	}
	else if(settings.selectedTool == TOOL_TEST)
		board->UnselectAll();
	else if(settings.selectedTool != TOOL_EDIT) {
		if(placedPointCount == 0)
            emit ToolChanged(TOOL_EDIT);
		FinishCreating();
    }
    update();
}

void MainCanvas::OnLeftUpEvent(QMouseEvent *event) {
	if(settings.selectedTool == TOOL_RECTANGLE || settings.selectedTool == TOOL_CIRCLE) {
		if(mousePosition == lastPlacedPoint)
			board->CancelPlacing();
		board->UnselectAll();
	} else if(settings.selectedTool == TOOL_EDIT) {
		dragStarted = false;
		if(selecting) {
			selecting = false;
			board->SelectInRect(AABB(Vec2::Min(selectStart, selectEnd),
			                         Vec2::Max(selectStart, selectEnd)));
		}
		lastPlacedPoint = Vec2::Invalid();
        update();
	}
}

void MainCanvas::OnMouseMotionEvent(QMouseEvent *event) {
    Vec2 mouse(board->ConvertToCoords(event->pos()));
	if(settings.selectedTool == TOOL_MEASURE && (event->buttons() & Qt::LeftButton)) {
		measureEnd = board->ToActiveGrid(mouse);
		EmitMeasure();
	}
	if(settings.selectedTool == TOOL_EDIT && (event->buttons() & Qt::LeftButton)) {
		if(selecting) {
			selectEnd = mouse;
		} else if(board->IsSelected()) {
			if(dragStarted) {          // snapshot once, before the drag moves anything
				emit BeforeChange();
				dragStarted = false;
			}
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
		if(creating) {
			emit BeforeChange();      // snapshot for undo before adding the object
			PlaceObject(creating);
		}
    } else
		mousePosition = mouse;

	if(settings.selectedTool == TOOL_CONNECTIONS) {
        if(firstConnectionPointSelected) {
            if(secondConnectionPad)
                secondConnectionPad->Unselect();
            secondConnectionPad = board->TestPointPad(mouse);
            if(secondConnectionPad)
                secondConnectionPad->Select();
        } else {
            if(firstConnectionPad)
                firstConnectionPad->Unselect();
            firstConnectionPad = board->TestPointPad(mouse);
            if(firstConnectionPad)
                firstConnectionPad->Select();
        }
    }

	if(event->buttons() & Qt::MiddleButton) {
		Vec2 delta = mouse - dragPosition;
		board->UpdateCamera(delta);
	}
    dragPosition = board->ConvertToCoords(event->pos());
    update();
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
		if(key == Qt::Key_Delete)
			board->DeleteSelected();
	}
    update();
}

void MainCanvas::OnKeyReleaseEvent(QKeyEvent *event) {
    UpdateBoardGrid(event);
    update();
}

void MainCanvas::OnLeaveWindowEvent() {
	if(board->GetFirstPlaced() && board->GetFirstPlaced()->groups.Empty() && !placedPointCount) {
		board->CancelPlacing();
		lastPlacedPoint = Vec2::Invalid();
        update();
	}
}

void MainCanvas::UpdateBoardGrid(QKeyEvent *event) {
	bool shiftFlag = event->modifiers() & Qt::ShiftModifier;
	bool ctrlFlag  = event->modifiers() & Qt::ControlModifier;
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
    update();
}

void MainCanvas::Copy() {
	ObjectGroup *copied = board->CopySelected();
	if(copied) {
		delete clipboard;
		clipboard = copied;
	}
}

void MainCanvas::Cut() {
	ObjectGroup *copied = board->CopySelected();
	if(copied) {
		delete clipboard;
		clipboard = copied;
		board->DeleteSelected();
		update();
	}
}

void MainCanvas::Paste() {
	if(clipboard) {
		board->UnselectAll();
		PlaceObjectGroup(*clipboard);
	}
}

void MainCanvas::Duplicate() {
	ObjectGroup *dup = board->CopySelected();
	if(dup) {
		board->UnselectAll();
		PlaceObjectGroup(*dup);
		delete dup;
	}
}

