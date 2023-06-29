cmake_minimum_required(VERSION 3.10)

project(Video)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(FFMPEG REQUIRED COMPONENTS AVCODEC AVFORMAT AVUTIL SWRESAMPLE)

if (NOT FFMPEG_FOUND)
    message(ERROR "Cannot find ffmpeg")
else ()
    message(STATUS "Found ffmpeg")
    message(STATUS "ffmpeg include dir is ${FFMPEG_INCLUDE_DIRS}")
    message(STATUS "ffmpeg libraries are ${FFMPEG_LIBRARIES}")
endif ()

find_package(OpenALSoft)
if (NOT OpenALSoft_FOUND)
    message(ERROR "Cannot find OpenALSoft")
else ()
    message(STATUS "Found OpenALSoft")
endif ()


find_package(SndFile)

if (NOT SNDFILE_FOUND)
    message(ERROR "Cannot find SndFile")
else ()
    message(STATUS "Found SndFile")
endif ()

include_directories(./src)

add_library(ffmpeg_dep INTERFACE)
target_link_libraries(ffmpeg_dep INTERFACE ${FFMPEG_LIBRARIES})
target_include_directories(ffmpeg_dep INTERFACE ${FFMPEG_INCLUDE_DIRS})

add_library(OpenALSoft INTERFACE)
target_link_libraries(OpenALSoft INTERFACE ${OPENALSOFT_LIBRARY})
target_include_directories(OpenALSoft INTERFACE ${OPENALSOFT_INCLUDE_DIR})

#add_library(SndFile INTERFACE)
#target_link_libraries(SndFile INTERFACE ${SNDFILE_LIBRARY})
#target_include_directories(SndFile INTERFACE ${SNDFILE_INCLUDE_DIR})

#mark_as_advanced(OPENALSOFT_INCLUDE_DIR OPENALSOFT_LIBRARY)


add_executable(decode_audio main.cpp)
target_link_libraries(decode_audio ffmpeg_dep OpenALSoft )

