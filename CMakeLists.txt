cmake_minimum_required( VERSION 2.8.12 )
set(CMAKE_BUILD_TYPE Release)
project(vinace)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)
find_package(Threads REQUIRED)

find_path(PULSEAUDIO_INCLUDE_DIR
        NAMES pulse/pulseaudio.h
        DOC "The PulseAudio include directory"
        )
find_library(PULSEAUDIO_LIBRARY
        NAMES pulse
        DOC "The PulseAudio library"
        )

include_directories(${PULSEAUDIO_INCLUDE_DIRS})

add_subdirectory(src)


