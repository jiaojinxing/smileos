/*
 * audio.h
 *
 *  Created on: 2012-8-28
 *      Author: Administrator
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <sys/ioctl.h>

#define AUDIO_SAMPLE_RATE           _IO('a', 'r')
#define AUDIO_SAMPLE_BITS           _IO('a', 'b')
#define AUDIO_CHANNELS_NR           _IO('a', 'c')

#endif /* AUDIO_H_ */
