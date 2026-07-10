#pragma once
#include <QMainWindow>
#include <QtWidgets>

#include <vector>

class QPrinter;

#include "ToolPanel.h"
#include "GridPanel.h"
#include "PCB.h"
#include "Settings.h"
#include "MainCanvas.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void CreateActions();
    void CreateMenuBar();
    void CreateToolBar();
    void CreateLeftPanel();

    void NewFile();
    void OpenFile();
    void SaveFile();
    void SaveFileAs();
    bool SaveToPath(const QString &path);

    void RebuildBoardTabs();   // resync the board tab bar with the PCB
    void SyncLayers();         // sync the layer toolbar with the active board
    void ImportGerberFile(const QString &path);  // parse RS-274X into objects

    // Snapshot-based undo/redo: each entry is a deep clone of the active board.
    void PushUndo();        // call before a mutating operation
    void Undo();
    void Redo();
    void ClearRedo();
    void ClearHistory();    // drop all snapshots (e.g. on New/Open)

    QString currentFile;
    std::vector<Board*> undoStack;
    std::vector<Board*> redoStack;

    QTabBar *boardTabs;
    bool updatingTabs = false;

    QComboBox *layerCombo = nullptr;
    QAction *layerVisAct[7];
    bool updatingLayers = false;

    QPrinter *printer = nullptr;
    QDockWidget *propsDock = nullptr;
    QDockWidget *selectorDock = nullptr;
    QListWidget *selectorList = nullptr;
    std::vector<Object*> selectorItems;

    Settings settings;

    QToolBar *toolBarMain, *toolBarHistory, *toolBarClipboard,
             *toolBarActions, *toolBarZoom, *toolBarPCB, *toolBarPanels;
    ToolPanel *toolPanel;
    GridPanel *gridPanel;
    MainCanvas *mainCanvas;

    QAction *aboutAct, *alignAct, *alignBottomAct, *alignHAct, *alignLeftAct,
            *alignRightAct, *alignTopAct, *alignVAct, *autosaveAct, *bitmapAct,
            *boardCopyAct, *boardDeleteAct, *boardImportAct, *boardMoveLeftAct,
            *boardMoveRightAct, *boardNewAct, *boardPropAct, *boardSaveAct,
            *boardSetLeftAct, *boardSetRightAct, *changeSideAct, *copyAct,
            *cutAct, *deleteAct, *deleteOutsideAct, *directoriesAct, *drillDataAct,
            *duplicateAct, *elementExportAct, *elementImportAct, *exitAct,
            *fillZoneAct, *footprintAct, *gerberExportAct, *gerberImportAct, *groupAct,
            *hmirrorAct, *isolationAct, *layerC1Act, *layerC2Act, *layerI1Act,
            *layerI2Act, *layerOAct, *layerS1Act, *layerS2Act, *listDrillingsAct,
            *massiveAct, *newAct, *openAct, *panelComponentsAct, *panelDrcAct,
            *panelMacroAct, *panelPropertiesAct, *panelSelectorAct, *pasteAct,
            *printAct, *printSetupAct, *projectInfoAct, *redoAct, *removeConAct,
            *resetMaskAct, *rotate15Act, *rotate45Act, *rotate5Act, *rotate90Act,
            *rotateAct, *rotateCustomAct, *saveAct, *saveBmpAct, *saveEmpAct,
            *saveGifAct, *saveJpgAct, *saveMacroAct, *savePngAct, *saveasAct,
            *selectallAct, *settingsAct, *snapGridAct, *transparentAct, *undoAct,
            *ungroupAct, *vmirrorAct, *zoomAct, *zoomBoardAct, *zoomObjectsAct,
            *zoomPreviousAct, *zoomSelectionAct;
    
    PCB pcb;
};
