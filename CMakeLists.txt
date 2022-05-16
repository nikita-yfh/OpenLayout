project(OpenLayout)

find_package(wxWidgets REQUIRED core base gl)
find_package(OpenGL REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(xpm)
set(SOURCES
	src/App.cpp
	src/Arc.cpp
	src/Board.cpp
	src/BottomPanel.cpp
	src/ColorScheme.cpp
	src/ComponentPanel.cpp
	src/File.cpp
	src/GridBinderDialog.cpp
	src/InputGridDialog.cpp
	src/ImageConfig.cpp
	src/MacroPanel.cpp
	src/MacroPreview.cpp
	src/MainCanvas.cpp
	src/MenuBar.cpp
	src/NewBoardDialog.cpp
	src/Object.cpp
	src/OpenLayoutMain.cpp
	src/Pad.cpp
	src/PCB.cpp
	src/Poly.cpp
	src/PolygonBase.cpp
	src/ProjectInfo.cpp
	src/LeftPanel.cpp
	src/SelectorPanel.cpp
	src/Settings.cpp
	src/SettingsDialog.cpp
	src/SMDPad.cpp
	src/THTPad.cpp
	src/ToolBar.cpp
	src/Track.cpp
)
set(HEADERS
	src/AABB.h
	src/App.h
	src/Arc.h
	src/Array.h
	src/Board.h
	src/BottomPanel.h
	src/ColorScheme.h
	src/ComponentPanel.h
	src/File.h
	src/GridBinderDialog.h
	src/InputGridDialog.h
	src/ImageConfig.h
	src/LineObject.h
	src/MacroPanel.h
	src/MacroPreview.h
	src/MainCanvas.h
	src/MenuBar.h
	src/NewBoardDialog.h
	src/Object.h
	src/OpenLayoutMain.h
	src/Pad.h
	src/PCB.h
	src/PadSize.h
	src/PolygonBase.h
	src/Poly.h
	src/ProjectInfo.h
	src/LeftPanel.h
	src/SelectorPanel.h
	src/Settings.h
	src/SettingsDialog.h
	src/SMDPad.h
	src/THTPad.h
	src/ToolBar.h
	src/Track.h
	src/Vec2.h
)
if(WIN32)
    set(SOURCES ${SOURCES} src/resource.rc)
endif()
add_executable(OpenLayout ${SOURCES} ${HEADERS})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -ffunction-sections -fdata-sections -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS "-Wno-write-strings")
endif()
target_link_libraries(OpenLayout PRIVATE ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GL_LIBRARIES})

install(TARGETS OpenLayout DESTINATION bin)
