#pragma once
#include <QPushButton>
#include <QColor>

class ColorPickerButton : public QPushButton {
    Q_OBJECT
public:
    ColorPickerButton(const QColor &color, QWidget *parent);
    ~ColorPickerButton() {};

    void setColor(const QColor &color);
private:
    QColor color;
signals:
    void colorChanged(const QColor &color);
private slots:
    void onClicked();
};
