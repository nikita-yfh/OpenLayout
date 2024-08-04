#pragma once
#include <QMainWindow>
#include <QtWidgets>

#include "ToolPanel.h"
#include "GridPanel.h"
#include "Settings.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void CreateMenuBar();
    void CreateToolBar();
    void CreateLeftPanel();

    QToolBar *toolBarMain, *toolBarHistory, *toolBarClipboard,
             *toolBarActions, *toolBarZoom, *toolBarPCB, *toolBarPanels;
    ToolPanel *toolPanel;
    GridPanel *gridPanel;

    QAction *newAct, *openAct, *saveAct, *printAct, *undoAct, *redoAct,
            *cutAct, *copyAct, *pasteAct, *deleteAct, *duplicateAct,
            *rotate90Act, *rotate45Act, *rotate15Act, *rotate5Act, *rotateCustomAct,
            *mirrorHAct, *mirrorVAct,
            *alignTopAct, *alignBottomAct, *alignLeftAct, *alignRightAct, *alignHAct, *alignVAct,
            *snapGridAct, *removeConAct, *groupAct, *ungroupAct,
            *zoomPreviousAct, *zoomBoardAct, *zoomObjectsAct, *zoomSelectionAct,
            *transparentAct, *projectInfoAct, *bitmapAct,
            *selectorAct, *componentAct, *propertiesAct, *DRCAct, *macroAct;

    Settings settings;
};
