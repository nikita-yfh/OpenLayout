#pragma once
#include <QPushButton>
#include <QColor>

class ColorPickerButton : public QPushButton {
    Q_OBJECT
public:
    ColorPickerButton(const QColor &color, QWidget *parent);
    ~ColorPickerButton() {};

    void SetColor(const QColor &color);
    const QColor &GetColor() {
        return color;
    }
private:
    QColor color;
signals:
    void ColorChanged(const QColor &color);
private slots:
    void OnClicked();
};
