project(litaudioplayer_ui C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

select_source_files()
file(GLOB_RECURSE SOURCE_UI
        "*.ui"
        )

include_directories(${litaudioplayer_INCLUDE_DIRS})

set(USED_LIBRARIES
        litaudioplayer
        Qt5::Widgets)

qt5_wrap_ui(UI_GENERATED_HEADERS ${SOURCE_UI})

message("Adding executable: ${PROJECT_NAME}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${UI_GENERATED_HEADERS})

target_link_libraries(
        ${PROJECT_NAME}
        ${USED_LIBRARIES}
)