#
# Project definition
#

project("DNA" C)
cmake_minimum_required(VERSION 3.3)

#
# Global definitions
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/Headers/Public)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/Headers/Public)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mm/Headers/Public)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tools/Headers/Public)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vfs/Headers/Public)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hal/soclib/platform/Headers/Public)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hal/soclib/processors/arm/v6/Headers/Public)

#
# Flags preferences
#

set(CMAKE_C_COMPILER /usr/local/bin/arm-none-eabi-gcc)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -Werror -march=armv6")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O3")

#
# Subdirectories
#

add_subdirectory(core)
add_subdirectory(lib)
add_subdirectory(mm)
add_subdirectory(tools)
add_subdirectory(vfs)
add_subdirectory(hal/soclib)
