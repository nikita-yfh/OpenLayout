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
	InputGridDialog.cpp
	Utils.cpp
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
	InputGridDialog.h
	Utils.h
)
add_custom_target(images ALL
	COMMAND /bin/bash ${CMAKE_CURRENT_SOURCE_DIR}/genimages.sh ${CMAKE_CURRENT_SOURCE_DIR}/xpm
	BYPRODUCTS images.h
	COMMENT "Generating images.h file"
)
add_executable(OpenLayout ${SOURCES} ${HEADERS})
add_dependencies(OpenLayout images)
target_link_libraries(OpenLayout PRIVATE ${wxWidgets_LIBRARIES})
