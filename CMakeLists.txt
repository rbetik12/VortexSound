cmake_minimum_required(VERSION 3.17)
project(vortex_sound C)

if (UNIX)
    set(PLATFORM Linux)
elseif (WIN32)
    set(PLATFORM Windows)
endif()

set(CMAKE_C_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE}-${PLATFORM}/${CMAKE_PROJECT_NAME})

add_definitions(-DAL_LIBTYPE_STATIC)

include_directories(include
        lib/openal/include
        lib/minimp3
        )

add_subdirectory(lib/openal)

add_library(vortex_sound src/vx-sound.c include/vx-sound.h example/example.c)

target_link_libraries(vortex_sound OpenAL)

project(example C)

add_executable(example example/example.c)

target_link_libraries(example vortex_sound)