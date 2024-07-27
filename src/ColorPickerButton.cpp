#include "ColorPickerButton.h"

#include <QColorDialog>

const int COLOR_BUTTON_SIZE = 24;

ColorPickerButton::ColorPickerButton(const QColor &color, QWidget *parent) : QPushButton(parent) {
    SetColor(color);
    setIconSize(QSize(COLOR_BUTTON_SIZE, COLOR_BUTTON_SIZE));
    connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

void ColorPickerButton::SetColor(const QColor &color) {
    this->color = color;
    QPixmap pixmap(COLOR_BUTTON_SIZE, COLOR_BUTTON_SIZE);
    pixmap.fill(color);
    setIcon(QIcon(pixmap));
}

void ColorPickerButton::OnClicked() {
    QColorDialog dialog(this);
    dialog.setOptions(QColorDialog::DontUseNativeDialog);
    dialog.setCurrentColor(color);
    dialog.exec();
    SetColor(dialog.currentColor());
    emit ColorChanged(color);
}

