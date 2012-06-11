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
** File name:               goahead_smileos.c
** Last modified Date:      2012-6-11
** Last Version:            1.0.0
** Descriptions:            Goahead ��ֲ�� SmileOS �ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-6-11
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#include "../uemf.h"
#include "../wsIntrn.h"

#ifdef WEBS_SSL_SUPPORT
#include "../websSSL.h"
#endif

#ifdef USER_MANAGEMENT_SUPPORT
#include "../um.h"
void formDefineUserMgmt(void);
#endif
/*********************************************************************************************************
    ȫ�ֱ���
*********************************************************************************************************/
static int __GbFinished = FALSE;
/*********************************************************************************************************
    ���Ժ�����
*********************************************************************************************************/
static int  aspTest(int eid, webs_t wp, int argc, char_t **argv);
static void formTest(webs_t wp, char_t *path, char_t *query);
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
                                int arg, char_t *url, char_t *path, char_t *query);
extern void defaultErrorHandler(int etype, char_t *msg);
extern void defaultTraceHandler(int level, char_t *buf);
static int  memLeaks(int iArgC, char *pcArgV[]);
/*********************************************************************************************************
** ��������: defaultTraceHandler
** ��������: Default error handler.  The developer should insert code to handle
**           error messages in the desired manner.
** �䡡��  : ...
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void defaultErrorHandler(int etype, char_t *msg)
{
}
/*********************************************************************************************************
** ��������: defaultErrorHandler
** ��������: Trace log. Customize this function to log trace output
** �䡡��  : ...
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void defaultTraceHandler(int level, char_t *buf)
{
}
/*********************************************************************************************************
** ��������: initWebs
** ��������: ��ʼ�� web ����������
** �䡡��  : pcAddr        ������ַ
**           pcWebPath     web ·��
**           iPort         �˿ں�
** �䡡��  : ERROR CODE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static int initWebs(char *pcAddr, char *pcWebPath, int iPort)
{
    int             iError;
    char            cBuffer[64];
    char           *pcString;
    char_t          wbuf[128];
    struct in_addr  intaddr;

    intaddr.s_addr = inet_addr(pcAddr);

    iError = access(pcWebPath, 0);
    if (iError < 0) {
        return -1;
    }

    /*
     *  Configure the web server options before opening the web server
     */
    websSetDefaultDir(pcWebPath);
    pcString = inet_ntoa_r(intaddr, cBuffer, sizeof(cBuffer));
    ascToUni(wbuf, pcString, min(strlen(pcString) + 1, sizeof(wbuf)));
    websSetIpaddr(wbuf);
    ascToUni(wbuf, pcAddr, min(strlen(pcAddr) + 1, sizeof(wbuf)));
    websSetHost(wbuf);

    /*
     *  Configure the web server options before opening the web server
     */
    websSetDefaultPage(T("default.asp"));
    websSetPassword("123456");

    /*
     *  Open the web server on the given port. If that port is taken, try
     *  the next sequential port for up to "retries" attempts.
     */
    websOpenServer(iPort, 5);

    /*
     *  First create the URL handlers. Note: handlers are called in sorted order
     *  with the longest path handler examined first. Here we define the security
     *  handler, forms handler and the default web page handler.
     */
    websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler, WEBS_HANDLER_FIRST);
    websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
    websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
    websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler, WEBS_HANDLER_LAST);

    /*
     *  Now define two test procedures. Replace these with your application
     *  relevant ASP script procedures and form functions.
     */
    websAspDefine(T("aspTest"), aspTest);
    websFormDefine(T("formTest"), formTest);

    /*
     *  Create the Form handlers for the User Management pages
     */
#ifdef USER_MANAGEMENT_SUPPORT
    formDefineUserMgmt();
#endif

    /*
     *  Create a handler for the default home page
     */
    websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0);

    return 0;
}
/*********************************************************************************************************
** ��������: websHomePageHandler
** ��������: Home page handler
** �䡡��  : ...
** �䡡��  : 1, 0
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
                               int arg, char_t *url, char_t *path, char_t *query)
{
    /*
     *  If the empty or "/" URL is invoked, redirect default URLs to the home page
     */
    if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
        websRedirect(wp, T("home.asp"));
        return 1;
    }
    return 0;
}
/*********************************************************************************************************
** ��������: aspTest
** ��������: Test Javascript binding for ASP. This will be invoked when "aspTest" is
**           embedded in an ASP page. See web/asp.asp for usage. Set browser to
**           localhost/asp.asp" to test.
** �䡡��  : ...
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static int aspTest(int eid, webs_t wp, int argc, char_t **argv)
{
    char_t *name, *address;

    if (ejArgs(argc, argv, T("%s %s"), &name, &address) < 2) {
        websError(wp, 400, T("Insufficient args\n"));
        return -1;
    }
    return websWrite(wp, T("Name: %s, Address %s"), name, address);
}
/*********************************************************************************************************
** ��������: aspTest
** ��������: Test form for posted data (in-memory CGI). This will be called when the
**           form in web/forms.asp is invoked. Set browser to "localhost/forms.asp" to test.
** �䡡��  : ...
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static void formTest(webs_t wp, char_t *path, char_t *query)
{
    char_t *name, *address;

    name = websGetVar(wp, T("name"), T("Joe Smith"));
    address = websGetVar(wp, T("address"), T("1212 Milky Way Ave."));

    websHeader(wp);
    websWrite(wp, T("<body><h2>Name: %s, Address: %s</h2>\n"), name, address);
    websFooter(wp);
    websDone(wp, 200);
}
/*********************************************************************************************************
** ��������: websGetCgiCommName
** ��������: Returns a pointer to an allocated qualified unique temporary file name.
**           This filename must eventually be deleted with bfree();
** �䡡��  : ...
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
char_t *websGetCgiCommName()
{
    char_t *tname, buf[FNAMESIZE];

    fmtAlloc(&tname,FNAMESIZE, T("%s/%s"), ggetcwd(buf, FNAMESIZE), tmpnam(NULL));
    return tname;
}
/*********************************************************************************************************
** ��������: websLaunchCgiProc
** ��������: Launch the CGI process and return a handle to it.
** �䡡��  : ...
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int websLaunchCgiProc(char_t *cgiPath, char_t **argp, char_t **envp,
                      char_t *stdIn, char_t *stdOut)
{
    return -1;
}
/*********************************************************************************************************
** ��������: vxWebsCgiEntry
** ��������: This is the CGI process wrapper.
** �䡡��  : ...
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
void vxWebsCgiEntry(void *entryAddr(int argc, char_t **argv),
                    char_t **argp, char_t **envp,
                    char_t *stdIn, char_t *stdOut)
{
}
/*********************************************************************************************************
** ��������: websCheckCgiProc
** ��������: Check the CGI process.  Return 0 if it does not exist; non 0 if it does.
** �䡡��  : ...
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int websCheckCgiProc(int handle)
{
    return 0;
}
/*********************************************************************************************************
** ��������: webServerStart
** ��������: ���� GoAhead web ������
** �䡡��  : pcAddr        ������ַ
**           pcWebPath     web ·��
**           iPort         �˿ں�
** �䡡��  : 0
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int webServerStart(char *pcAddr, char *pcWebPath, int iPort)
{
    /*
     *  Initialize the memory allocator. Allow use of malloc and start
     *  with a 60K heap.  For each page request approx 8KB is allocated.
     *  60KB allows for several concurrent page requests.  If more space
     *  is required, malloc will be used for the overflow.
     */
    bopen(NULL, (60 * 1024), B_USE_MALLOC);

    /*
     *  Initialize the web server
     */
    __GbFinished = FALSE;
    if (initWebs(pcAddr, pcWebPath, iPort) < 0) {
        return -1;
    }

#ifdef WEBS_SSL_SUPPORT
    websSSLOpen();
#endif

    /*
     *  Basic event loop. SocketReady returns true when a socket is ready for
     *  service. SocketSelect will block for two seconds or until an event
     *  occurs. SocketProcess will actually do the servicing.
     */
    while (!__GbFinished) {
        if (socketReady(-1) || socketSelect(-1, 2000)) {
            socketProcess(-1);
        }
        websCgiCleanup();
        emfSchedProcess();
    }

#ifdef WEBS_SSL_SUPPORT
    websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
    umClose();
#endif

    /*
     *  Close the socket module, report memory leaks and close the memory allocator
     */
    websCloseServer();
    websDefaultClose();
    socketClose();
    symSubClose();

#ifdef B_STATS
    {
        char *    pcArgv[2];
        pcArgv[0] = "goaheadmemleak";
        pcArgv[1] = "\0";
        memLeaks(1, pcArgv);                                            /*  print message on stdout     */
    }
#endif

    bclose();

    return 0;
}
/*********************************************************************************************************
** ��������: printMemStats
** ��������: Print memory usage / leaks
** �䡡��  : handle    file handle
**           fmt       format string
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
#ifdef B_STATS

static void printMemStats(int handle, char_t *fmt, ...)
{
    va_list args;
    char_t  buf[256];

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    write(handle, buf, strlen(buf));
}
/*********************************************************************************************************
** ��������: memLeaks
** ��������: �ڴ�й¶
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static int memLeaks(int iArgC, char *pcArgV[])
{
    int fd;

    if (iArgC < 2) {
        bstats(STDOUT_FILENO, printMemStats);
        return 0;
    } else if ((fd = gopen(T(pcArgV[1]), O_CREAT | O_TRUNC | O_WRONLY, 0644)) >= 0) {
        bstats(fd, printMemStats);
        close(fd);
    }

    return 0;
}
#endif
/*********************************************************************************************************
** ��������: webServerStop
** ��������: ֹͣ GoAhead web ������
** �䡡��  : NONE
** �䡡��  : 0
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
int webServerStop(void)
{
    __GbFinished = TRUE;

    return 0;
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
