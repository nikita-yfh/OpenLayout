project(OpenLayout)

find_package(wxWidgets REQUIRED core base gl)
find_package(OpenGL REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(xpm)
set(SOURCES
	src/App.cpp
	src/Board.cpp
	src/ImageConfig.cpp
	src/PCB.cpp
	src/THTPad.cpp
	src/SMDPad.cpp
	src/ProjectInfo.cpp
	src/File.cpp
	src/ColorScheme.cpp
	src/Settings.cpp
	src/MenuBar.cpp
	src/ToolBar.cpp
	src/OpenLayoutMain.cpp
	src/NewBoardDialog.cpp
	src/MacroPanel.cpp
	src/MacroPreview.cpp
	src/ComponentPanel.cpp
	src/SelectorPanel.cpp
	src/Object.cpp
)
set(HEADERS
	src/App.h
	src/Board.h
	src/ImageConfig.h
	src/PCB.h
	src/Pad.h
	src/THTPad.h
	src/ProjectInfo.h
	src/File.h
	src/Vec2.h
	src/AABB.h
	src/ColorScheme.h
	src/Settings.h
	src/MenuBar.h
	src/ToolBar.h
	src/OpenLayoutMain.h
	src/NewBoardDialog.h
	src/MacroPanel.h
	src/MacroPreview.h
	src/ComponentPanel.h
	src/SelectorPanel.h
	src/Object.h
	src/Pad.h
	src/THTPad.h
	src/SMDPad.h
)
if(WIN32)
    set(SOURCES ${SOURCES} src/resource.rc)
endif()
add_executable(OpenLayout ${SOURCES} ${HEADERS})
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -ffunction-sections -fdata-sections")
	set(CMAKE_CXX_FLAGS "-Wno-write-strings")
endif()
target_link_libraries(OpenLayout PRIVATE ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GL_LIBRARIES})

install(TARGETS OpenLayout DESTINATION bin)
