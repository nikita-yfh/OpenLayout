project(openlayout)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)

set(SOURCES
    src/MainWindow.cpp
    src/main.cpp
)
set(HEADERS
    src/MainWindow.h
    src/Locale.h
)

add_executable(openlayout ${SOURCES} ${HEADERS})

qt5_use_modules(openlayout Widgets)
