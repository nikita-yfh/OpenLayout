project(openlayout)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCES
    src/Board.cpp
    src/Circle.cpp
    src/ColorPickerButton.cpp
    src/ColorScheme.cpp
    src/DirChooserButton.cpp
    src/File.cpp
    src/GLUtils.cpp
    src/GridPanel.cpp
    src/ImageConfig.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/Object.cpp
    src/ObjectGroup.cpp
    src/Pad.cpp
    src/PCB.cpp
    src/Poly.cpp
    src/PolygonBase.cpp
    src/ProjectInfo.cpp
    src/Settings.cpp
    src/SettingsDialog.cpp
    src/SMDPad.cpp
    src/THTPad.cpp
    src/ToolPanel.cpp
    src/Track.cpp
)
set(HEADERS
    src/AABB.h
    src/Array.h
    src/Board.h
    src/Circle.h
    src/ColorPickerButton.h
    src/ColorScheme.h
    src/DirChooserButton.h
    src/File.h
    src/GLUtils.h
    src/GridPanel.h
    src/ImageConfig.h
    src/LineObject.h
    src/Locale.h
    src/MainWindow.h
    src/ObjectGroup.h
    src/Object.h
    src/Pad.h
    src/PCB.h
    src/PadSize.h
    src/PolygonBase.h
    src/Poly.h
    src/ProjectInfo.h
    src/SettingsDialog.h
    src/Settings.h
    src/SMDPad.h
    src/THTPad.h
    src/ToolPanel.h
    src/Track.h
    src/Vec2.h
)

add_executable(openlayout ${SOURCES} ${HEADERS})

target_link_libraries(openlayout ${OPENGL_LIBRARIES})

qt5_use_modules(openlayout Widgets OpenGL)
