#include "FormDialog.h"

#include "GLUtils.h"
#include "ColorScheme.h"
#include "Track.h"
#include "Poly.h"
#include "Circle.h"
#include "Locale.h"

#include <vector>
#include <cmath>
#include <cstdlib>

#include <QTabWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDialogButtonBox>

// ---------------------------------------------------------------------------
// FormPreview
// ---------------------------------------------------------------------------

FormPreview::FormPreview(const Settings &_settings, uint8_t _layer, QWidget *parent)
    : QOpenGLWidget(parent), settings(_settings), layer(_layer) {
    setMinimumSize(280, 280);
}

void FormPreview::resizeGL(int w, int h) {
    currentSize.x = w;
    currentSize.y = h;
}

void FormPreview::paintGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, currentSize.x, currentSize.y);

    if(!group || group->IsEmpty())
        return;

    // Fit the group's bounding box into the viewport, keeping the aspect ratio
    // and leaving a small margin.
    AABB box = group->GetObjectsAABB();
    if(!box.IsValid())
        return;
    Vec2 size = box.Size();
    if(size.x < 0.001f) size.x = 0.001f;
    if(size.y < 0.001f) size.y = 0.001f;
    Vec2 center = box.GetCenter();

    float margin = 1.1f;
    float viewW = size.x * margin, viewH = size.y * margin;
    float screenAspect = currentSize.x / currentSize.y;
    if(screenAspect > viewW / viewH)
        viewW = viewH * screenAspect;
    else
        viewH = viewW / screenAspect;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Y grows downward, matching Board::Draw.
    glOrtho(center.x - viewW / 2.0f, center.x + viewW / 2.0f,
            center.y + viewH / 2.0f, center.y - viewH / 2.0f, 0.0f, 1.0f);

    group->DrawObjects(settings.GetColorScheme(), layer, false);
}

// ---------------------------------------------------------------------------
// FormDialog
// ---------------------------------------------------------------------------

static QDoubleSpinBox *MakeDouble(double min, double max, double value, double step) {
    QDoubleSpinBox *spin = new QDoubleSpinBox();
    spin->setRange(min, max);
    spin->setSingleStep(step);
    spin->setValue(value);
    return spin;
}

static QSpinBox *MakeInt(int min, int max, int value) {
    QSpinBox *spin = new QSpinBox();
    spin->setRange(min, max);
    spin->setValue(value);
    return spin;
}

FormDialog::FormDialog(const Settings &_settings, const Vec2 &_boardSize,
                       uint8_t _layer, QWidget *parent)
    : QDialog(parent), settings(_settings), boardSize(_boardSize), layer(_layer) {
    setWindowTitle(_("Special form"));

    tabs = new QTabWidget(this);
    tabs->addTab(CreatePolygonTab(), _("Polygon"));
    tabs->addTab(CreateSpiralTab(),  _("Spiral"));
    tabs->addTab(CreateFrameTab(),   _("Frame"));

    preview = new FormPreview(settings, layer, this);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *side = new QVBoxLayout();
    side->addWidget(tabs);
    side->addWidget(buttons);

    QHBoxLayout *content = new QHBoxLayout(this);
    content->addLayout(side, 2);
    content->addWidget(preview, 3);

    // Any change to any control re-generates the preview.
    auto rerun = [this]{ Regenerate(); };
    connect(tabs, &QTabWidget::currentChanged, this, rerun);
    for(QDoubleSpinBox *s : {polyRadius, polyWidth, polyOffset, spiralRadius,
                             spiralDistance, spiralWidth, spiralTurns,
                             frameWidth, frameHeight})
        connect(s, qOverload<double>(&QDoubleSpinBox::valueChanged), this, rerun);
    for(QSpinBox *s : {polyCorners, frameColumns, frameRows})
        connect(s, qOverload<int>(&QSpinBox::valueChanged), this, rerun);
    for(QCheckBox *c : {polyRays, polyFill})
        connect(c, &QCheckBox::toggled, this, rerun);
    connect(spiralRound,  &QRadioButton::toggled, this, rerun);
    connect(spiralSquare, &QRadioButton::toggled, this, rerun);
    for(QComboBox *c : {frameColumnsDef, frameRowsDef, frameColumnsSide, frameRowsSide})
        connect(c, qOverload<int>(&QComboBox::currentIndexChanged), this, rerun);

    Regenerate();
}

QWidget *FormDialog::CreatePolygonTab() {
    QWidget *panel = new QWidget();
    QFormLayout *form = new QFormLayout(panel);

    polyRadius  = MakeDouble(1.0,  249.9, 12.0, 0.5);
    polyWidth   = MakeDouble(0.0,  99.99, 0.4,  0.05);
    polyCorners = MakeInt(3, 99, 6);
    polyOffset  = MakeDouble(-180.0, 180.0, 0.0, 0.5);
    polyRays    = new QCheckBox(_("Rays"));
    polyFill    = new QCheckBox(_("Filled"));

    form->addRow(_("Radius (mm):"),         polyRadius);
    form->addRow(_("Line width (mm):"),     polyWidth);
    form->addRow(_("Number of corners:"),   polyCorners);
    form->addRow(_("Angle offset (°):"), polyOffset);
    form->addRow(QString(), polyRays);
    form->addRow(QString(), polyFill);
    return panel;
}

QWidget *FormDialog::CreateSpiralTab() {
    QWidget *panel = new QWidget();
    QFormLayout *form = new QFormLayout(panel);

    spiralRadius   = MakeDouble(0.1, 249.9, 2.0, 0.5);
    spiralDistance = MakeDouble(0.1, 99.9,  2.0, 0.1);
    spiralWidth    = MakeDouble(0.0, 99.99, 0.4, 0.05);
    spiralTurns    = MakeDouble(1.0, 100.0, 6.0, 0.25);
    spiralRound    = new QRadioButton(_("Round"));
    spiralSquare   = new QRadioButton(_("Square"));
    spiralRound->setChecked(true);
    spiralDiameter = new QLabel();

    form->addRow(_("Start radius (mm):"), spiralRadius);
    form->addRow(_("Distance (mm):"),     spiralDistance);
    form->addRow(_("Track width (mm):"),  spiralWidth);
    form->addRow(_("Turns:"),             spiralTurns);
    QHBoxLayout *shape = new QHBoxLayout();
    shape->addWidget(spiralRound);
    shape->addWidget(spiralSquare);
    form->addRow(_("Shape:"), shape);
    form->addRow(_("End diameter (mm):"), spiralDiameter);
    return panel;
}

QWidget *FormDialog::CreateFrameTab() {
    QWidget *panel = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(panel);

    const QStringList defs   = { "A, B, ...", "1, 2, ..." };
    const QStringList colSides = { "...", _("Top"),  _("Bottom"), _("Top + Bottom") };
    const QStringList rowSides = { "...", _("Left"), _("Right"),  _("Left + Right") };

    frameColumns     = MakeInt(1, 99, 8);
    frameRows        = MakeInt(1, 99, 8);
    frameColumnsDef  = new QComboBox(); frameColumnsDef->addItems(defs);  frameColumnsDef->setCurrentIndex(0);
    frameRowsDef     = new QComboBox(); frameRowsDef->addItems(defs);     frameRowsDef->setCurrentIndex(1);
    frameColumnsSide = new QComboBox(); frameColumnsSide->addItems(colSides); frameColumnsSide->setCurrentIndex(SIDE_A);
    frameRowsSide    = new QComboBox(); frameRowsSide->addItems(rowSides);    frameRowsSide->setCurrentIndex(SIDE_A);
    frameWidth       = MakeDouble(10.0, 999.0, 90.0, 0.5);
    frameHeight      = MakeDouble(10.0, 999.0, 40.0, 0.5);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel(_("Columns:")), 0, 0);
    grid->addWidget(frameColumns,     0, 1);
    grid->addWidget(frameColumnsDef,  0, 2);
    grid->addWidget(frameColumnsSide, 0, 3);
    grid->addWidget(new QLabel(_("Rows:")), 1, 0);
    grid->addWidget(frameRows,     1, 1);
    grid->addWidget(frameRowsDef,  1, 2);
    grid->addWidget(frameRowsSide, 1, 3);
    vbox->addLayout(grid);

    QFormLayout *size = new QFormLayout();
    size->addRow(_("Width (mm):"),  frameWidth);
    size->addRow(_("Height (mm):"), frameHeight);
    vbox->addLayout(size);

    QPushButton *autosize = new QPushButton(_("Autosize"));
    connect(autosize, &QPushButton::clicked, this, &FormDialog::AutosizeFrame);
    vbox->addWidget(autosize);
    vbox->addStretch();
    return panel;
}

void FormDialog::AutosizeFrame() {
    frameWidth->setValue(boardSize.x - 5.1f);
    frameHeight->setValue(boardSize.y - 5.1f);
}

void FormDialog::Regenerate() {
    delete group;
    group = new ObjectGroup();

    const float gd = settings.groundDistance;
    int type = tabs->currentIndex();

    if(type == TYPE_POLYGON) {
        float radius  = polyRadius->value();
        float width   = polyWidth->value();
        int corners   = polyCorners->value();
        float offset  = glutils::DegToRad(polyOffset->value());

        std::vector<Vec2> points(corners + 1);
        for(int corner = 0; corner < corners; corner++) {
            float angle = offset + corner * M_PI * 2.0f / corners;
            points[corner] = Vec2(angle) * radius;
        }

        if(polyFill->isChecked()) {                 // Filled polygon
            group->AddObjectEnd(new Poly(layer, gd, width, points.data(), corners, false));
        } else {
            points[corners] = points[0];            // Close the loop
            group->AddObjectBegin(new Track(layer, gd, width, points.data(), corners + 1));
            if(polyRays->isChecked()) {             // Spokes from the centre
                for(int corner = 0; corner < corners; corner++) {
                    Vec2 line[2] = { Vec2(0.0f, 0.0f), points[corner] };
                    group->AddObjectBegin(new Track(layer, gd, width, line, 2));
                }
            }
        }
    } else if(type == TYPE_SPIRAL) {
        float startRadius = spiralRadius->value();
        float distance    = spiralDistance->value();
        float width       = spiralWidth->value();
        int qturns        = (int)(spiralTurns->value() * 4);

        float endDiameter;
        if(spiralSquare->isChecked()) {             // Square spiral
            std::vector<Vec2> points(qturns + 1);
            points[0].SetZero();
            float delta = (distance + width) / 2.0f;
            if(startRadius <= delta / 2.0f)
                startRadius = delta / 2.0f + 0.1f;
            float length = startRadius * 2.0f - delta;
            for(int qturn = 0; qturn < qturns; qturn++) {
                points[qturn + 1] = points[qturn];
                if(qturn == qturns - 1)             // Last (shortened) segment
                    length -= delta * 2.0f;
                switch(qturn % 4) {                 // Direction
                case 0: points[qturn + 1].x -= length; break;
                case 1: points[qturn + 1].y -= length; break;
                case 2: points[qturn + 1].x += length; break;
                case 3: points[qturn + 1].y += length; break;
                }
                length += delta;
            }
            group->AddObjectBegin(new Track(layer, gd, width, points.data(), qturns + 1));
            endDiameter = length + width;
        } else {                                    // Round spiral
            float delta = (distance + width) / 4.0f;
            if(startRadius <= delta * 2.0f)
                startRadius = delta * 2.0f + 0.1f;
            float radius = startRadius / 2.0f - delta;
            Vec2 centers[4] = {
                Vec2(-delta / 2.0f, -delta / 2.0f),
                Vec2(-delta / 2.0f,  delta / 2.0f),
                Vec2( delta / 2.0f,  delta / 2.0f),
                Vec2( delta / 2.0f, -delta / 2.0f)
            };
            for(int qturn = 0; qturn < qturns; qturn++) {
                float beginAngle = glutils::AngleMod(M_PI / 2.0f * qturn);
                float endAngle   = glutils::AngleMod(M_PI / 2.0f * (qturn + 1));
                group->AddObjectBegin(new Circle(layer, gd, width,
                    centers[qturn % 4], radius * 2.0f, beginAngle, endAngle));
                radius += delta;
            }
            endDiameter = 2.0f * radius - 3.0f * delta + width;
        }
        spiralRadius->blockSignals(true);
        spiralRadius->setValue(startRadius);
        spiralRadius->blockSignals(false);
        spiralDiameter->setText(QString::number(endDiameter, 'f', 3));
    } else {                                        // TYPE_FRAME
        int cols = std::abs(frameColumns->value());
        int rows = std::abs(frameRows->value());
        if(cols < 1) cols = 1;
        if(rows < 1) rows = 1;
        float W = frameWidth->value();
        float H = frameHeight->value();
        float lw = settings.trackSize;

        float x0 = -W / 2.0f, y0 = -H / 2.0f;
        float x1 =  W / 2.0f, y1 =  H / 2.0f;

        // Outer border.
        Vec2 rect[5] = { Vec2(x0, y0), Vec2(x1, y0), Vec2(x1, y1), Vec2(x0, y1), Vec2(x0, y0) };
        group->AddObjectBegin(new Track(layer, gd, lw, rect, 5));

        // Internal grid dividers.
        for(int c = 1; c < cols; c++) {
            float x = x0 + W * c / cols;
            Vec2 line[2] = { Vec2(x, y0), Vec2(x, y1) };
            group->AddObjectBegin(new Track(layer, gd, lw, line, 2));
        }
        for(int r = 1; r < rows; r++) {
            float y = y0 + H * r / rows;
            Vec2 line[2] = { Vec2(x0, y), Vec2(x1, y) };
            group->AddObjectBegin(new Track(layer, gd, lw, line, 2));
        }

        // Tick marks marking each cell on the requested side(s). The letter /
        // number labels themselves wait for a Text object.
        const float tick = 2.0f;
        int colSide = frameColumnsSide->currentIndex();
        int rowSide = frameRowsSide->currentIndex();
        for(int c = 1; c < cols; c++) {
            float x = x0 + W * c / cols;
            if(colSide == SIDE_A || colSide == SIDE_BOTH) {
                Vec2 t[2] = { Vec2(x, y0), Vec2(x, y0 - tick) };
                group->AddObjectBegin(new Track(layer, gd, lw, t, 2));
            }
            if(colSide == SIDE_B || colSide == SIDE_BOTH) {
                Vec2 t[2] = { Vec2(x, y1), Vec2(x, y1 + tick) };
                group->AddObjectBegin(new Track(layer, gd, lw, t, 2));
            }
        }
        for(int r = 1; r < rows; r++) {
            float y = y0 + H * r / rows;
            if(rowSide == SIDE_A || rowSide == SIDE_BOTH) {
                Vec2 t[2] = { Vec2(x0, y), Vec2(x0 - tick, y) };
                group->AddObjectBegin(new Track(layer, gd, lw, t, 2));
            }
            if(rowSide == SIDE_B || rowSide == SIDE_BOTH) {
                Vec2 t[2] = { Vec2(x1, y), Vec2(x1 + tick, y) };
                group->AddObjectBegin(new Track(layer, gd, lw, t, 2));
            }
        }
    }

    preview->SetGroup(group);
}
