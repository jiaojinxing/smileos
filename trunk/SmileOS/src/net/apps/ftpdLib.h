/*
 * ftpdLib.h
 *
 *  Created on: 2013-2-5
 *      Author: jiaojinxing
 */

#ifndef FTPDLIB_H_
#define FTPDLIB_H_

#include "./ftpdLibFix.h"

IMPORT int ftpdDebug;
IMPORT int ftpdTaskPriority;
IMPORT int ftpdTaskOptions;
IMPORT int ftpdWorkTaskPriority;
IMPORT int ftpdWorkTaskOptions;
IMPORT int ftpdWorkTaskStackSize;
IMPORT int ftpdWindowSize;
IMPORT int ftpsMaxClients;

extern STATUS   ftpdInit(int port, int stackSize);
extern STATUS   ftpdDelete(void);
extern void     ftpdLoginInstall(FUNCPTR pLoginFunc);
extern STATUS   ftpdHomeDirSet(const char * homeDir);
extern STATUS   ftpdAnonymousAllow(const char *rootDir, const char *uploadDir);

#endif /* FTPDLIB_H_ */
