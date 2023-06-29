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


# 包含头文件
# glad项目已经加入进来了
include_directories(./glad/include)
include_directories(./includes)

include_directories(/opt/homebrew/Cellar/glfw/3.3.4/include)
link_directories(/opt/homebrew/Cellar/glfw/3.3.4/lib)
# /opt/homebrew/Cellar/glm/0.9.9.8/include


include_directories(/opt/homebrew/Cellar/glm/0.9.9.8/include)
link_directories(/opt/homebrew/Cellar/glm/0.9.9.8/lib)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# include_directories(/opt/homebrew/Cellar/freetype/2.11.0/include)
link_directories(/opt/homebrew/Cellar/freetype/2.11.0/lib)

include_directories(/opt/homebrew/Cellar/glew/2.2.0_1/include)
# link_directories(/opt/homebrew/Cellar/glew/2.2.0_1/lib)


add_executable(decode_audio
        ./src/stb_image.cpp
        ./glad/src/glad.c
        ./main.cpp)

target_link_libraries(decode_audio
        ffmpeg_dep
        OpenALSoft
        glfw
        freetype
        )

