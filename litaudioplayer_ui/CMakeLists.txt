project(litaudioplayer_ui C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Portaudio REQUIRED)

select_source_files()
file(GLOB_RECURSE SOURCE_UI
        "*.ui"
        )
qt5_wrap_ui(UI_GENERATED_HEADERS ${SOURCE_UI})

include_directories(${litaudioplayer_INCLUDE_DIRS})
include_directories(${PORTAUDIO_INCLUDE_DIRS})

set(USED_LIBRARIES
        litaudioplayer
        Qt5::Widgets
        Qt5::Multimedia
        ${PORTAUDIO_LIBRARIES})

message("Adding executable: ${PROJECT_NAME}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_GENERATED_HEADERS})

target_link_libraries(
        ${PROJECT_NAME}
        ${USED_LIBRARIES}
)