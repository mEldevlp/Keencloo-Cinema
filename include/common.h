#pragma once

#pragma region("STD")
#include <iostream>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#pragma endregion

#pragma region("QT")
#include <QtCore>

#include <QtWidgets>
#include <QtMultimedia>
#include <QtMultimediaWidgets>

#include <QMouseEvent>
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

#pragma endregion

#define KEENCLOO_DEBUG

#define DEFAULT_RES 1280, 720
#define SERVE_RES	720, 480

#define MAX_LENGTH_TITLE_PX 430