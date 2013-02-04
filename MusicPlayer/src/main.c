/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               main.c
** Last modified Date:      2012-2-2
** Last Version:            1.0.0
** Descriptions:            主函数
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-2
** Version:                 1.0.0
** Descriptions:            创建文件
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <drivers/audio.h>

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"

/*
 * 音频播放器线程
 */
static void *player_thread(void *arg)
{
    AVFormatContext            *format_ctx = NULL;
    AVCodecContext             *codec_ctx = NULL;
    AVCodec                    *codec = NULL;
    AVPacket                    packet, __packet;
    int                         i, audio_stream;
    int                         audio;
    int                         ret = -1;
    const char                 *filename = arg;
    int16_t                    *samples;
    int                         frame_size;

    if (avformat_open_input(&format_ctx, filename, NULL, NULL) < 0) {
        printf("failed to %s\n", "avformat_open_input");
        goto error0;
    }

    if (av_find_stream_info(format_ctx) < 0) {
        printf("failed to %s\n", "av_find_stream_info");
        goto error1;
    }

    av_dump_format(format_ctx, 0, filename, 0);

    audio_stream = -1;
    for (i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream = i;
            break;
        }
    }

    if (audio_stream == -1) {
        printf("failed to %s\n", "audio_stream == -1");
        goto error2;
    }

    codec_ctx = format_ctx->streams[audio_stream]->codec;

    codec = avcodec_find_decoder(codec_ctx->codec_id);
    if (codec == NULL) {
        printf("failed to %s\n", "avcodec_find_decoder");
        goto error3;
    }

    if (avcodec_open(codec_ctx, codec) < 0) {
        printf("failed to %s\n", "avcodec_open");
        goto error4;
    }

    audio = open("/dev/audio", O_WRONLY);
    if (audio < 0) {
        printf("failed to %s\n", "open /dev/audio");
        goto error5;
    }

    ioctl(audio, AUDIO_SAMPLE_RATE, codec_ctx->sample_rate);
    ioctl(audio, AUDIO_CHANNELS_NR,  codec_ctx->channels);
    ioctl(audio, AUDIO_SAMPLE_BITS, codec_ctx->bits_per_raw_sample ? codec_ctx->bits_per_raw_sample : 16);

    samples = malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
    if (samples == NULL) {
        printf("failed to %s\n", "malloc AVCODEC_MAX_AUDIO_FRAME_SIZE");
        goto error6;
    }

    while (av_read_frame(format_ctx, &packet) >= 0) {

        if (packet.stream_index == audio_stream) {

            memcpy(&__packet, &packet, sizeof(__packet));

            while (__packet.size > 0) {

                frame_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;

                ret = avcodec_decode_audio3(codec_ctx, samples, &frame_size, &__packet);
                if (ret <= 0) {
                    break;
                }

                write(audio, samples, frame_size);

                __packet.size -= ret;
                __packet.data += ret;
            }
        }

        av_free_packet(&packet);
    }

    free(samples);

    error6:
    close(audio);

    error5:
    avcodec_close(codec_ctx);

    error4:
    error3:
    error2:
    error1:
    av_close_input_file(format_ctx);

    error0:
    return NULL;
}

/*
 * main 函数
 */
int main(int argc, char *argv[])
{
    pthread_t player_tid;

    if (argc < 2) {
        printf("Usage: %s file.mp3\n", argv[0]);
        return -1;
    }

    avcodec_register_all();

    av_register_all();

    pthread_create(&player_tid, NULL, player_thread, argv[1]);

    while (1) {
        pthread_yield_np();
    }

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
