project(openlayout)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)
find_package(OpenGL REQUIRED)

include_directories("src" "src/tools" "src/objects")

set(SOURCES
    src/Board.cpp
    src/ColorPickerButton.cpp
    src/ColorScheme.cpp
    src/DirChooserButton.cpp
    src/File.cpp
    src/GLUtils.cpp
    src/GridPanel.cpp
    src/ImageConfig.cpp
    src/MainCanvas.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/ObjectGroup.cpp
    src/PCB.cpp
    src/ProjectInfo.cpp
    src/Settings.cpp
    src/SettingsDialog.cpp
    src/ToolPanel.cpp

    src/objects/Circle.cpp
    src/objects/Object.cpp
    src/objects/Pad.cpp
    src/objects/Poly.cpp
    src/objects/PolygonBase.cpp
    src/objects/SMDPad.cpp
    src/objects/THTPad.cpp
    src/objects/Track.cpp

    src/tools/ConnectionsTool.cpp
    src/tools/EditTool.cpp
    src/tools/PadTool.cpp
    src/tools/Tool.cpp
    src/tools/ZoomTool.cpp
)

set(HEADERS
    src/AABB.h
    src/Array.h
    src/Board.h
    src/ColorPickerButton.h
    src/ColorScheme.h
    src/DirChooserButton.h
    src/File.h
    src/GLUtils.h
    src/GridPanel.h
    src/ImageConfig.h
    src/Locale.h
    src/MainCanvas.h
    src/MainWindow.h
    src/ObjectGroup.h
    src/PadSize.h
    src/PCB.h
    src/ProjectInfo.h
    src/SettingsDialog.h
    src/Settings.h
    src/ToolPanel.h
    src/Utils.h
    src/Vec2.h

    src/objects/Circle.h
    src/objects/LineObject.h
    src/objects/Object.h
    src/objects/Pad.h
    src/objects/PolygonBase.h
    src/objects/Poly.h
    src/objects/SMDPad.h
    src/objects/THTPad.h
    src/objects/Track.h
    src/tools/ConnectionsTool.h

    src/tools/EditTool.h
    src/tools/PadTool.h
    src/tools/RectTool.h
    src/tools/Tool.h
    src/tools/ZoomTool.h
)

add_executable(openlayout ${SOURCES} ${HEADERS})

target_link_libraries(openlayout ${OPENGL_LIBRARIES})

qt5_use_modules(openlayout Widgets OpenGL)
