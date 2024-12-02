cmake_minimum_required(VERSION 3.14)

project(UNIT_TESTS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

# Find modules Qt6
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Multimedia REQUIRED)
find_package(Qt6MultimediaWidgets REQUIRED)

# rapidjson
include_directories(../libs/rapidjson/include)

add_executable(UNIT_TESTS main.cpp Test.cpp Test.h)

target_link_libraries(UNIT_TESTS PRIVATE 
	LIB_KEENCLOO
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ${FFMPEG_LIB_AVCODEC}
    ${FFMPEG_LIB_AVDEVICE}
    ${FFMPEG_LIB_AVFILTER}
    ${FFMPEG_LIB_AVFORMAT}
    ${FFMPEG_LIB_AVUTIL}
    ${FFMPEG_LIB_POSTPROC}
    ${FFMPEG_LIB_SWRESAMPLE}
    ${FFMPEG_LIB_SWSCALE}
)

set_target_properties(UNIT_TESTS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/$<CONFIG>"
)