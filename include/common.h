#pragma once

#pragma region("STD")
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <string>
#pragma endregion

#pragma region("QT")
#include <QtCore>

#include <QtWidgets>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

#include <QMouseEvent>
#include <QPropertyAnimation>
#pragma endregion

#pragma region("External")
extern "C" {
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
	
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#pragma endregion

#define VERSION "0.16.0"

#define KEENCLOO_DEBUG

#define APP_DIR QCoreApplication::applicationDirPath()
#define RSC_DIR APP_DIR + "/rsc/"

#define DEF_WIDTH 1280
#define DEF_HEIGHT 720
#define DEFAULT_RES DEF_WIDTH, DEF_HEIGHT

#define SERVE_WIDTH 720
#define SERVE_HEIGHT 480
#define SERVE_RES	SERVE_WIDTH, SERVE_HEIGHT

// Explore films list
#define MAX_LENGTH_TITLE_PX 430	