project(openlayout)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

set(SOURCES
    src/MainWindow.cpp
    src/ToolPanel.cpp
    src/GridPanel.cpp
    src/SettingsDialog.cpp
    src/Settings.cpp
    src/File.cpp
    src/ColorScheme.cpp
    src/main.cpp
)
set(HEADERS
    src/MainWindow.h
    src/ToolPanel.h
    src/GridPanel.h
    src/SettingsDialog.h
    src/Settings.h
    src/File.h
    src/Vec2.h
    src/PadSize.h
    src/ColorScheme.h
    src/Locale.h
)

add_executable(openlayout ${SOURCES} ${HEADERS})

qt5_use_modules(openlayout Widgets)
