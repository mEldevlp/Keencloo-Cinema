cmake_minimum_required(VERSION 3.9)

project(Keencloo LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)
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

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Path for FFmpeg
set(FFMPEG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/ffmpeg/include")
set(FFMPEG_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/ffmpeg/lib")
set(FFMPEG_BIN_DIR "${CMAKE_SOURCE_DIR}/libs/ffmpeg/bin")

# Find libs FFmpeg
find_library(FFMPEG_LIB_AVCODEC 	avcodec 	HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_AVDEVICE 	avdevice 	HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_AVFILTER 	avfilter 	HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_AVFORMAT 	avformat 	HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_AVUTIL 		avutil 		HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_POSTPROC 	postproc 	HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_SWRESAMPLE 	swresample	HINTS ${FFMPEG_LIB_DIR})
find_library(FFMPEG_LIB_SWSCALE 	swscale 	HINTS ${FFMPEG_LIB_DIR})

include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})
include_directories(${FFMPEG_INCLUDE_DIR})

# Asssembling sources of project
file(GLOB_RECURSE SOURCES
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
	"${CMAKE_SOURCE_DIR}/styles/*.qss"
)

add_library(LIB_KEENCLOO STATIC ${SOURCES})
target_include_directories(LIB_KEENCLOO PUBLIC ${INCLUDE_DIR})

add_executable(Keencloo WIN32 ${SOURCES})

# Install compile Qt
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

target_include_directories(Keencloo PUBLIC ${INCLUDE_DIR})
target_include_directories(Keencloo PUBLIC ${SOURCE_DIR})

target_link_libraries(LIB_KEENCLOO PRIVATE
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

# Link Qt & FFmpeg
target_link_libraries(Keencloo PRIVATE
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

# Unit tests for Keencloo
add_subdirectory(unit_tests)

# rapidjson
include_directories(libs/rapidjson/include)

# Copy DLL file into dir with execute file (win only)
if(WIN32)
    function(copy_resources target source destination)
        foreach(file ${source})
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${file}"
                "${destination}"
            )
        endforeach()
    endfunction()

    # Create directories
    foreach(dir "$<TARGET_FILE_DIR:Keencloo>/videos"
                 "$<TARGET_FILE_DIR:Keencloo>/rsc"
                 "$<TARGET_FILE_DIR:Keencloo>/rsc/fullscreen"
                 "$<TARGET_FILE_DIR:Keencloo>/styles"
                 "${CMAKE_SOURCE_DIR}/build/rsc"
                 "${CMAKE_SOURCE_DIR}/build/rsc/fullscreen")
				 
        add_custom_command(TARGET Keencloo POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${dir}"
        )
    endforeach()

    # Copy DLLs
    set(FFMPEG_DLLS
        "${FFMPEG_BIN_DIR}/avcodec-61.dll"
        "${FFMPEG_BIN_DIR}/avdevice-61.dll"
        "${FFMPEG_BIN_DIR}/avfilter-10.dll"
        "${FFMPEG_BIN_DIR}/avformat-61.dll"
        "${FFMPEG_BIN_DIR}/avutil-59.dll"
        "${FFMPEG_BIN_DIR}/postproc-58.dll"
        "${FFMPEG_BIN_DIR}/swresample-5.dll"
        "${FFMPEG_BIN_DIR}/swscale-8.dll"
    )
    copy_resources(Keencloo "${FFMPEG_DLLS}" "$<TARGET_FILE_DIR:Keencloo>")

    # Copy icons and resources
    set(RSC_FILES
        "${CMAKE_SOURCE_DIR}/rsc/logo_full.png"
        "${CMAKE_SOURCE_DIR}/rsc/logo_minimal.png"
        "${CMAKE_SOURCE_DIR}/rsc/close_window.ico"
        "${CMAKE_SOURCE_DIR}/rsc/folder_films.ico"
        "${CMAKE_SOURCE_DIR}/rsc/folder_films_hover.ico"
        "${CMAKE_SOURCE_DIR}/rsc/minimize_window.ico"
        "${CMAKE_SOURCE_DIR}/rsc/settings.ico"
        "${CMAKE_SOURCE_DIR}/rsc/settings_hover.ico"
		"${CMAKE_SOURCE_DIR}/rsc/fullscreen.ico"
        "${CMAKE_SOURCE_DIR}/rsc/pause.ico"
        "${CMAKE_SOURCE_DIR}/rsc/play.ico"
        "${CMAKE_SOURCE_DIR}/rsc/screenminimaze.ico"
        "${CMAKE_SOURCE_DIR}/rsc/volume_down.ico"
        "${CMAKE_SOURCE_DIR}/rsc/volume_off.ico"
        "${CMAKE_SOURCE_DIR}/rsc/volume_up.ico"
    )
	
    set(FULLSCREEN_FILES
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/fullscreen.ico"
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/pause.ico"
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/play.ico"
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/screenminimaze.ico"
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/volume_down.ico"
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/volume_off.ico"
        "${CMAKE_SOURCE_DIR}/rsc/fullscreen/volume_up.ico"
    )
	
	# Copy resources
    copy_resources(Keencloo "${FULLSCREEN_FILES}" 	"$<TARGET_FILE_DIR:Keencloo>/rsc/fullscreen")
	copy_resources(Keencloo "${RSC_FILES}" 			"$<TARGET_FILE_DIR:Keencloo>/rsc")
	copy_resources(Keencloo "${FULLSCREEN_FILES}" 	"${CMAKE_SOURCE_DIR}/build/rsc/fullscreen")
    copy_resources(Keencloo "${RSC_FILES}" 			"${CMAKE_SOURCE_DIR}/build/rsc")

    # Copy styles
    copy_resources(Keencloo "${CMAKE_SOURCE_DIR}/styles/style.qss" "$<TARGET_FILE_DIR:Keencloo>/styles")
endif()

# Set start-up project as Keencloo but now all_build
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Keencloo)