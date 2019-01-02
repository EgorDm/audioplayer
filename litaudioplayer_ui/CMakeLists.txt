project(litaudioplayer_ui C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Portaudio REQUIRED)

select_source_files()
select_ui_source_files()

include_directories(${litaudioplayer_INCLUDE_DIRS})
include_directories(${litwidgets_INCLUDE_DIRS})
include_directories(${PORTAUDIO_INCLUDE_DIRS})

set(USED_LIBRARIES
        Qt5::Widgets
        Qt5::Multimedia
        ${PORTAUDIO_LIBRARIES}
        litaudioplayer
        litwidgets
        )

message("Adding executable: ${PROJECT_NAME}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCE_UI} ${SOURCE_RESOURCE})

target_link_libraries(
        ${PROJECT_NAME}
        ${USED_LIBRARIES}
)