#include "imagemap.h"

char *__img = NULL;

ImageInfo _image_info[]={
{0,13,7},
{12,8,6},
{19,8,8},
{28,9,5},
{34,5,3},
{36,11,11},
{52,5,7},
{57,7,5},
{62,7,5},
{67,5,7},
{72,5,9},
{78,3,5},
{80,8,8},
{89,8,8},
{98,15,16},
{129,11,11},
{145,11,11},
{161,11,11},
{177,5,9},
{183,3,5},
{185,22,13},
{221,9,5},
{227,5,3},
{229,6,10},
{237,6,10},
{245,6,10},
{253,6,10},
{261,6,10},
{269,6,10},
{277,6,10},
{285,6,10},
{293,6,10},
{301,6,10},
{309,6,10},
{317,6,10},
{325,6,10},
{333,6,10},
};

ImageInfo *GetStockImage(int index)
{
	if( index < 0 || index >= IMAGE_MAX)
		index = 0;

	static ImageInfo info;

	info.addr = __img + _image_info[index].addr;
	info.color_num = _image_info[index].color_num;
	info.width = _image_info[index].width;
	info.height = _image_info[index].height;
	
	return &info;
}
void SetStockImgBuf(void *buf)
{
	__img = buf;
}
