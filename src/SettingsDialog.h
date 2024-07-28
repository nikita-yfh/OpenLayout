#pragma once
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

#include "Settings.h"
#include "ColorPickerButton.h"
#include "DirChooserButton.h"

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(const Settings &oldSettings, QWidget *parent);
    ~SettingsDialog() {}
private:
    ColorPickerButton *colorPickerButtons[COLOR_COUNT];
    QPushButton *resetColorsButton;
    QLineEdit *macroPath;

    Settings settings;
public slots:
    void OnUnitsChanged(int index);
    void OnDrillChanged(int index);

    void OnCheckboxToggled(int index);

    void OnColorSchemeChanged(int index);
    void OnResetColorScheme();

    void OnSameDirToggled(bool state);

    void OnMacroDirChange();
    void OnMacroDirReset();
    void OnMacroDirOpenFM();

    void OnUndoDepthChanged(int depth);
};
