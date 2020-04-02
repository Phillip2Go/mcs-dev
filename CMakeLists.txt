cmake_minimum_required(VERSION 3.15)
project(mcs-dev)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp libs/tinyxml2/tinyxml2.cpp src/server/server.h src/server/server.cpp src/cameracontroller/cameracontroller.h src/cameracontroller/cameracontroller.cpp src/rootstream/rootstream.h src/rootstream/rootstream.cpp src/data/structs.h src/clientstream/clientstream.h src/clientstream/clientstream.cpp)

# function to retrieve files sources inside a list of folders
function(find_sources DIRS)
    set(_headers "")
    set(_srcs "")
    foreach (dir ${ARGV})
        file (GLOB_RECURSE h_${dir} "${dir}/*.h" "${dir}/*.ipp" "${dir}/*.hpp")
        file (GLOB_RECURSE s_${dir} "${dir}/*.cpp" "${dir}/*.c" "${dir}/*.cxx")
        source_group (${dir} FILES ${s_${dir}} ${h_${dir}})
        set (_srcs ${_srcs} ${s_${dir}})
        set (_headers ${_headers} ${h_${dir}})
    endforeach ()
    set (SOURCES ${_srcs} PARENT_SCOPE)
    set (HEADERS ${_headers} PARENT_SCOPE)
endfunction()

# Cmake properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(PREDEFINED_TARGETS_FOLDER "CMake")
set(CMAKE_VERBOSE_MAKEFILE true)

# include find package and try to find required packages
include(FindPkgConfig)
pkg_check_modules(GLIB2 glib-2.0)

# Check build type
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No CMAKE_BUILD_TYPE specified, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "Building ${CMAKE_BUILD_TYPE} ERTSP")

# compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") #enable C++14

# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Wno-unused-function") # extra warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color") #enable error coloration on gcc

# release specific flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2") #enable error coloration on gcc

#debug specific flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")

# find gstreamer 1.x libraries
pkg_search_module(GSTREAMER REQUIRED gstreamer-1.0)
find_library(LIB_GSTREAMER NAMES ${GSTREAMER_LIBRARIES} HINTS ${GSTREAMER_LIBRARY_DIRS})
FIND_LIBRARY(GST_RTSP_SERVER_LIBRARY_DIRS NAMES gstrtspserver-1.0 PATHS "/usr/lib/x86_64-linux-gnu/gstreamer-1.0/")
FIND_PATH(GST_RTSP_SERVER_INCLUDE_DIRS gst/rtsp-server/rtsp-server.h PATHS "/usr/include/gstreamer-1.0")
pkg_search_module(GSTREAMER_APP REQUIRED gstreamer-app-1.0) # for appsrc

include_directories (
        "${PROJECT_SOURCE_DIR}/include"
        "${GSTREAMER_INCLUDE_DIRS}"
        "${GST_RTSP_SERVER_INCLUDE_DIRS}"
        "${GSTREAMER_APP_INCLUDE_DIRS}"
)

link_directories (
        "${GSTREAMER_LIBRARY_DIRS}"
        "${GST_RTSP_SERVER_LIBRARY_DIRS}"
        "${GSTREAMER_APP_LIBRARY_DIRS}"
)
find_sources("src" "include")

#
# OpenCV Setup
SET("OpenCV_DIR" "/usr/local/Cellar/opencv/4.2.0_1/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# NDI
find_library(
        NDI
        libndi.4.dylib
        PATHS
        /libs)

#
# First include
add_executable(mcs-dev ${SOURCE_FILES} ${HEADERS} ${SOURCES})
target_link_libraries(mcs-dev ${NDI} ${OpenCV_LIBS} ${GSTREAMER_LIBRARIES} ${GST_RTSP_SERVER_LIBRARY_DIRS} ${GSTREAMER_APP_LIBRARIES})
include(${CMAKE_ROOT}/Modules/FindPkgConfig.cmake)

# Gstreamer Setup
# Set CMAKE_C_FLAGS variable with info from pkg-util
execute_process(COMMAND pkg-config --cflags gstreamer-1.0
        OUTPUT_VARIABLE CMAKE_C_FLAGS)
string(REPLACE "\n" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
# Set CMAKE_EXE_LINK_FLAGS variable with info from pkg-util
execute_process(COMMAND pkg-config --libs gstreamer-1.0
        OUTPUT_VARIABLE CMAKE_EXE_LINK_FLAGS)
string(REPLACE "\n" "" CMAKE_EXE_LINK_FLAGS ${CMAKE_EXE_LINK_FLAGS})
message("CMAKE_EXE_LINK_FLAGS: ${CMAKE_EXE_LINK_FLAGS}")

set_target_properties(mcs-dev
        PROPERTIES COMPILE_FLAGS ${CMAKE_C_FLAGS}
        LINK_FLAGS ${CMAKE_EXE_LINK_FLAGS})

