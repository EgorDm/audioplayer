project(litwidgets C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

select_source_files()
select_ui_source_files()

include_directories(${litsignal_INCLUDE_DIRS})
include_directories(${litaudioplayer_INCLUDE_DIRS})

set(USED_LIBRARIES
        litsignal
        litaudioplayer)

init_qt_library(${PROJECT_NAME} "${USED_LIBRARIES}")