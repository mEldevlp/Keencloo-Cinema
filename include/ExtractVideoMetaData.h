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
	long long duration;
	long long video_bit_rate;
	long long audio_bit_rate;
	long long file_size;
	double fps;
	VideoFrame frame_data;
};

MetaDataVideo extract_video_metadata(const char* file);