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
** File name:               ffplay.c
** Last modified Date:      2012-6-14
** Last Version:            1.0.0
** Descriptions:            基于 FFmpeg 的简单视频播放器
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-14
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>

#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"

/*
 * 播放视频文件
 */
int ffplay(const char *filename)
{
    AVFormatContext            *format_ctx = NULL;
    AVCodecContext             *codec_ctx = NULL;
    AVCodec                    *codec = NULL;
    AVFrame                    *raw_frame = NULL;
    AVFrame                    *rgb_frame = NULL;
    AVPacket                    packet;
    struct fb_var_screeninfo    var_info;
    struct fb_fix_screeninfo    fix_info;
    static struct SwsContext   *sws_ctx = NULL;
    int                         i, video_stream;
    int                         got_picture;
    int                         fb;
    int                         ret = -1;

    avcodec_register_all();

    av_register_all();

    if (avformat_open_input(&format_ctx, filename, NULL, NULL) < 0) {
        printf("failed to %s\n", "avformat_open_input");
        goto error0;
    }

    if (av_find_stream_info(format_ctx) < 0) {
        printf("failed to %s\n", "av_find_stream_info");
        goto error1;
    }

    /*
     * TODO:
     * 加上下面一句后, 在多进程下挂掉, 原因不明
     */
    //av_dump_format(format_ctx, 0, filename, 0);

    video_stream = -1;
    for (i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = i;
            break;
        }
    }

    if (video_stream == -1) {
        printf("failed to %s\n", "video_stream == -1");
        goto error2;
    }

    codec_ctx = format_ctx->streams[video_stream]->codec;

    codec = avcodec_find_decoder(codec_ctx->codec_id);
    if (codec == NULL) {
        printf("failed to %s\n", "avcodec_find_decoder");
        goto error3;
    }

    if (avcodec_open(codec_ctx, codec) < 0) {
        printf("failed to %s\n", "avcodec_open");
        goto error4;
    }

    raw_frame = avcodec_alloc_frame();
    if (raw_frame == NULL) {
        printf("failed to %s\n", "avcodec_alloc_frame 1");
        goto error5;
    }

    rgb_frame = avcodec_alloc_frame();
    if (rgb_frame == NULL) {
        printf("failed to %s\n", "avcodec_alloc_frame 2");
        goto error6;
    }

    fb = open("/dev/fb0", O_RDWR, 0666);
    if (fb < 0) {
        printf("failed to %s\n", "open /dev/fb0");
        goto error7;
    }

    ioctl(fb, FBIOGET_VSCREENINFO, &var_info);

    ioctl(fb, FBIOGET_FSCREENINFO, &fix_info);

    avpicture_fill((AVPicture *)rgb_frame, fix_info.smem_start, PIX_FMT_RGB565,
                   var_info.xres, var_info.yres);

    sws_ctx = sws_getCachedContext(sws_ctx, codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
                                   var_info.xres, var_info.yres, PIX_FMT_RGB565,
                                   SWS_BICUBIC, NULL, NULL, NULL);
    if (sws_ctx == NULL) {
        printf("failed to %s\n", "sws_getCachedContext");
        goto error8;
    }

    while (av_read_frame(format_ctx, &packet) >= 0) {

        if (packet.stream_index == video_stream) {
            avcodec_decode_video2(codec_ctx, raw_frame, &got_picture, &packet);

            if (got_picture) {
                sws_scale(sws_ctx,
                          raw_frame->data, raw_frame->linesize, 0, codec_ctx->height,
                          rgb_frame->data, rgb_frame->linesize);
            }
        }

        av_free_packet(&packet);

        usleep(1000 * 10);
    }

    ret = 0;

    error8:
    close(fb);

    error7:
    av_free(rgb_frame);

    error6:
    av_free(raw_frame);

    error5:
    avcodec_close(codec_ctx);

    error4:
    error3:
    error2:
    error1:
    av_close_input_file(format_ctx);

    error0:
    return ret;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
