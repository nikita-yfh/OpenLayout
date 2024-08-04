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
    void CreateActions();
    void CreateMenuBar();
    void CreateToolBar();
    void CreateLeftPanel();

    QToolBar *toolBarMain, *toolBarHistory, *toolBarClipboard,
             *toolBarActions, *toolBarZoom, *toolBarPCB, *toolBarPanels;
    ToolPanel *toolPanel;
    GridPanel *gridPanel;

    QAction *boardNewAct, *openAct, *saveAct, *printAct, *undoAct, *redoAct,
            *cutAct, *copyAct, *pasteAct, *deleteAct, *duplicateAct,
            *rotate90Act, *rotate45Act, *rotate15Act, *rotate5Act, *rotateCustomAct,
            *hmirrorAct, *vmirrorAct,
            *alignTopAct, *alignBottomAct, *alignLeftAct, *alignRightAct, *alignHAct, *alignVAct,
            *snapGridAct, *removeConAct, *groupAct, *ungroupAct,
            *zoomPreviousAct, *zoomBoardAct, *zoomObjectsAct, *zoomSelectionAct,
            *transparentAct, *projectInfoAct, *bitmapAct;
    QAction *newAct, *saveasAct, *saveMacroAct, *autosaveAct,
            *gerberImportAct, *gerberExportAct, *drillDataAct, *isolationAct,
            *saveBmpAct, *saveJpgAct, *saveGifAct, *saveEmpAct, *savePngAct,
            *directoriesAct, *printSetupAct, *exitAct,
            *selectallAct, *boardCopyAct, *rotateAct,
            *boardPropAct, *boardDeleteAct, *boardSetRightAct, *boardSetLeftAct, *boardMoveRightAct,
            *boardMoveLeftAct, *boardImportAct, *boardSaveAct,
            *changeSideAct,
            *layerC1Act, *layerS1Act, *layerC2Act, *layerS2Act, *layerI1Act, *layerI2Act, *layerOAct,
            *massiveAct, *listDrillingsAct,
            *footprintAct, *resetMaskAct, *deleteOutsideAct, *elementExportAct,
            *panelMacroAct, *panelPropertiesAct, *panelDrcAct, *panelComponentsAct, *panelSelectorAct,
            *elementImportAct, *settingsAct, *aboutAct;
};
