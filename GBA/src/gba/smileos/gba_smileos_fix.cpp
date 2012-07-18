/*
 * gba_sylixos_fix.cpp
 *
 *  Created on: 2011-11-15
 *      Author: jiaojinxing
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "../AutoBuild.h"

#include "../GBA.h"
#include "../agbprint.h"
#include "../Flash.h"
#include "../Port.h"
#include "../RTC.h"
#include "../Sound.h"
#include "../Text.h"
#include "../unzip.h"
#include "../Util.h"
#include "../gb/GB.h"
#include "../gb/gbGlobals.h"

bool systemSoundOn;
u16 systemColorMap16[0x10000];
u32 systemColorMap32[0x10000];
u16 systemGbPalette[24];
int systemRedShift;
int systemGreenShift;
int systemBlueShift;
int systemColorDepth;
int systemDebug;
int systemVerbose;
int systemFrameSkip;
int systemSaveUpdateCounter;

int RGB_LOW_BITS_MASK;
int emulating;
bool debugger;

void debuggerOutput(char *str, u32 addr)
{
    /*
     * do nothing
     */
}
void (*dbgOutput)(char *, u32) = debuggerOutput;

static FILE  *logfile = NULL;

void log(const char *fmt, ...)
{
    char     buffer[256];
    va_list  valist;

    va_start(valist, fmt);
    vsprintf(buffer, fmt, valist);
    if (logfile == NULL) {
        logfile = fopen("gba.log", "w");
    }
    fputs(buffer, logfile);
    va_end(valist);
}

void winlog(const char *fmt, ...)
{
    char     buffer[256];
    va_list  valist;

    va_start(valist, fmt);
    vsprintf(buffer, fmt, valist);
    if (logfile == NULL) {
        logfile = fopen("gba.log", "w");
    }
    fputs(buffer, logfile);
    va_end(valist);
}

bool systemPauseOnFrame()
{
    return false;
}

void systemGbPrint(u8 *data, int pages, int feed, int palette, int contrast)
{

}

void systemScreenCapture(int num)
{
    /*
     * do nothing
     */
}

// updates the joystick data
bool systemReadJoypads()
{
    /*
     * do nothing
     */
    return true;
}

// return information about the given joystick, -1 for default joystick
u32 systemReadJoypad(int which)
{
    /*
     * do nothing
     */
    return 0;
}

u32 systemGetClock()
{
    static int ticks = 0;

    return ticks++;
}

void systemMessage(int num, const char *fmt, ...)
{
    char buffer[256];
    va_list valist;

    va_start(valist, fmt);
    vsprintf(buffer, fmt, valist);
    fprintf(stderr, "%s\n", buffer);
    va_end(valist);
}

void systemSetTitle(const char *title)
{
    fprintf(stderr, "%s:%s\n", __func__, title);
}

void systemWriteDataToSoundBuffer()
{
    /*
     * do nothing
     */
}

void systemSoundShutdown()
{
    /*
     * do nothing
     */
}

void systemSoundPause()
{
    /*
     * do nothing
     */
}

void systemSoundResume()
{
    /*
     * do nothing
     */
}

void systemSoundReset()
{
    /*
     * do nothing
     */
}

bool systemSoundInit()
{
    return true;
}

void systemScreenMessage(const char *msg)
{
    fprintf(stderr, "%s:%s\n", __func__, msg);
}

void systemUpdateMotionSensor()
{
    /*
     * do nothing
     */
}

int systemGetSensorX()
{
    return 0;
}

int systemGetSensorY()
{
    return 0;
}

bool systemCanChangeSoundQuality()
{
    return false;
}

void systemShowSpeed(int speed)
{
    /*
     * do nothing
     */
}

void system10Frames(int rate)
{
    /*
     * do nothing
     */
}

void systemFrame()
{
    /*
     * do nothing
     */
}

void systemGbBorderOn()
{
    /*
     * do nothing
     */
}

int gba_quit(int argc, char **argv)
{
    emulating = false;

    return 0;
}

struct FbInfo
{
    int             fd;
    void           *bits;
    struct fb_var_screeninfo   varinfo;
    struct fb_fix_screeninfo   fixinfo;
};

static struct FbInfo _G_FbInfo;

#define fb_width(fb)    ((fb)->varinfo.xres)
#define fb_height(fb)   ((fb)->varinfo.yres)
#define fb_bpp(fb)      ((fb)->varinfo.bits_per_pixel)
#define fb_size(fb)     (fb_width(fb) * fb_height(fb) * fb_bpp(fb) >> 3)

extern "C" {
int ioctl(int fd, int cmd, ...);
}

static int gmem_open(struct FbInfo *fb, const char *filename)
{
    fb->fd = open(filename, O_RDWR);
    if (fb->fd < 0) {
        return -1;
    }

    if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->varinfo) < 0) {
        goto fail;
    }

    if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fixinfo) < 0) {
        goto fail;
    }

    fb->bits = fb->fixinfo.smem_start;

    memset(fb->bits, 0x00, fb_size(fb));

    return 0;

fail:
    close(fb->fd);

    return -1;
}

static void gmem_close(struct FbInfo *fb)
{
    memset(fb->bits, 0x00, fb_size(fb));

    close(fb->fd);
}

void systemDrawScreen()
{
    int x, y;
    unsigned short *src = (unsigned short *)pix;
    unsigned short *dst = (unsigned short *)_G_FbInfo.bits;

    for (y = 0; y < 162; y++) {
        for (x = 0; x < 242; x++) {
            dst[2 * x]     = src[y * 242 + x];
            dst[2 * x + 1] = src[y * 242 + x];
        }
        dst += fb_width(&_G_FbInfo);

        for (x = 0; x < 242; x++) {
            dst[2 * x]     = src[y * 242 + x];
            dst[2 * x + 1] = src[y * 242 + x];
        }
        dst += fb_width(&_G_FbInfo);
    }
}

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        printf("Usage: %s file.gba\n", argv[0]);
        return -1;
    }

    useBios                 = true;

    systemSoundOn           = false;

    systemRedShift          = 11;
    systemGreenShift        = 6;
    systemBlueShift         = 0;
    systemColorDepth        = 16;

    for (i = 0; i < 0x10000; i++) {
        systemColorMap16[i] = ((i & 0x1f) << systemRedShift) |
          (((i & 0x3e0 ) >>  5) << systemGreenShift) |
          (((i & 0x7c00) >> 10) << systemBlueShift);
    }

    systemDebug             = 0;
    systemVerbose           = 0;
    systemFrameSkip         = 0;
    systemSaveUpdateCounter = SYSTEM_SAVE_NOT_UPDATED;

    RGB_LOW_BITS_MASK       = 0x821;
    debugger                = 0;
    emulating               = true;

    if (CPULoadRom(argv[1]) == 0) {
        return -1;
    }

    CPUInit(NULL, useBios);

    CPUReset();

    gmem_open(&_G_FbInfo, "/dev/fb0");

    while (emulating) {
        GBASystem.emuMain(systemGetClock());
    }

    GBASystem.emuCleanUp();

    gmem_close(&_G_FbInfo);

    return 0;
}
