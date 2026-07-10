#include "MainWindow.h"
#include "Locale.h"

#include "SettingsDialog.h"
#include "FormDialog.h"
#include "Text.h"
#include "Gerber.h"
#include "Track.h"
#include "Poly.h"
#include "THTPad.h"
#include "SMDPad.h"

#include <QIcon>
#include <QAction>
#include <QVBoxLayout>
#include <QTimer>
#include <QInputDialog>

#include <QPrinter>
#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPainter>

#include <fstream>
#include <iomanip>
#include <iterator>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <utility>

#include "xpm/toolbar/align_bottom.xpm"
#include "xpm/toolbar/align_hcenter.xpm"
#include "xpm/toolbar/align_left.xpm"
#include "xpm/toolbar/align_right.xpm"
#include "xpm/toolbar/align_top.xpm"
#include "xpm/toolbar/align_vcenter.xpm"
#include "xpm/toolbar/align.xpm"
#include "xpm/toolbar/bitmap.xpm"
#include "xpm/toolbar/components.xpm"
#include "xpm/toolbar/copy.xpm"
#include "xpm/toolbar/cut.xpm"
#include "xpm/toolbar/delete.xpm"
#include "xpm/toolbar/drc.xpm"
#include "xpm/toolbar/duplicate.xpm"
#include "xpm/toolbar/group_off.xpm"
#include "xpm/toolbar/group_on.xpm"
#include "xpm/toolbar/info.xpm"
#include "xpm/toolbar/macro.xpm"
#include "xpm/toolbar/mirror_h.xpm"
#include "xpm/toolbar/mirror_v.xpm"
#include "xpm/toolbar/new.xpm"
#include "xpm/toolbar/open.xpm"
#include "xpm/toolbar/paste.xpm"
#include "xpm/toolbar/print.xpm"
#include "xpm/toolbar/properties.xpm"
#include "xpm/toolbar/redo.xpm"
#include "xpm/toolbar/remove_con.xpm"
#include "xpm/toolbar/rotate.xpm"
#include "xpm/toolbar/save.xpm"
#include "xpm/toolbar/selector.xpm"
#include "xpm/toolbar/to_grid.xpm"
#include "xpm/toolbar/transparent.xpm"
#include "xpm/toolbar/undo.xpm"
#include "xpm/toolbar/zoom_any.xpm"
#include "xpm/toolbar/zoom_board.xpm"
#include "xpm/toolbar/zoom_objects.xpm"
#include "xpm/toolbar/zoom_selection.xpm"
#include "xpm/toolbar/zoom_prev.xpm"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    File file("/storage/diy/pcb/1.lay6", "rb");
    if(!file.IsOk() || !pcb.Load(file))
        pcb.AddBoard(new Board(_("Board"), Board::Type::Rectangle,
                               Vec2(100.0f, 80.0f), 5.0f, false));

    CreateActions();
    CreateToolBar();
    CreateLeftPanel();
    CreateMenuBar();

    mainCanvas = new MainCanvas(pcb.GetSelectedBoard(), settings, this);

    // Central area: a tab bar (one tab per board) above the canvas.
    QWidget *central = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(central);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    boardTabs = new QTabBar(central);
    boardTabs->setExpanding(false);
    centralLayout->addWidget(boardTabs);
    centralLayout->addWidget(mainCanvas);
    setCentralWidget(central);

    connect(boardTabs, &QTabBar::currentChanged, this, [this](int index){
        if(updatingTabs || index < 0)
            return;
        pcb.SetTab(index);
        mainCanvas->SetBoard(pcb.GetSelectedBoard());
        SyncLayers();
    });
    RebuildBoardTabs();

    connect(toolPanel, SIGNAL(ToolChanged()), mainCanvas, SLOT(FinishCreating()));
    connect(mainCanvas, SIGNAL(ToolChanged(int)), toolPanel, SLOT(OnToolChanged(int)));
    connect(mainCanvas, &MainCanvas::BeforeChange, this, &MainWindow::PushUndo);

    statusBar()->showMessage(_("Ready"));
    connect(mainCanvas, &MainCanvas::Measured, this, [this](const QString &text){
        if(text.isEmpty())
            statusBar()->clearMessage();
        else
            statusBar()->showMessage(text);
    });

    // The Special-form tool opens a generator dialog; on OK its objects become a
    // group that follows the cursor until clicked (a left click on the canvas
    // drops it and reverts to Edit). Cancel just returns to Edit.
    connect(toolPanel, &ToolPanel::ToolChanged, this, [this]{
        if(settings.selectedTool != TOOL_FORM)
            return;
        Board *b = pcb.GetSelectedBoard();
        FormDialog dlg(settings, b->GetSize(), b->GetSelectedLayer(), this);
        if(dlg.exec() == QDialog::Accepted)
            mainCanvas->PlaceObjectGroup(dlg.GetObjects());
        else
            toolPanel->OnToolChanged(TOOL_EDIT);
    });

    // The Text tool asks for the string and its size, then places a single Text
    // object that follows the cursor until clicked (same flow as Special form).
    connect(toolPanel, &ToolPanel::ToolChanged, this, [this]{
        if(settings.selectedTool != TOOL_TEXT)
            return;
        QDialog dlg(this);
        dlg.setWindowTitle(_("Text"));
        QLineEdit *edit = new QLineEdit(&dlg);
        QDoubleSpinBox *height = new QDoubleSpinBox(&dlg);
        height->setRange(0.1, 999.0); height->setSingleStep(0.5); height->setValue(2.0);
        QDoubleSpinBox *lineWidth = new QDoubleSpinBox(&dlg);
        lineWidth->setRange(0.01, 99.0); lineWidth->setSingleStep(0.05);
        lineWidth->setValue(settings.trackSize);
        QCheckBox *mirror = new QCheckBox(_("Mirror (other side)"), &dlg);
        QDialogButtonBox *bb =
            new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
        connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        QFormLayout *form = new QFormLayout(&dlg);
        form->addRow(_("Text:"),            edit);
        form->addRow(_("Height (mm):"),     height);
        form->addRow(_("Line width (mm):"), lineWidth);
        form->addRow(QString(), mirror);
        form->addRow(bb);

        if(dlg.exec() == QDialog::Accepted && !edit->text().isEmpty()) {
            Board *b = pcb.GetSelectedBoard();
            ObjectGroup group;
            group.AddObjectEnd(new Text(b->GetSelectedLayer(), settings.groundDistance,
                lineWidth->value(), Vec2(0.0f, 0.0f), height->value(),
                edit->text().toLocal8Bit().constData(), mirror->isChecked()));
            mainCanvas->PlaceObjectGroup(group);
        } else {
            toolPanel->OnToolChanged(TOOL_EDIT);
        }
    });

    // The Autoroute tool asks for track width / clearance / sides, runs the
    // maze router once over the board's rubber-band connections, then drops
    // back to Edit.
    connect(toolPanel, &ToolPanel::ToolChanged, this, [this]{
        if(settings.selectedTool != TOOL_AUTOROUTE)
            return;
        Board *b = pcb.GetSelectedBoard();
        QDialog dlg(this);
        dlg.setWindowTitle(_("Autoroute"));
        QDoubleSpinBox *tw = new QDoubleSpinBox(&dlg);
        tw->setRange(0.05, 10.0); tw->setSingleStep(0.05); tw->setValue(settings.trackSize);
        QDoubleSpinBox *cl = new QDoubleSpinBox(&dlg);
        cl->setRange(0.05, 10.0); cl->setSingleStep(0.05); cl->setValue(settings.groundDistance);
        QCheckBox *both = new QCheckBox(_("Use both sides (vias)"), &dlg);
        both->setChecked(b->IsMultilayer());
        QDialogButtonBox *bb =
            new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
        connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        QFormLayout *form = new QFormLayout(&dlg);
        form->addRow(_("Track width (mm):"), tw);
        form->addRow(_("Clearance (mm):"),   cl);
        form->addRow(QString(), both);
        form->addRow(bb);

        if(dlg.exec() == QDialog::Accepted) {
            Settings rs = settings;
            rs.trackSize = tw->value();
            rs.groundDistance = cl->value();
            PushUndo();
            std::pair<int, int> r = b->Autoroute(rs, both->isChecked());
            mainCanvas->update();
            statusBar()->showMessage(QString(_("Autorouted %1 of %2 connections"))
                .arg(r.first).arg(r.second));
        }
        toolPanel->OnToolChanged(TOOL_EDIT);
    });

    // Helper: run an operation on the active board, then refresh the canvas.
    auto edit = [this](auto op) {
        return [this, op]() { op(pcb.GetSelectedBoard()); mainCanvas->update(); };
    };
    // Like edit(), but snapshots the board for undo before mutating it.
    auto editUndo = [this](auto op) {
        return [this, op]() { PushUndo(); op(pcb.GetSelectedBoard()); mainCanvas->update(); };
    };
    // Helper: build a zoom handler that needs the current canvas size.
    auto zoom = [this](void (Board::*fn)(const Vec2&)) {
        return [this, fn]() {
            (pcb.GetSelectedBoard()->*fn)(Vec2(mainCanvas->width(), mainCanvas->height()));
            mainCanvas->update();
        };
    };

    // File
    connect(newAct,    &QAction::triggered, this, &MainWindow::NewFile);
    connect(openAct,   &QAction::triggered, this, &MainWindow::OpenFile);
    connect(saveAct,   &QAction::triggered, this, &MainWindow::SaveFile);
    connect(saveasAct,   &QAction::triggered, this, &MainWindow::SaveFileAs);
    connect(boardSaveAct, &QAction::triggered, this, &MainWindow::SaveFileAs);
    connect(exitAct,     &QAction::triggered, this, &MainWindow::close);

    // Edit
    connect(undoAct, &QAction::triggered, this, &MainWindow::Undo);
    connect(redoAct, &QAction::triggered, this, &MainWindow::Redo);

    connect(deleteAct,    &QAction::triggered, this, editUndo([](Board *b){ b->DeleteSelected(); }));
    connect(selectallAct, &QAction::triggered, this, edit([](Board *b){ b->SelectAll(); }));
    connect(copyAct,      &QAction::triggered, mainCanvas, &MainCanvas::Copy);
    connect(cutAct,       &QAction::triggered, this, [this](){ PushUndo(); mainCanvas->Cut(); });
    connect(pasteAct,     &QAction::triggered, this, [this](){ PushUndo(); mainCanvas->Paste(); });
    connect(duplicateAct, &QAction::triggered, this, [this](){ PushUndo(); mainCanvas->Duplicate(); });
    connect(groupAct,     &QAction::triggered, this, editUndo([](Board *b){ b->GroupSelected(); }));
    connect(ungroupAct,   &QAction::triggered, this, editUndo([](Board *b){ b->UngroupSelected(); }));
    connect(snapGridAct,  &QAction::triggered, this, editUndo([](Board *b){ b->SnapSelectedToGrid(); }));

    // Rotate / mirror
    connect(rotate90Act, &QAction::triggered, this, editUndo([](Board *b){ b->RotateSelected(M_PI / 2.0f); }));
    connect(rotate45Act, &QAction::triggered, this, editUndo([](Board *b){ b->RotateSelected(M_PI / 4.0f); }));
    connect(rotate15Act, &QAction::triggered, this, editUndo([](Board *b){ b->RotateSelected(M_PI / 12.0f); }));
    connect(rotate5Act,  &QAction::triggered, this, editUndo([](Board *b){ b->RotateSelected(M_PI / 36.0f); }));
    connect(hmirrorAct,  &QAction::triggered, this, editUndo([](Board *b){ b->MirrorSelectedHorizontal(); }));
    connect(vmirrorAct,  &QAction::triggered, this, editUndo([](Board *b){ b->MirrorSelectedVertical(); }));

    // Align (callback returns the move delta for each selected object)
    connect(alignLeftAct,   &QAction::triggered, this, editUndo([](Board *b){ b->AlignSelected(+[](const AABB &a, const AABB &o){ return Vec2(a.lower.x - o.lower.x, 0.0f); }); }));
    connect(alignRightAct,  &QAction::triggered, this, editUndo([](Board *b){ b->AlignSelected(+[](const AABB &a, const AABB &o){ return Vec2(a.upper.x - o.upper.x, 0.0f); }); }));
    connect(alignTopAct,    &QAction::triggered, this, editUndo([](Board *b){ b->AlignSelected(+[](const AABB &a, const AABB &o){ return Vec2(0.0f, a.lower.y - o.lower.y); }); }));
    connect(alignBottomAct, &QAction::triggered, this, editUndo([](Board *b){ b->AlignSelected(+[](const AABB &a, const AABB &o){ return Vec2(0.0f, a.upper.y - o.upper.y); }); }));
    connect(alignHAct,      &QAction::triggered, this, editUndo([](Board *b){ b->AlignSelected(+[](const AABB &a, const AABB &o){ return Vec2(a.GetCenter().x - o.GetCenter().x, 0.0f); }); }));
    connect(alignVAct,      &QAction::triggered, this, editUndo([](Board *b){ b->AlignSelected(+[](const AABB &a, const AABB &o){ return Vec2(0.0f, a.GetCenter().y - o.GetCenter().y); }); }));

    // "Functions > Set to layer" moves the selected objects to that layer.
    struct LayerEntry { QAction *act; uint8_t layer; };
    LayerEntry layerEntries[] = {
        {layerC1Act, ObjectGroup::LAYER_C1}, {layerS1Act, ObjectGroup::LAYER_S1},
        {layerC2Act, ObjectGroup::LAYER_C2}, {layerS2Act, ObjectGroup::LAYER_S2},
        {layerI1Act, ObjectGroup::LAYER_I1}, {layerI2Act, ObjectGroup::LAYER_I2},
        {layerOAct,  ObjectGroup::LAYER_O},
    };
    for(const LayerEntry &e : layerEntries) {
        uint8_t layer = e.layer;
        connect(e.act, &QAction::triggered, this, editUndo([layer](Board *b){ b->SetSelectedToLayer(layer); }));
    }

    // Layer toolbar: active-layer selector + per-layer visibility toggles.
    static const char *layerNames[7] = {"C1", "S1", "C2", "S2", "I1", "I2", "O"};
    QToolBar *toolBarLayers = addToolBar(_("Layers"));
    toolBarLayers->addWidget(new QLabel(_("Layer: ")));
    layerCombo = new QComboBox(toolBarLayers);
    for(int i = 0; i < 7; i++)
        layerCombo->addItem(layerNames[i]);
    toolBarLayers->addWidget(layerCombo);
    connect(layerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        if(updatingLayers || index < 0)
            return;
        pcb.GetSelectedBoard()->SetSelectedLayer(index);
        mainCanvas->update();
    });
    toolBarLayers->addSeparator();
    toolBarLayers->addWidget(new QLabel(_("Visible: ")));
    for(int i = 0; i < 7; i++) {
        layerVisAct[i] = toolBarLayers->addAction(layerNames[i]);
        layerVisAct[i]->setCheckable(true);
        connect(layerVisAct[i], &QAction::toggled, this, [this, i](bool on){
            if(updatingLayers)
                return;
            pcb.GetSelectedBoard()->SetLayerVisible(i, on);
            mainCanvas->update();
        });
    }
    SyncLayers();

    // Zoom
    connect(zoomBoardAct,     &QAction::triggered, this, zoom(&Board::ZoomBoard));
    connect(zoomObjectsAct,   &QAction::triggered, this, zoom(&Board::ZoomObjects));
    connect(zoomSelectionAct, &QAction::triggered, this, zoom(&Board::ZoomSelection));
    connect(zoomPreviousAct,  &QAction::triggered, this, [this](){
        pcb.GetSelectedBoard()->ZoomPrevious();
        mainCanvas->update();
    });

    // View
    transparentAct->setCheckable(true);
    connect(transparentAct, &QAction::toggled, this, [this](bool on){ settings.transparent = on; mainCanvas->update(); });

    // Board tabs
    connect(boardNewAct, &QAction::triggered, this, [this](){
        pcb.AddBoard(new Board(_("Board"), Board::Type::Rectangle, Vec2(100.0f, 80.0f), 5.0f, false));
        mainCanvas->SetBoard(pcb.GetSelectedBoard());
        RebuildBoardTabs();
    });
    connect(boardDeleteAct, &QAction::triggered, this, [this](){
        if(pcb.Size() > 1) {
            pcb.DeleteSelectedBoard();
            mainCanvas->SetBoard(pcb.GetSelectedBoard());
            RebuildBoardTabs();
        }
    });
    connect(boardCopyAct, &QAction::triggered, this, [this](){
        pcb.AddBoard(new Board(*pcb.GetSelectedBoard()));
        mainCanvas->SetBoard(pcb.GetSelectedBoard());
        RebuildBoardTabs();
    });
    connect(boardMoveLeftAct,  &QAction::triggered, this, [this](){
        if(pcb.CanMoveLeft())  { pcb.MoveSelectedBoardLeft();  mainCanvas->SetBoard(pcb.GetSelectedBoard()); RebuildBoardTabs(); }
    });
    connect(boardMoveRightAct, &QAction::triggered, this, [this](){
        if(pcb.CanMoveRight()) { pcb.MoveSelectedBoardRight(); mainCanvas->SetBoard(pcb.GetSelectedBoard()); RebuildBoardTabs(); }
    });
    connect(boardSetLeftAct,   &QAction::triggered, this, [this](){
        if(pcb.CanMoveLeft())  { pcb.SetSelectedBoardLeft();   mainCanvas->SetBoard(pcb.GetSelectedBoard()); RebuildBoardTabs(); }
    });
    connect(boardSetRightAct,  &QAction::triggered, this, [this](){
        if(pcb.CanMoveRight()) { pcb.SetSelectedBoardRight();  mainCanvas->SetBoard(pcb.GetSelectedBoard()); RebuildBoardTabs(); }
    });

    // Rotate by a custom angle (the fixed-angle rotations live above)
    connect(rotateAct, &QAction::triggered, this, [this](){
        bool ok = false;
        double deg = QInputDialog::getDouble(this, _("Rotate"), _("Angle (degrees):"),
                                             90.0, -360.0, 360.0, 1, &ok);
        if(ok) {
            PushUndo();
            pcb.GetSelectedBoard()->RotateSelected(deg * M_PI / 180.0f);
            mainCanvas->update();
        }
    });

    // Extras that map onto engine helpers
    connect(deleteOutsideAct, &QAction::triggered, this,
            editUndo([](Board *b){ b->DeleteOutside(AABB(Vec2(0.0f, 0.0f), b->GetSize())); }));
    connect(removeConAct, &QAction::triggered, this,
            editUndo([](Board *b){ b->RemoveAllConnections(); }));
    connect(changeSideAct, &QAction::triggered, this,
            editUndo([](Board *b){ b->ChangeSide(b->GetSize().x * 0.5f); }));

    // Drill data: Excellon export of the THT pad holes.
    connect(drillDataAct, &QAction::triggered, this, [this](){
        QString path = QFileDialog::getSaveFileName(this, _("Export drill data"),
                                                    QString(), "Excellon (*.drl);;All files (*)");
        if(path.isEmpty())
            return;
        Board *b = pcb.GetSelectedBoard();
        float height = b->GetSize().y;
        // Group holes by diameter in microns -> bottom-up coordinates (mm).
        std::map<int, std::vector<Vec2>> tools;
        for(Object *o = b->GetObjects(); o; o = o->GetNext()) {
            float d = o->GetDrillDiameter();
            if(d > 0.0f) {
                Vec2 p = o->GetPosition();
                tools[(int)lround(d * 1000.0f)].push_back(Vec2(p.x, height - p.y));
            }
        }
        std::ofstream out(path.toLocal8Bit().constData());
        if(!out) {
            QMessageBox::warning(this, _("Export drill data"), _("Could not write the file."));
            return;
        }
        out << "M48\n;DRILL file generated by OpenLayout\nMETRIC,TZ\n";
        out << std::fixed << std::setprecision(3);
        int t = 1;
        for(const auto &kv : tools)
            out << "T" << t++ << "C" << (kv.first / 1000.0) << "\n";
        out << "%\n";
        t = 1;
        for(const auto &kv : tools) {
            out << "T" << t++ << "\n";
            for(const Vec2 &p : kv.second)
                out << "X" << p.x << "Y" << p.y << "\n";
        }
        out << "T0\nM30\n";
    });

    // Gerber import: parse an RS-274X file onto the active layer.
    connect(gerberImportAct, &QAction::triggered, this, [this](){
        QString path = QFileDialog::getOpenFileName(this, _("Import Gerber"),
                                                    QString(), "Gerber (*.gbr);;All files (*)");
        if(!path.isEmpty())
            ImportGerberFile(path);
    });

    // Gerber export: one RS-274X file per non-empty layer.
    connect(gerberExportAct, &QAction::triggered, this, [this](){
        QString path = QFileDialog::getSaveFileName(this, _("Export Gerber"),
                                                    QString(), "Gerber (*.gbr);;All files (*)");
        if(path.isEmpty())
            return;
        QString base = path;                       // strip extension for per-layer names
        int dot = base.lastIndexOf('.'), slash = base.lastIndexOf('/');
        if(dot > slash)
            base.truncate(dot);
        Board *b = pcb.GetSelectedBoard();
        float h = b->GetSize().y;
        static const char *names[7] = {"C1", "S1", "C2", "S2", "I1", "I2", "O"};
        int written = 0;
        for(int layer = 0; layer < 7; layer++) {
            GerberWriter w(h);
            bool any = false;
            for(Object *o = b->GetObjects(); o; o = o->GetNext())
                if(o->GetLayer() == layer) {
                    o->ExportGerber(w);
                    any = true;
                }
            if(!any)
                continue;
            std::ofstream out(QString("%1.%2.gbr").arg(base).arg(names[layer]).toLocal8Bit().constData());
            if(out) {
                w.write(out);
                written++;
            }
        }
        // Solder-mask layers: openings over every pad on the side (through-pads
        // open on both), plus any copper object explicitly flagged into the mask.
        GerberWriter maskTop(h), maskBot(h);
        bool anyTop = false, anyBot = false;
        for(Object *o = b->GetObjects(); o; o = o->GetNext()) {
            if(o->GetType() == Object::THT_PAD) {
                o->ExportGerber(maskTop); anyTop = true;
                o->ExportGerber(maskBot); anyBot = true;
            } else if(o->GetType() == Object::SMD_PAD || o->HasSoldermask()) {
                if(o->GetLayer() == ObjectGroup::LAYER_C1) { o->ExportGerber(maskTop); anyTop = true; }
                else if(o->GetLayer() == ObjectGroup::LAYER_C2) { o->ExportGerber(maskBot); anyBot = true; }
            }
        }
        if(anyTop) {
            std::ofstream out(QString("%1.MASKTOP.gbr").arg(base).toLocal8Bit().constData());
            if(out) { maskTop.write(out); written++; }
        }
        if(anyBot) {
            std::ofstream out(QString("%1.MASKBOT.gbr").arg(base).toLocal8Bit().constData());
            if(out) { maskBot.write(out); written++; }
        }
        QMessageBox::information(this, _("Export Gerber"),
                                 QString(_("Exported %1 layer file(s).")).arg(written));
    });

    // Isolation: approximate milling clearance = copper grown by the cutter
    // diameter, emitted as Gerber per copper layer. Not a true single-pass
    // toolpath (no polygon offset / overlap removal).
    connect(isolationAct, &QAction::triggered, this, [this](){
        bool ok = false;
        double cutter = QInputDialog::getDouble(this, _("Isolation milling"),
                                                _("Cutter diameter (mm):"), 0.2, 0.01, 5.0, 2, &ok);
        if(!ok)
            return;
        QString path = QFileDialog::getSaveFileName(this, _("Export isolation"),
                                                    QString(), "Gerber (*.gbr);;All files (*)");
        if(path.isEmpty())
            return;
        QString base = path;
        int dot = base.lastIndexOf('.'), slash = base.lastIndexOf('/');
        if(dot > slash)
            base.truncate(dot);
        Board *b = pcb.GetSelectedBoard();
        float h = b->GetSize().y;
        const int copper[2] = {ObjectGroup::LAYER_C1, ObjectGroup::LAYER_C2};
        const char *names[2] = {"C1", "C2"};
        int written = 0;
        for(int idx = 0; idx < 2; idx++) {
            GerberWriter w(h);
            w.setInflate((float)cutter);
            bool any = false;
            for(Object *o = b->GetObjects(); o; o = o->GetNext())
                if(o->GetLayer() == copper[idx]) {
                    o->ExportGerber(w);
                    any = true;
                }
            if(!any)
                continue;
            std::ofstream out(QString("%1.iso.%2.gbr").arg(base).arg(names[idx]).toLocal8Bit().constData());
            if(out) {
                w.write(out);
                written++;
            }
        }
        QMessageBox::information(this, _("Export isolation"),
                                 QString(_("Exported %1 isolation layer(s).")).arg(written));
    });

    // Save macro / export elements: write the selected objects to a file
    // (OpenLayout's own binary object format).
    auto saveSelected = [this](){
        Board *b = pcb.GetSelectedBoard();
        if(!b->IsSelected()) {
            QMessageBox::information(this, _("Save macro"), _("Nothing is selected."));
            return;
        }
        QString path = QFileDialog::getSaveFileName(this, _("Save macro"), QString(),
                                                    "OpenLayout macro (*.olm);;All files (*)");
        if(path.isEmpty())
            return;
        if(!path.contains('.'))
            path += ".olm";
        File f(path.toLocal8Bit().constData(), "wb");
        if(!f.IsOk()) {
            QMessageBox::warning(this, _("Save macro"), _("Could not write the file."));
            return;
        }
        uint32_t n = 0;
        for(Object *o = b->GetObjects(); o; o = o->GetNext())
            if(o->IsSelected()) n++;
        f.Write<uint32_t>(n);
        for(Object *o = b->GetObjects(); o; o = o->GetNext())
            if(o->IsSelected()) o->Save(f);
    };
    connect(saveMacroAct,     &QAction::triggered, this, saveSelected);
    connect(elementExportAct, &QAction::triggered, this, saveSelected);

    // Import elements / load macro / place footprint: append objects from such
    // a file (all three use the same OpenLayout macro format).
    auto loadMacro = [this](const QString &path){
        File f(path.toLocal8Bit().constData(), "rb");
        if(!f.IsOk()) {
            QMessageBox::warning(this, _("Import elements"), _("Could not open the file."));
            return;
        }
        PushUndo();
        Board *b = pcb.GetSelectedBoard();
        b->UnselectAll();
        uint32_t n = f.Read<uint32_t>();
        for(uint32_t i = 0; i < n; i++) {
            Object *o = Object::Load(f);
            if(!o)
                break;        // unknown type or EOF: stop instead of looping
            o->Select();
            b->AddObjectEnd(o);
        }
        mainCanvas->update();
    };
    auto importElements = [this, loadMacro](){
        QString path = QFileDialog::getOpenFileName(this, _("Import elements"), QString(),
                                                    "OpenLayout macro (*.olm);;All files (*)");
        if(!path.isEmpty())
            loadMacro(path);
    };
    connect(elementImportAct, &QAction::triggered, this, importElements);

    // Macro-Library panel: list .olm macros from the macro folder; double-click
    // loads one onto the board (selected, ready to drag into place).
    QDockWidget *macroDock = new QDockWidget(_("Macro-Library"), this);
    {
        QWidget *panel = new QWidget(macroDock);
        QVBoxLayout *ml = new QVBoxLayout(panel);
        QPushButton *folder = new QPushButton(_("Folder…"), panel);
        QListWidget *macroList = new QListWidget(panel);
        ml->addWidget(folder);
        ml->addWidget(macroList);
        macroDock->setWidget(panel);

        auto refreshMacros = [this, macroList](){
            macroList->clear();
            QDir dir(QString::fromLocal8Bit(settings.macroDir));
            if(settings.macroDir[0] && dir.exists())
                macroList->addItems(dir.entryList(QStringList() << "*.olm", QDir::Files, QDir::Name));
        };
        connect(folder, &QPushButton::clicked, this, [this, refreshMacros](){
            QString d = QFileDialog::getExistingDirectory(this, _("Macro folder"),
                            QString::fromLocal8Bit(settings.macroDir));
            if(!d.isEmpty()) {
                strncpy(settings.macroDir, d.toLocal8Bit().constData(), sizeof(settings.macroDir) - 1);
                settings.macroDir[sizeof(settings.macroDir) - 1] = '\0';
                refreshMacros();
            }
        });
        connect(macroList, &QListWidget::itemDoubleClicked, this,
                [this, loadMacro](QListWidgetItem *item){
            loadMacro(QString::fromLocal8Bit(settings.macroDir) + "/" + item->text());
        });
        connect(macroDock, &QDockWidget::visibilityChanged, this,
                [refreshMacros](bool vis){ if(vis) refreshMacros(); });
    }
    addDockWidget(Qt::RightDockWidgetArea, macroDock);
    macroDock->hide();
    panelMacroAct->setCheckable(true);
    connect(panelMacroAct, &QAction::toggled, macroDock, &QDockWidget::setVisible);
    connect(macroDock, &QDockWidget::visibilityChanged, panelMacroAct, &QAction::setChecked);

    // Components panel: list the grouped objects (placed footprints/macros) as
    // components; clicking one selects all of its objects.
    QDockWidget *compDock = new QDockWidget(_("Components"), this);
    {
        QWidget *panel = new QWidget(compDock);
        QVBoxLayout *cl = new QVBoxLayout(panel);
        QPushButton *refresh = new QPushButton(_("Refresh"), panel);
        QListWidget *compList = new QListWidget(panel);
        cl->addWidget(refresh);
        cl->addWidget(compList);
        compDock->setWidget(panel);

        auto refreshComps = [this, compList](){
            compList->clear();
            std::vector<uint32_t> gids;
            for(Object *o = pcb.GetSelectedBoard()->GetObjects(); o; o = o->GetNext())
                for(int i = 0; i < o->groups.Size(); i++) {
                    uint32_t g = o->groups[i];
                    if(std::find(gids.begin(), gids.end(), g) == gids.end())
                        gids.push_back(g);
                }
            std::sort(gids.begin(), gids.end());
            int n = 0;
            for(uint32_t g : gids) {
                QListWidgetItem *it = new QListWidgetItem(QString(_("Component %1")).arg(++n));
                it->setData(Qt::UserRole, g);
                compList->addItem(it);
            }
        };
        connect(refresh, &QPushButton::clicked, this, refreshComps);
        connect(compList, &QListWidget::currentRowChanged, this, [this, compList](int row){
            if(row < 0)
                return;
            uint32_t g = compList->item(row)->data(Qt::UserRole).toUInt();
            Board *b = pcb.GetSelectedBoard();
            b->UnselectAll();
            for(Object *o = b->GetObjects(); o; o = o->GetNext())
                if(o->groups.Find(g))
                    o->Select();
            mainCanvas->update();
        });
        connect(compDock, &QDockWidget::visibilityChanged, this,
                [refreshComps](bool vis){ if(vis) refreshComps(); });
    }
    addDockWidget(Qt::RightDockWidgetArea, compDock);
    compDock->hide();
    panelComponentsAct->setCheckable(true);
    connect(panelComponentsAct, &QAction::toggled, compDock, &QDockWidget::setVisible);
    connect(compDock, &QDockWidget::visibilityChanged, panelComponentsAct, &QAction::setChecked);

    // DRC panel: run the check, list the violations, click one to centre on it.
    QDockWidget *drcDock = new QDockWidget(_("DRC"), this);
    {
        QWidget *panel = new QWidget(drcDock);
        QVBoxLayout *dl = new QVBoxLayout(panel);
        QPushButton *run = new QPushButton(_("Run DRC"), panel);
        QListWidget *drcList = new QListWidget(panel);
        dl->addWidget(run);
        dl->addWidget(drcList);
        drcDock->setWidget(panel);

        auto runDrc = [this, drcList](){
            drcList->clear();
            auto issues = pcb.GetSelectedBoard()->CheckDRC(settings.groundDistance);
            if(issues.empty()) {
                drcList->addItem(_("No violations."));
                return;
            }
            for(const auto &is : issues) {
                QListWidgetItem *it = new QListWidgetItem(QString("%1  (%2, %3)")
                    .arg(QString::fromStdString(is.second))
                    .arg(is.first.x, 0, 'f', 2).arg(is.first.y, 0, 'f', 2));
                it->setData(Qt::UserRole, QPointF(is.first.x, is.first.y));
                drcList->addItem(it);
            }
        };
        connect(run, &QPushButton::clicked, this, runDrc);
        connect(drcList, &QListWidget::currentRowChanged, this, [this, drcList](int row){
            if(row < 0)
                return;
            QVariant v = drcList->item(row)->data(Qt::UserRole);
            if(v.isValid()) {
                QPointF p = v.toPointF();
                mainCanvas->CenterOn(Vec2(p.x(), p.y()));
            }
        });
        connect(drcDock, &QDockWidget::visibilityChanged, this,
                [runDrc](bool vis){ if(vis) runDrc(); });
    }
    addDockWidget(Qt::RightDockWidgetArea, drcDock);
    drcDock->hide();
    panelDrcAct->setCheckable(true);
    connect(panelDrcAct, &QAction::toggled, drcDock, &QDockWidget::setVisible);
    connect(drcDock, &QDockWidget::visibilityChanged, panelDrcAct, &QAction::setChecked);
    connect(footprintAct,     &QAction::triggered, this, importElements);

    // Reset solder mask on all objects.
    connect(resetMaskAct, &QAction::triggered, this,
            editUndo([](Board *b){ b->ResetSoldermask(); }));

    // Copper pour: fill the selected zone (Poly), clearing other-net copper.
    connect(fillZoneAct, &QAction::triggered, this, [this](){
        Board *b = pcb.GetSelectedBoard();
        Object *sel = b->GetFirstSelected();
        if(!sel || sel->GetType() != Object::POLY) {
            QMessageBox::information(this, _("Fill zone"),
                _("Select a zone (filled polygon) first."));
            return;
        }
        PushUndo();
        b->FillZone(sel, settings.groundDistance, settings.trackSize);
        mainCanvas->update();
    });


    // List drillings: summarise the through holes by diameter.
    connect(listDrillingsAct, &QAction::triggered, this, [this](){
        std::map<int, int> counts;
        for(Object *o = pcb.GetSelectedBoard()->GetObjects(); o; o = o->GetNext()) {
            float d = o->GetDrillDiameter();
            if(d > 0.0f)
                counts[(int)lround(d * 1000.0f)]++;
        }
        QString text;
        int total = 0;
        for(const auto &kv : counts) {
            text += QString("Ø %1 mm: %2\n").arg(kv.first / 1000.0).arg(kv.second);
            total += kv.second;
        }
        if(total == 0)
            text = _("No through holes on this board.");
        else
            text.prepend(QString(_("%1 holes total\n\n")).arg(total));
        QMessageBox::information(this, _("Drillings"), text);
    });

    // Massive: duplicate the selection in a rectangular array.
    connect(massiveAct, &QAction::triggered, this, [this](){
        if(!pcb.GetSelectedBoard()->IsSelected()) {
            QMessageBox::information(this, _("Array"), _("Select something first."));
            return;
        }
        QDialog dlg(this);
        dlg.setWindowTitle(_("Array (massive duplicate)"));
        QFormLayout *form = new QFormLayout(&dlg);
        QSpinBox *cols = new QSpinBox(&dlg); cols->setRange(1, 1000); cols->setValue(2);
        QSpinBox *rows = new QSpinBox(&dlg); rows->setRange(1, 1000); rows->setValue(2);
        QDoubleSpinBox *dx = new QDoubleSpinBox(&dlg); dx->setRange(-1000, 1000); dx->setDecimals(3); dx->setValue(5.0);
        QDoubleSpinBox *dy = new QDoubleSpinBox(&dlg); dy->setRange(-1000, 1000); dy->setDecimals(3); dy->setValue(5.0);
        form->addRow(_("Columns:"), cols);
        form->addRow(_("Rows:"), rows);
        form->addRow(_("X spacing (mm):"), dx);
        form->addRow(_("Y spacing (mm):"), dy);
        QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
        form->addRow(bb);
        connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        if(dlg.exec() == QDialog::Accepted) {
            PushUndo();
            pcb.GetSelectedBoard()->ArraySelected(cols->value(), rows->value(),
                                                  Vec2(dx->value(), dy->value()));
            mainCanvas->update();
        }
    });

    // Board import: append the boards from another .lay6 file.
    connect(boardImportAct, &QAction::triggered, this, [this](){
        QString path = QFileDialog::getOpenFileName(this, _("Import board"), QString(),
                                                    "Sprint-Layout 6 (*.lay6);;All files (*)");
        if(path.isEmpty())
            return;
        File f(path.toLocal8Bit().constData(), "rb");
        PCB temp;
        if(!f.IsOk() || !temp.Load(f)) {
            QMessageBox::warning(this, _("Import board"), _("Could not open the file."));
            return;
        }
        for(uint32_t i = 0; i < temp.Size(); i++)
            pcb.AddBoard(temp[i]);            // ownership transfers; temp won't free boards
        mainCanvas->SetBoard(pcb.GetSelectedBoard());
        RebuildBoardTabs();
    });

    // Bitmap underlay: pick an image to show under the board, set its DPI.
    connect(bitmapAct, &QAction::triggered, this, [this](){
        ImageConfig &ic = pcb.GetSelectedBoard()->images.Get(0);
        QString path = QFileDialog::getOpenFileName(this, _("Bitmap underlay"),
                            QString::fromLocal8Bit(ic.path),
                            "Images (*.png *.jpg *.jpeg *.bmp *.gif);;All files (*)");
        if(path.isEmpty())
            return;
        bool ok = false;
        int dpi = QInputDialog::getInt(this, _("Bitmap underlay"), _("Image resolution (DPI):"),
                                       ic.dpi ? ic.dpi : 600, 1, 10000, 1, &ok);
        if(!ok)
            return;
        strncpy(ic.path, path.toLocal8Bit().constData(), 199);
        ic.path[199] = '\0';
        ic.dpi = dpi;
        ic.enabled = 1;
        ic.Reload();
        mainCanvas->update();
    });

    // Printing: render the current canvas view to a printer or PDF.
    connect(printSetupAct, &QAction::triggered, this, [this](){
        if(!printer)
            printer = new QPrinter;
        QPageSetupDialog dlg(printer, this);
        dlg.exec();
    });
    connect(printAct, &QAction::triggered, this, [this](){
        if(!printer)
            printer = new QPrinter;
        QPrintDialog dlg(printer, this);
        if(dlg.exec() != QDialog::Accepted)
            return;
        QImage img = mainCanvas->grabFramebuffer();
        QPainter painter(printer);
        QRect page = painter.viewport();
        QSize size = img.size();
        size.scale(page.size(), Qt::KeepAspectRatio);
        painter.setViewport(page.x(), page.y(), size.width(), size.height());
        painter.setWindow(img.rect());
        painter.drawImage(0, 0, img);
    });

    // Board properties: rename and toggle multilayer.
    connect(boardPropAct, &QAction::triggered, this, [this](){
        Board *b = pcb.GetSelectedBoard();
        QDialog dlg(this);
        dlg.setWindowTitle(_("Board properties"));
        QFormLayout *form = new QFormLayout(&dlg);
        QLineEdit *nameEdit = new QLineEdit(b->GetName(), &dlg);
        QCheckBox *multi = new QCheckBox(&dlg);
        multi->setChecked(b->IsMultilayer());
        form->addRow(_("Name:"), nameEdit);
        form->addRow(_("Multilayer:"), multi);
        QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
        form->addRow(bb);
        connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        if(dlg.exec() == QDialog::Accepted) {
            b->SetName(nameEdit->text().toLocal8Bit().constData());
            if(multi->isChecked() != b->IsMultilayer())
                b->ToggleMultilayer();
            RebuildBoardTabs();
            mainCanvas->update();
        }
    });

    // Project info: edit the project metadata.
    connect(projectInfoAct, &QAction::triggered, this, [this](){
        ProjectInfo &info = pcb.info;
        QDialog dlg(this);
        dlg.setWindowTitle(_("Project information"));
        QFormLayout *form = new QFormLayout(&dlg);
        QLineEdit *title   = new QLineEdit(info.GetTitle(), &dlg);
        QLineEdit *author  = new QLineEdit(info.GetAuthor(), &dlg);
        QLineEdit *company = new QLineEdit(info.GetCompany(), &dlg);
        QTextEdit *comment = new QTextEdit(info.GetComment(), &dlg);
        form->addRow(_("Title:"),   title);
        form->addRow(_("Author:"),  author);
        form->addRow(_("Company:"), company);
        form->addRow(_("Comment:"), comment);
        QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
        form->addRow(bb);
        connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        if(dlg.exec() == QDialog::Accepted) {
            info.SetTitle(title->text().toLocal8Bit().constData());
            info.SetAuthor(author->text().toLocal8Bit().constData());
            info.SetCompany(company->text().toLocal8Bit().constData());
            info.SetComment(comment->toPlainText().toLocal8Bit().constData());
        }
    });

    // Settings / about (directories live in the same dialog)
    auto openSettings = [this]() {
        SettingsDialog dlg(settings, this);
        if(dlg.exec() == QDialog::Accepted) {
            settings = dlg.Result();
            mainCanvas->update();
        }
    };
    connect(settingsAct,    &QAction::triggered, this, openSettings);
    connect(directoriesAct, &QAction::triggered, this, openSettings);
    connect(aboutAct, &QAction::triggered, this, [this](){
        QMessageBox::about(this, _("About OpenLayout"),
            "OpenLayout\nhttps://github.com/nikita-yfh/OpenLayout");
    });

    // Properties panel: edit the selected objects' line width.
    propsDock = new QDockWidget(_("Properties"), this);
    {
        QWidget *panel = new QWidget(propsDock);
        QFormLayout *pf = new QFormLayout(panel);
        QDoubleSpinBox *widthSpin = new QDoubleSpinBox(panel);
        widthSpin->setRange(0.01, 100.0);
        widthSpin->setDecimals(3);
        widthSpin->setValue(0.3);
        QPushButton *applyWidth = new QPushButton(_("Apply to selection"), panel);
        pf->addRow(_("Track width (mm):"), widthSpin);
        pf->addRow(applyWidth);
        propsDock->setWidget(panel);
        connect(applyWidth, &QPushButton::clicked, this, [this, widthSpin](){
            PushUndo();
            pcb.GetSelectedBoard()->SetSelectedWidth(widthSpin->value());
            mainCanvas->update();
        });
    }
    addDockWidget(Qt::RightDockWidgetArea, propsDock);
    propsDock->hide();
    panelPropertiesAct->setCheckable(true);
    connect(panelPropertiesAct, &QAction::toggled, propsDock, &QDockWidget::setVisible);
    connect(propsDock, &QDockWidget::visibilityChanged, panelPropertiesAct, &QAction::setChecked);

    // Selector panel: list the objects on the board and select one on click.
    selectorDock = new QDockWidget(_("Selector"), this);
    {
        QWidget *panel = new QWidget(selectorDock);
        QVBoxLayout *sl = new QVBoxLayout(panel);
        QPushButton *refresh = new QPushButton(_("Refresh"), panel);
        selectorList = new QListWidget(panel);
        sl->addWidget(refresh);
        sl->addWidget(selectorList);
        selectorDock->setWidget(panel);

        auto refreshSelector = [this](){
            selectorList->clear();
            selectorItems.clear();
            int i = 0;
            for(Object *o = pcb.GetSelectedBoard()->GetObjects(); o; o = o->GetNext()) {
                const char *t;
                switch(o->GetType()) {
                    case Object::THT_PAD: t = "THT pad"; break;
                    case Object::SMD_PAD: t = "SMD pad"; break;
                    case Object::TRACK:   t = "Track";   break;
                    case Object::POLY:    t = "Polygon"; break;
                    case Object::CIRCLE:  t = "Circle";  break;
                    default:              t = "Object";  break;
                }
                selectorList->addItem(QString("%1 %2").arg(t).arg(++i));
                selectorItems.push_back(o);
            }
        };
        connect(refresh, &QPushButton::clicked, this, refreshSelector);
        connect(selectorList, &QListWidget::currentRowChanged, this, [this](int row){
            if(row < 0 || row >= (int)selectorItems.size())
                return;
            Object *target = selectorItems[row];
            Board *b = pcb.GetSelectedBoard();
            for(Object *o = b->GetObjects(); o; o = o->GetNext())
                if(o == target) {                 // still on the board
                    b->UnselectAll();
                    target->Select();
                    mainCanvas->update();
                    return;
                }
        });
        connect(selectorDock, &QDockWidget::visibilityChanged, this,
                [refreshSelector](bool vis){ if(vis) refreshSelector(); });
    }
    addDockWidget(Qt::RightDockWidgetArea, selectorDock);
    selectorDock->hide();
    panelSelectorAct->setCheckable(true);
    connect(panelSelectorAct, &QAction::toggled, selectorDock, &QDockWidget::setVisible);
    connect(selectorDock, &QDockWidget::visibilityChanged, panelSelectorAct, &QAction::setChecked);

    // Image export: grab the rendered framebuffer and save it.
    auto exportImage = [this](const char *fmt, const char *filter) {
        return [this, fmt, filter]() {
            QString path = QFileDialog::getSaveFileName(this, _("Export image"), QString(), filter);
            if(path.isEmpty())
                return;
            if(!mainCanvas->grabFramebuffer().save(path, fmt))
                QMessageBox::warning(this, _("Export image"), _("Could not export the image."));
        };
    };
    connect(savePngAct, &QAction::triggered, this, exportImage("PNG", "PNG image (*.png)"));
    connect(saveJpgAct, &QAction::triggered, this, exportImage("JPG", "JPEG image (*.jpg)"));
    connect(saveBmpAct, &QAction::triggered, this, exportImage("BMP", "BMP image (*.bmp)"));
    connect(saveGifAct, &QAction::triggered, this, exportImage("GIF", "GIF image (*.gif)"));

    // Autosave: periodically rewrite the current file while enabled.
    autosaveAct->setCheckable(true);
    QTimer *autosaveTimer = new QTimer(this);
    autosaveTimer->setInterval(60000);
    connect(autosaveTimer, &QTimer::timeout, this, [this]() {
        if(!currentFile.isEmpty())
            SaveToPath(currentFile);
    });
    connect(autosaveAct, &QAction::toggled, this, [this, autosaveTimer](bool on) {
        settings.autosave = on;
        if(on)
            autosaveTimer->start();
        else
            autosaveTimer->stop();
    });
}

static const char *fileFilter = "Sprint-Layout 6 (*.lay6);;All files (*)";

void MainWindow::NewFile() {
    ClearHistory();
    for(uint32_t i = 0; i < pcb.Size(); i++)
        delete pcb[i];
    pcb.Clear();
    pcb.AddBoard(new Board(_("Board"), Board::Type::Rectangle,
                           Vec2(100.0f, 80.0f), 5.0f, false));
    currentFile.clear();
    mainCanvas->SetBoard(pcb.GetSelectedBoard());
    RebuildBoardTabs();
}

void MainWindow::OpenFile() {
    QString path = QFileDialog::getOpenFileName(this, _("Open"), QString(), fileFilter);
    if(path.isEmpty())
        return;
    File file(path.toLocal8Bit().constData(), "rb");
    if(!file.IsOk() || !pcb.Load(file)) {
        QMessageBox::warning(this, _("Open"), _("Could not open file."));
        return;
    }
    currentFile = path;
    ClearHistory();
    mainCanvas->SetBoard(pcb.GetSelectedBoard());
    RebuildBoardTabs();
}

void MainWindow::SaveFile() {
    if(currentFile.isEmpty())
        SaveFileAs();
    else
        SaveToPath(currentFile);
}

void MainWindow::SaveFileAs() {
    QString path = QFileDialog::getSaveFileName(this, _("Save as"), QString(), fileFilter);
    if(path.isEmpty())
        return;
    if(!path.contains('.'))
        path += ".lay6";
    if(SaveToPath(path))
        currentFile = path;
}

bool MainWindow::SaveToPath(const QString &path) {
    File file(path.toLocal8Bit().constData(), "wb");
    if(!file.IsOk()) {
        QMessageBox::warning(this, _("Save"), _("Could not write file."));
        return false;
    }
    pcb.Save(file);
    return true;
}

void MainWindow::CreateToolBar() {
    toolBarMain = addToolBar(_("File"));
    toolBarMain->addAction(boardNewAct);
    toolBarMain->addAction(openAct);
    toolBarMain->addAction(saveAct);
    toolBarMain->addAction(printAct);

    toolBarHistory = addToolBar(_("History"));
    toolBarHistory->addAction(undoAct);
    toolBarHistory->addAction(redoAct);

    toolBarClipboard = addToolBar(_("Clipboard"));
    toolBarClipboard->addAction(cutAct);
    toolBarClipboard->addAction(copyAct);
    toolBarClipboard->addAction(pasteAct);
    toolBarClipboard->addAction(deleteAct);

    toolBarActions = addToolBar(_("Actions"));
    toolBarActions->addAction(duplicateAct);

    toolBarActions->addAction(rotateAct);

    { // Create menu for rotate button
        QMenu *rotateMenu = new QMenu(this);
        rotate90Act = rotateMenu->addAction(_("90°"));
        rotate15Act = rotateMenu->addAction(_("15°"));
        rotate45Act = rotateMenu->addAction(_("45°"));
        rotate5Act  = rotateMenu->addAction(_("5°"));
        rotateMenu->addSeparator();
        rotateCustomAct = rotateMenu->addAction(_("22.5° ..."));

        rotate90Act->setCheckable(true);
        rotate45Act->setCheckable(true);
        rotate15Act->setCheckable(true);
        rotate5Act->setCheckable(true);
        rotateCustomAct->setCheckable(true);

        QActionGroup *rotateGroup = new QActionGroup(this);
        rotateGroup->addAction(rotate90Act);
        rotateGroup->addAction(rotate45Act);
        rotateGroup->addAction(rotate15Act);
        rotateGroup->addAction(rotate5Act);
        rotateGroup->addAction(rotateCustomAct);
        rotateGroup->setExclusive(true);

        rotateAct->setMenu(rotateMenu);
        qobject_cast<QToolButton*>(toolBarActions->widgetForAction(rotateAct))->setPopupMode(QToolButton::MenuButtonPopup);
    }

    toolBarActions->addAction(hmirrorAct);
    toolBarActions->addAction(vmirrorAct);
    toolBarActions->addAction(alignAct);
    { // Create menu for align button
        QMenu *alignMenu = new QMenu(this);
        alignMenu->addAction(alignTopAct);
        alignMenu->addAction(alignBottomAct);
        alignMenu->addAction(alignLeftAct);
        alignMenu->addAction(alignRightAct);
        alignMenu->addAction(alignHAct);
        alignMenu->addAction(alignVAct);
        alignAct->setMenu(alignMenu);
        qobject_cast<QToolButton*>(toolBarActions->widgetForAction(alignAct))->setPopupMode(QToolButton::InstantPopup);
    }

    toolBarActions->addAction(snapGridAct);
    toolBarActions->addSeparator();
    toolBarActions->addAction(removeConAct);
    toolBarActions->addSeparator();
    toolBarActions->addAction(groupAct);
    toolBarActions->addAction(ungroupAct);
    toolBarZoom = addToolBar(_("Zoom functions"));
    toolBarZoom->addAction(zoomAct);
    { // Create menu for zoom button
        QMenu *zoomMenu = new QMenu(this);
        zoomMenu->addAction(zoomPreviousAct);
        zoomMenu->addSeparator();
        zoomMenu->addAction(zoomBoardAct);
        zoomMenu->addAction(zoomObjectsAct);
        zoomMenu->addAction(zoomSelectionAct);

        zoomAct->setMenu(zoomMenu);
        qobject_cast<QToolButton*>(toolBarZoom->widgetForAction(zoomAct))->setPopupMode(QToolButton::InstantPopup);
    }

    toolBarPCB = addToolBar(_("PCB actions"));
    toolBarPCB->addAction(transparentAct);
    transparentAct->setCheckable(true);
    toolBarPCB->addAction(projectInfoAct);
    toolBarPCB->addAction(bitmapAct);

    toolBarPanels = addToolBar(_("Panel visibility"));
    {
        QWidget* panelsSpacer = new QWidget();
        panelsSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        toolBarPanels->addWidget(panelsSpacer);
    }
    toolBarPanels->addAction(panelSelectorAct);
    toolBarPanels->addAction(panelComponentsAct);
    toolBarPanels->addAction(panelPropertiesAct);
    toolBarPanels->addAction(panelDrcAct);
    toolBarPanels->addAction(panelMacroAct);
}

void MainWindow::CreateLeftPanel() {
    toolPanel = new ToolPanel(settings, this);
    addToolBar(Qt::LeftToolBarArea, toolPanel);

    gridPanel = new GridPanel(settings, this);
    addToolBar(Qt::LeftToolBarArea, gridPanel);
}

void MainWindow::CreateActions() {
#define I(image) QIcon(QPixmap(image))
    boardNewAct         = new QAction(I(new_xpm),            _("&Add new board"), this);
    openAct             = new QAction(I(open_xpm),           _("&Open"), this);
    saveAct             = new QAction(I(save_xpm),           _("&Save"), this);
    printAct            = new QAction(I(print_xpm),          _("&Print"), this);
    undoAct             = new QAction(I(undo_xpm),           _("&Undo"), this);
    redoAct             = new QAction(I(redo_xpm),           _("&Redo"), this);
    copyAct             = new QAction(I(copy_xpm),           _("C&opy"), this);
    cutAct              = new QAction(I(cut_xpm),            _("&Cut"), this);
    pasteAct            = new QAction(I(paste_xpm),          _("&Paste"), this);
    duplicateAct        = new QAction(I(duplicate_xpm),      _("Dup&licate"), this);
    deleteAct           = new QAction(I(delete_xpm),         _("&Delete"), this);
    rotateAct           = new QAction(I(rotate_xpm),         _("&Rotate"), this);
    hmirrorAct          = new QAction(I(mirror_h_xpm),       _("Mirror &horisontal"), this);
    vmirrorAct          = new QAction(I(mirror_v_xpm),       _("Mirror &vertical"), this);
    alignAct            = new QAction(I(align_xpm),          _("Align elements"), this);
    alignTopAct         = new QAction(I(align_top_xpm),      _("Align top"), this);
    alignBottomAct      = new QAction(I(align_bottom_xpm),   _("Align bottom"), this);
    alignLeftAct        = new QAction(I(align_left_xpm),     _("Align left"), this);
    alignRightAct       = new QAction(I(align_right_xpm),    _("Align right"), this);
    alignHAct           = new QAction(I(align_hcenter_xpm),  _("Align horizontal-center"), this);
    alignVAct           = new QAction(I(align_vcenter_xpm),  _("Align vertical-center"), this);
    snapGridAct         = new QAction(I(to_grid_xpm),        _("S&nap to grid"), this);
    removeConAct        = new QAction(I(remove_con_xpm),     _("&Remove connections (rubberbands)"), this);
    groupAct            = new QAction(I(group_on_xpm),       _("Build &group"), this);
    ungroupAct          = new QAction(I(group_off_xpm),      _("Split gro&up"), this);
    transparentAct      = new QAction(I(transparent_xpm),    _("Toggle transparent mode"), this);
    zoomAct             = new QAction(I(zoom_any_xpm),       _("Zoom functions"), this);
    zoomPreviousAct     = new QAction(I(zoom_prev_xpm),      _("&Zoom previous"), this);
    zoomBoardAct        = new QAction(I(zoom_board_xpm),     _("Zoom &board"), this);
    zoomObjectsAct      = new QAction(I(zoom_objects_xpm),   _("Zoom &objects"), this);
    zoomSelectionAct    = new QAction(I(zoom_selection_xpm), _("Zoom &selection"), this);
    projectInfoAct      = new QAction(I(info_xpm),           _("&Project info"), this);
    bitmapAct           = new QAction(I(bitmap_xpm),         _("&Scanned copy"), this);
    panelMacroAct       = new QAction(I(macro_xpm),          _("&Macro-Library"), this);
    panelPropertiesAct  = new QAction(I(properties_xpm),     _("&Properties-Panel"), this);
    panelDrcAct         = new QAction(I(drc_xpm),            _("&DRC-Panel"), this);
    panelComponentsAct  = new QAction(I(components_xpm),     _("&Components-Panel"), this);
    panelSelectorAct    = new QAction(I(selector_xpm),       _("S&elector-Panel"), this);
#undef I
    panelMacroAct     ->setCheckable(true);
    panelPropertiesAct->setCheckable(true);
    panelDrcAct       ->setCheckable(true);
    panelComponentsAct->setCheckable(true);
    panelSelectorAct  ->setCheckable(true);
    newAct              = new QAction(_("&New"), this);
    saveasAct           = new QAction(_("S&ave as"), this);
    saveMacroAct        = new QAction(_("Save as &macro"), this);
    autosaveAct         = new QAction(_("A&utoSave"), this);
    gerberImportAct     = new QAction(_("&Gerber-Import"), this);
    gerberExportAct     = new QAction(_("Gerber Export"), this);
    drillDataAct        = new QAction(_("Drill data (Excellon)"), this);
    isolationAct        = new QAction(_("Isolation milling (HPGL, *.plt)"), this);
    saveBmpAct          = new QAction(_("Bitmap (*.bmp)"), this);
    saveJpgAct          = new QAction(_("JPG (*.jpg)"), this);
    saveGifAct          = new QAction(_("GIF (*.gif)"), this);
    saveEmpAct          = new QAction(_("EMP (*.emp)"), this);
    savePngAct          = new QAction(_("PNG (*.png)"), this);
    directoriesAct      = new QAction(_("&Directories"), this);
    printSetupAct       = new QAction(_("P&rinter setup"), this);
    exitAct             = new QAction(_("&Exit"), this);
    selectallAct        = new QAction(_("Select &all"), this);
    boardPropAct        = new QAction(_("&Properties"), this);
    boardCopyAct        = new QAction(_("&Copy board"), this);
    boardDeleteAct      = new QAction(_("&Delete board"), this);
    boardSetRightAct    = new QAction(_("Set board to &right"), this);
    boardSetLeftAct     = new QAction(_("Set board to &left"), this);
    boardMoveRightAct   = new QAction(_("&Move board to right"), this);
    boardMoveLeftAct    = new QAction(_("M&ove board to left"), this);
    boardImportAct      = new QAction(_("&Import pages from file"), this);
    boardSaveAct        = new QAction(_("&Save pages to file"), this);
    changeSideAct       = new QAction(_("&Change board side"), this);
    layerC1Act          = new QAction(_("&C1"), this);
    layerS1Act          = new QAction(_("S&1"), this);
    layerC2Act          = new QAction(_("C&2"), this);
    layerS2Act          = new QAction(_("&S2"), this);
    layerI1Act          = new QAction(_("I1"), this);
    layerI2Act          = new QAction(_("&I2"), this);
    layerOAct           = new QAction(_("&O"), this);
    massiveAct          = new QAction(_("&Tile / Arrange circular"), this);
    listDrillingsAct    = new QAction(_("&List drillings"), this);
    footprintAct        = new QAction(_("&Footprint-Wizard"), this);
    resetMaskAct        = new QAction(_("R&eset solder mask"), this);
    fillZoneAct         = new QAction(_("Fill &zone (copper pour)"), this);
    deleteOutsideAct    = new QAction(_("&Delete elements outside the board"), this);
    elementExportAct    = new QAction(_("Te&xt-IO: Export elements"), this);
    elementImportAct    = new QAction(_("&Text-IO: Import elements"), this);
    settingsAct         = new QAction(_("&General settings"), this);
    aboutAct            = new QAction(_("&About"), this);

    newAct        ->setShortcut(QKeySequence::New);
    openAct       ->setShortcut(QKeySequence::Open);
    saveAct       ->setShortcut(QKeySequence::Save);
    saveasAct     ->setShortcut(QKeySequence::SaveAs);
    printAct      ->setShortcut(QKeySequence::Print);
    exitAct       ->setShortcut(QKeySequence::Quit);
    undoAct       ->setShortcut(QKeySequence::Undo);
    redoAct       ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
    copyAct       ->setShortcut(QKeySequence::Copy);
    cutAct        ->setShortcut(QKeySequence::Cut);
    pasteAct      ->setShortcut(QKeySequence::Paste);
    duplicateAct  ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    deleteAct     ->setShortcut(QKeySequence::Delete);
    selectallAct  ->setShortcut(QKeySequence::SelectAll);
    rotateAct     ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    hmirrorAct    ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    vmirrorAct    ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    groupAct      ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    ungroupAct    ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U));
    changeSideAct ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));
}

void MainWindow::CreateMenuBar() {
    QMenuBar *menuBar = this->menuBar();

	{ //File
		QMenu *menu = menuBar->addMenu(_("&File"));
        menu->addAction(newAct);
        menu->addAction(openAct);
        menu->addAction(saveAct);
        menu->addAction(saveasAct);
		menu->addSeparator();
        menu->addAction(saveMacroAct);
		menu->addSeparator();
        menu->addAction(autosaveAct);
		menu->addSeparator();
        menu->addAction(gerberImportAct);
		menu->addSeparator();
		{ //File->Export
            QMenu *submenu = menu->addMenu(_("&Export..."));

            submenu->addAction(gerberExportAct);
            submenu->addAction(drillDataAct);
			submenu->addSeparator();
            submenu->addAction(isolationAct);
			submenu->addSeparator();
            submenu->addAction(saveBmpAct);
            submenu->addAction(saveJpgAct);
            submenu->addAction(saveGifAct);
            submenu->addAction(saveEmpAct);
            submenu->addAction(savePngAct);
		}
		menu->addSeparator();
        menu->addAction(directoriesAct);
		menu->addSeparator();
        menu->addAction(printSetupAct);
        menu->addAction(printAct);
		menu->addSeparator();
        menu->addAction(exitAct);
	}
	{ //Edit
		QMenu *menu = menuBar->addMenu(_("&Edit"));

        menu->addAction(undoAct);
        menu->addAction(redoAct);
		menu->addSeparator();
        menu->addAction(copyAct);
        menu->addAction(cutAct);
        menu->addAction(pasteAct);
        menu->addAction(duplicateAct);
        menu->addAction(deleteAct);
		menu->addSeparator();
        menu->addAction(selectallAct);
	}
	{ //Board
		QMenu *menu = menuBar->addMenu(_("&Board"));

        menu->addAction(boardNewAct);
        menu->addAction(boardPropAct);
        menu->addAction(boardCopyAct);
        menu->addAction(boardDeleteAct);
		menu->addSeparator();
        menu->addAction(boardSetRightAct);
        menu->addAction(boardSetLeftAct);
		menu->addSeparator();
        menu->addAction(boardMoveRightAct);
        menu->addAction(boardMoveLeftAct);
		menu->addSeparator();
        menu->addAction(boardImportAct);
        menu->addAction(boardSaveAct);
	}
	{ //Functions
		QMenu *menu = menuBar->addMenu(_("F&unctions"));

        menu->addAction(rotateAct);
		menu->addSeparator();
        menu->addAction(hmirrorAct);
        menu->addAction(vmirrorAct);
		menu->addSeparator();
        menu->addAction(groupAct);
        menu->addAction(ungroupAct);
		menu->addSeparator();
        menu->addAction(changeSideAct);
		{ //Functions->Set to layer
			QMenu *submenu = menu->addMenu(_("&Set to layer..."));

            submenu->addAction(layerC1Act);
            submenu->addAction(layerS1Act);
            submenu->addAction(layerC2Act);
            submenu->addAction(layerS2Act);
            submenu->addAction(layerI1Act);
            submenu->addAction(layerI2Act);
            submenu->addAction(layerOAct);
		}
		menu->addSeparator();
        menu->addAction(snapGridAct);
        menu->addAction(massiveAct);
	}
	{ //Extras
		QMenu *menu = menuBar->addMenu(_("E&xtras"));

        menu->addAction(projectInfoAct);
        menu->addAction(listDrillingsAct);
		menu->addSeparator();
        menu->addAction(bitmapAct);
		menu->addSeparator();
        menu->addAction(footprintAct);
		menu->addSeparator();
        menu->addAction(resetMaskAct);
        menu->addAction(fillZoneAct);
        menu->addAction(removeConAct);
        menu->addAction(deleteOutsideAct);
		menu->addSeparator();
        menu->addAction(elementImportAct);
        menu->addAction(elementExportAct);
	}
	{ //Options
		QMenu *menu = menuBar->addMenu(_("&Options"));
        menu->addAction(settingsAct);
		menu->addSeparator();
        menu->addAction(panelMacroAct);
        menu->addAction(panelPropertiesAct);
        menu->addAction(panelDrcAct);
        menu->addAction(panelComponentsAct);
        menu->addAction(panelSelectorAct);
		menu->addSeparator();
        menu->addAction(zoomBoardAct);
        menu->addAction(zoomObjectsAct);
        menu->addAction(zoomSelectionAct);
	}
	{ //Help
		QMenu *menu = menuBar->addMenu(_("&Help"));
        menu->addAction(aboutAct);
	}
}

void MainWindow::ImportGerberFile(const QString &path) {
    std::ifstream in(path.toLocal8Bit().constData());
    if(!in) {
        QMessageBox::warning(this, _("Import Gerber"), _("Could not open the file."));
        return;
    }
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    Board *b = pcb.GetSelectedBoard();
    uint8_t layer = b->GetSelectedLayer();
    float gd = settings.groundDistance;
    float height = b->GetSize().y;
    PushUndo();

    std::map<int, std::pair<char, Vec2>> apertures;  // dcode -> (type, size)
    int curAp = -1;
    double lastX = 0.0, lastY = 0.0;
    std::vector<Vec2> poly;
    bool inRegion = false, building = false;

    auto toBoard = [&](double rx, double ry){ return Vec2((float)(rx / 1e6), (float)(height - ry / 1e6)); };
    auto flushTrack = [&](){
        if(building && poly.size() >= 2 && apertures.count(curAp))
            b->AddObjectEnd(new Track(layer, gd, apertures[curAp].second.x, poly.data(), poly.size()));
        building = false;
        poly.clear();
    };

    size_t i = 0;
    while(i < content.size()) {
        char c = content[i];
        if(c == '%') {
            size_t end = content.find('%', i + 1);
            if(end == std::string::npos) break;
            std::string cmd = content.substr(i + 1, end - i - 1);
            while(!cmd.empty() && (cmd.back() == '*' || cmd.back() == '\n' || cmd.back() == '\r'))
                cmd.pop_back();
            if(cmd.rfind("ADD", 0) == 0) {
                size_t p = 3;
                int dcode = 0;
                while(p < cmd.size() && isdigit(cmd[p])) dcode = dcode * 10 + (cmd[p++] - '0');
                size_t comma = cmd.find(',', p);
                if(p < cmd.size() && comma != std::string::npos) {
                    char type = cmd[p];
                    std::string rest = cmd.substr(comma + 1);
                    if(type == 'C')
                        apertures[dcode] = {'C', Vec2(atof(rest.c_str()), atof(rest.c_str()))};
                    else if(type == 'R') {
                        size_t x = rest.find('X');
                        float w = atof(rest.c_str());
                        float h = (x != std::string::npos) ? atof(rest.c_str() + x + 1) : w;
                        apertures[dcode] = {'R', Vec2(w, h)};
                    }
                }
            }
            i = end + 1;
            continue;
        }
        if(c == '\n' || c == '\r' || c == ' ' || c == '\t') { i++; continue; }
        size_t end = content.find('*', i);
        if(end == std::string::npos) break;
        std::string blk = content.substr(i, end - i);
        i = end + 1;

        if(blk == "G36") { flushTrack(); inRegion = true; poly.clear(); continue; }
        if(blk == "G37") {
            if(poly.size() >= 3)
                b->AddObjectEnd(new Poly(layer, gd, 0.0f, poly.data(), poly.size(), false));
            poly.clear(); inRegion = false; continue;
        }
        if(blk == "M02" || blk == "M00") { flushTrack(); break; }
        if(!blk.empty() && blk[0] == 'G') {            // strip leading G-code
            size_t k = 0;
            while(k < blk.size() && (blk[k] == 'G' || isdigit(blk[k]))) k++;
            if(k >= blk.size()) continue;
            blk = blk.substr(k);
        }
        if(!blk.empty() && blk[0] == 'D' && blk.find('X') == std::string::npos
                        && blk.find('Y') == std::string::npos) {
            int d = atoi(blk.c_str() + 1);
            if(d >= 10) { flushTrack(); curAp = d; }
            continue;
        }

        double x = lastX, y = lastY;
        int dcmd = -1;
        for(size_t p = 0; p < blk.size();) {
            char ch = blk[p];
            if(ch == 'X' || ch == 'Y') {
                size_t s = ++p;
                while(p < blk.size() && (isdigit(blk[p]) || blk[p] == '-' || blk[p] == '+')) p++;
                double v = atof(blk.substr(s, p - s).c_str());
                if(ch == 'X') x = v; else y = v;
            } else if(ch == 'D') {
                dcmd = atoi(blk.c_str() + p + 1);
                break;
            } else p++;
        }
        lastX = x; lastY = y;
        Vec2 pt = toBoard(x, y);
        if(inRegion) {
            poly.push_back(pt);
        } else if(dcmd == 2) {
            flushTrack(); building = true; poly.push_back(pt);
        } else if(dcmd == 1) {
            if(!building) building = true;
            poly.push_back(pt);
        } else if(dcmd == 3 && curAp >= 0 && apertures.count(curAp)) {
            flushTrack();
            std::pair<char, Vec2> ap = apertures[curAp];
            if(ap.first == 'C')
                b->AddObjectEnd(new THTPad(layer, gd, pt, PadSize(ap.second.x, 0.0f), THTPad::S_CIRCLE, false));
            else
                b->AddObjectEnd(new SMDPad(layer, gd, pt, ap.second));
        }
    }
    flushTrack();
    mainCanvas->update();
}

void MainWindow::RebuildBoardTabs() {
    updatingTabs = true;
    while(boardTabs->count())
        boardTabs->removeTab(0);
    for(uint32_t i = 0; i < pcb.Size(); i++)
        boardTabs->addTab(QString("%1 (%2)").arg(pcb[i]->GetName()).arg(i + 1));
    boardTabs->setCurrentIndex(pcb.GetTab());
    updatingTabs = false;
    SyncLayers();
}

void MainWindow::SyncLayers() {
    if(!layerCombo)   // toolbar not built yet (early call from RebuildBoardTabs)
        return;
    Board *b = pcb.GetSelectedBoard();
    updatingLayers = true;
    layerCombo->setCurrentIndex(b->GetSelectedLayer());
    for(int i = 0; i < 7; i++)
        layerVisAct[i]->setChecked(b->IsLayerVisible(i));
    updatingLayers = false;
}

void MainWindow::PushUndo() {
    ClearRedo();
    undoStack.push_back(new Board(*pcb.GetSelectedBoard()));
    if((int)undoStack.size() > MAX_UNDO_DEPTH) {
        delete undoStack.front();
        undoStack.erase(undoStack.begin());
    }
}

void MainWindow::Undo() {
    if(undoStack.empty())
        return;
    redoStack.push_back(pcb.GetSelectedBoard());
    Board *prev = undoStack.back();
    undoStack.pop_back();
    pcb.SetSelectedBoard(prev);
    mainCanvas->SetBoard(prev);
}

void MainWindow::Redo() {
    if(redoStack.empty())
        return;
    undoStack.push_back(pcb.GetSelectedBoard());
    Board *next = redoStack.back();
    redoStack.pop_back();
    pcb.SetSelectedBoard(next);
    mainCanvas->SetBoard(next);
}

void MainWindow::ClearRedo() {
    for(Board *b : redoStack)
        delete b;
    redoStack.clear();
}

void MainWindow::ClearHistory() {
    for(Board *b : undoStack)
        delete b;
    undoStack.clear();
    ClearRedo();
}

MainWindow::~MainWindow() {
    ClearHistory();
    delete printer;
}
