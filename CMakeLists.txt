project(OpenLayout)

find_package(wxWidgets REQUIRED core base gl)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCES
	src/ScannedCopyDialog.cpp
	src/InputNumberDialog.cpp
	src/GridBinderDialog.cpp
	src/LayerInfoDialog.cpp
	src/AboutDialog.cpp
	src/NewBoardDialog.cpp
	src/OpenLayoutApp.cpp
	src/OpenLayoutMain.cpp
	src/PCBFile.cpp
	src/ProjectInfoDialog.cpp
	src/Settings.cpp
	src/SettingsDialog.cpp
	src/InputGridDialog.cpp
	src/Utils.cpp
	src/GLCanvas.cpp
	src/Canvas.cpp
	src/LeftPanel.cpp
	src/BottomPanel.cpp
)
set(HEADERS
	src/ScannedCopyDialog.h
	src/InputNumberDialog.h
	src/GridBinderDialog.h
	src/LayerInfoDialog.h
	src/AboutDialog.h
	src/NewBoardDialog.h
	src/OpenLayoutApp.h
	src/OpenLayoutMain.h
	src/PCBFile.h
	src/ProjectInfoDialog.h
	src/Settings.h
	src/SettingsDialog.h
	src/InputGridDialog.h
	src/Utils.h
	src/LeftPanel.h
	src/BottomPanel.h
	src/GLCanvas.h
	src/Canvas.h
)
if(WIN32)
    set(SOURCES ${SOURCES} src/resource.rc)
#else()
	#add_custom_target(images ALL
	#	COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/genimages.sh ${CMAKE_CURRENT_SOURCE_DIR}/xpm
	#	BYPRODUCTS images.h
	#	COMMENT "Generating images.h file"
	#)
endif()
add_executable(OpenLayout ${SOURCES} ${HEADERS})

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -ffunction-sections -fdata-sections")
#add_dependencies(OpenLayout images)
target_link_libraries(OpenLayout PRIVATE ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GL_LIBRARIES})
