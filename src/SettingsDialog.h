#pragma once
#include <QDialog>

#include "Settings.h"

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(const Settings &oldSettings, QWidget *parent);
    ~SettingsDialog() {}
private:
    Settings settings;
public slots:
};
