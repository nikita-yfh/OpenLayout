project(OpenLayout)

find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})
set(SOURCES
	Board.cpp
	LayerInfoDialog.cpp
	AboutDialog.cpp
	NewBoardDialog.cpp
	OpenLayoutApp.cpp
	OpenLayoutMain.cpp
	PCBFile.cpp
	ProjectInfoDialog.cpp
	Settings.cpp
	SettingsDialog.cpp
)
set(HEADERS
	Board.h
	LayerInfoDialog.h
	AboutDialog.h
	NewBoardDialog.h
	OpenLayoutApp.h
	OpenLayoutMain.h
	PCBFile.h
	ProjectInfoDialog.h
	Settings.h
	SettingsDialog.h
)
add_custom_target(images ALL
	COMMAND ${PYTHON_EXECUTABLE} /bin/bash genimages.sh
	BYPRODUCTS images.h
	COMMENT "Generating images.h file"
)
add_executable(OpenLayout ${SOURCES} ${HEADERS})
add_dependencies(OpenLayout images)
target_link_libraries(OpenLayout PRIVATE ${wxWidgets_LIBRARIES})
