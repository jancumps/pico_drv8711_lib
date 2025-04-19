cmake_minimum_required(VERSION 3.28)

include(pico_sdk_import.cmake)

project(pico_drv8711 C CXX ASM)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts -fcommon")

pico_sdk_init()

include(FetchContent)

FetchContent_Declare(drv8711
  GIT_REPOSITORY "https://github.com/jancumps/drv8711_lib.git"
  GIT_TAG "origin/main"
  # prevent makefile execution (if any)
  SOURCE_SUBDIR =
)
FetchContent_MakeAvailable(drv8711)

add_library(pico_drv8711)
target_sources(pico_drv8711
        PUBLIC
)
target_sources(pico_drv8711
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ${drv8711_SOURCE_DIR}/source/drv8711.cpp
        ${drv8711_SOURCE_DIR}/source/drv8711_config.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/source/drv8711_pico.cpp
)

target_link_libraries( pico_drv8711
        hardware_spi
        hardware_gpio
        drv8711
)

