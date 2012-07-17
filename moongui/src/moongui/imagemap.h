#ifndef __IMAGEMAP_H
#define __IMAGEMAP_H

#include "gal.h"

enum {
IMAGE_BACK_ARROW,
IMAGE_CHECKED,
IMAGE_CLOSE,
IMAGE_DOWN_ARROW,
IMAGE_DOWN_SMALL_ARROW,
IMAGE_FOLDER_UP,
IMAGE_KB_DOWN_ARROW,
IMAGE_KB_LEFT_ARROW,
IMAGE_KB_RIGHT_ARROW,
IMAGE_KB_UP_ARROW,
IMAGE_LEFT_ARROW,
IMAGE_LEFT_SMALL_ARROW,
IMAGE_MINE_FLAG,
IMAGE_MINE_MINE,
IMAGE_PROPERTY,
IMAGE_RADIO_BACK,
IMAGE_RADIO_CIRCLE,
IMAGE_RADIO_DOT,
IMAGE_RIGHT_ARROW,
IMAGE_RIGHT_SMALL_ARROW,
IMAGE_SMALL_KB,
IMAGE_UP_ARROW,
IMAGE_UP_SMALL_ARROW,
IMAGE_YINBIAO_0,
IMAGE_YINBIAO_1,
IMAGE_YINBIAO_10,
IMAGE_YINBIAO_11,
IMAGE_YINBIAO_12,
IMAGE_YINBIAO_13,
IMAGE_YINBIAO_2,
IMAGE_YINBIAO_3,
IMAGE_YINBIAO_4,
IMAGE_YINBIAO_5,
IMAGE_YINBIAO_6,
IMAGE_YINBIAO_7,
IMAGE_YINBIAO_8,
IMAGE_YINBIAO_9,
IMAGE_MAX,
};

typedef struct{
    int addr;
    int width;
    int height;
    int color_num;
}ImageInfo;

#ifdef __cplusplus
extern "C" {
#endif

ImageInfo *GetStockImage(int index);
void SetStockImgBuf(void *buf);

#ifdef __cplusplus
} //extern "C"
#endif
#endif
