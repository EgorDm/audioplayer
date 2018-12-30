project(litaudioplayer C CXX)

select_source_files()

include_directories(${litaudiofile_INCLUDE_DIRS})
include_directories(${litaudio_INCLUDE_DIRS})

set(USED_LIBRARIES
        litaudio
        litaudiofile)

init_library(${PROJECT_NAME} "${USED_LIBRARIES}")