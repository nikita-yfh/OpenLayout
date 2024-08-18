#include "MainWindow.h"
#include "Locale.h"

#include "SettingsDialog.h"
#include "MainCanvas.h"

#include <QIcon>
#include <QAction>
#include <QVBoxLayout>

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
    pcb.Load(file);

    CreateActions();
    CreateToolBar();
    CreateLeftPanel();
    CreateMenuBar();

    MainCanvas *canvas = new MainCanvas(pcb.GetSelectedBoard(), settings, this);
    setCentralWidget(canvas);
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

MainWindow::~MainWindow() {
}
