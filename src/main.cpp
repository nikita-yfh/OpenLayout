#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {

    // QOpenGLWidget оставляет на нативном Wayland остаточные изображения
    // (ghosting). XWayland (xcb) рисует корректно — выбираем его по умолчанию,
    // но не перетираем явно заданную пользователем платформу.
    if(!qEnvironmentVariableIsSet("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "xcb");

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    MainWindow window;
    window.show();
    return a.exec();
}

