#pragma once

#include "common.h"

struct Resolution
{
	int width;
	int height;
};

struct VideoFrame
{
	Resolution resolution;
	int channels; // RGB
	std::vector<uint8_t> data;
};

struct MetaDataVideo
{
	std::string file_name;
	const char* file_path;
	int64_t duration;
	int64_t video_bit_rate;
	int64_t audio_bit_rate;
	int64_t file_size;
	double fps;
	VideoFrame frame_data;
};

MetaDataVideo extract_video_metadata(const char* file);