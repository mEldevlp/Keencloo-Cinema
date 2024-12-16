#include "ExtractVideoMetaData.h"

MetaDataVideo extract_video_metadata(const char* file)
{
    MetaDataVideo metadata;

    // Open the video file
    AVFormatContext* formatContext = nullptr;
    if (avformat_open_input(&formatContext, file, nullptr, nullptr) != 0)
    {
        return MetaDataVideo();
    }

    // Get info about format
    if (avformat_find_stream_info(formatContext, nullptr) < 0)
    {
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    // Find first video and audio stream
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;

    uint32_t stream = 0;
    while ((audioStreamIndex == -1 && videoStreamIndex == -1) || (stream < formatContext->nb_streams))
    {
        switch (formatContext->streams[stream]->codecpar->codec_type)
        {
        case AVMediaType::AVMEDIA_TYPE_VIDEO: videoStreamIndex = stream; break;
        case AVMediaType::AVMEDIA_TYPE_AUDIO: audioStreamIndex = stream; break;
        }
        ++stream;
    }

    // No audio stream found 
    if (audioStreamIndex == -1 || videoStreamIndex == -1)
    {
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    AVCodecParameters* audCodecParams = formatContext->streams[audioStreamIndex]->codecpar;
    AVCodecParameters* vidCodecParams = formatContext->streams[videoStreamIndex]->codecpar;

    const AVCodec* codec = avcodec_find_decoder(vidCodecParams->codec_id);

    if (!codec)
    {
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (!codecContext)
    {
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    if (avcodec_parameters_to_context(codecContext, vidCodecParams) < 0)
    {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0)
    {
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    AVFrame* frame = av_frame_alloc();
    AVFrame* frameRGB = av_frame_alloc();
    if (!frame || !frameRGB)
    {
        av_frame_free(&frame);
        av_frame_free(&frameRGB);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);

    std::vector<uint8_t> buffer(numBytes);

    struct SwsContext* swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt,
        codecContext->width, codecContext->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);

    av_image_fill_arrays(frameRGB->data, frameRGB->linesize, buffer.data(), AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);

    // get 5% frame from video
    int64_t timestamp = av_rescale_q(formatContext->duration * 0.05, AV_TIME_BASE_Q, formatContext->streams[videoStreamIndex]->time_base);

    if (av_seek_frame(formatContext, videoStreamIndex, timestamp, AVSEEK_FLAG_BACKWARD) < 0)
    {
        sws_freeContext(swsContext);
        av_frame_free(&frame);
        av_frame_free(&frameRGB);
        avcodec_free_context(&codecContext);
        avformat_close_input(&formatContext);
        return MetaDataVideo();
    }

    avcodec_flush_buffers(codecContext);
   
    AVPacket packet = { 0 };
    packet.data = nullptr;
    packet.size = 0;

    // Read packets and decode
    while (av_read_frame(formatContext, &packet) >= 0)
    {
        if (packet.stream_index == videoStreamIndex)
        {
            if (avcodec_send_packet(codecContext, &packet) == 0)
            {
                while (avcodec_receive_frame(codecContext, frame) == 0)
                {
                    // Convert frame to RGB
                    sws_scale(swsContext, reinterpret_cast<uint8_t const* const*>(frame->data), frame->linesize,
                        0, codecContext->height, frameRGB->data, frameRGB->linesize);

                    metadata.frame_data.resolution.width = codecContext->width;
                    metadata.frame_data.resolution.height = codecContext->height;
                    metadata.frame_data.channels = 3; // RGB

                    metadata.frame_data.data = buffer;
                    break;
                }
            }
        }

        av_packet_unref(&packet);

        if (!metadata.frame_data.data.empty())
        {
            break; // If we get first frame then break the loop
        }
    }

    metadata.file_name = std::filesystem::path(file).stem().string();
    metadata.file_path = file;
    metadata.duration = formatContext->duration;
    metadata.video_bit_rate = vidCodecParams->bit_rate;
    metadata.audio_bit_rate = audCodecParams->bit_rate;
    metadata.file_size = std::filesystem::file_size(file);
    metadata.fps = av_q2d(formatContext->streams[videoStreamIndex]->avg_frame_rate);
    
    // Close file and free resources
    sws_freeContext(swsContext);
    av_frame_free(&frame);
    av_frame_free(&frameRGB);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);

    return metadata;
}