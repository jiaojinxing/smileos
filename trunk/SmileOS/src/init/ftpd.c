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
** File name:               ftpd.c
** Last modified Date:      2012-7-19
** Last Version:            1.0.0
** Descriptions:            ftp 服务器
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-7-19
** Version:                 1.0.0
** Descriptions:            创建文件
**
*********************************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#include <syslimits.h>
#include <dirent.h>

#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
/*********************************************************************************************************
  配置
*********************************************************************************************************/
#define FTPD_PORT               21
#define FTPD_SRV_ROOT           "/"
#define FTPD_MAX_CONNECTION     2
#define FTPD_USER               "SmileOS"
#define FTPD_PASSWORD           "1234"
#define FTPD_WELCOME_MSG        "220-= welcome on SmileOS FTP server =-\r\n220 \r\n"
#define FTPD_BUFFER_SIZE        1024
/*********************************************************************************************************
  FTPD 会话
*********************************************************************************************************/
struct ftpd_session {
    struct sockaddr_in          remote;
    int                         fd;
    int                         pasv_fd;
    size_t                      offset;
    unsigned short              pasv_port;
	char                        is_anonymous;
    char                        pasv_active;
	char                        cwd[PATH_MAX];
	struct ftpd_session         *next;
};
/*********************************************************************************************************
  FTPD 会话链表
*********************************************************************************************************/
static struct ftpd_session *session_list = NULL;
/*********************************************************************************************************
  FTPD 会话函数
*********************************************************************************************************/
/*
 * 新建一个会话
 */
static struct ftpd_session *ftpd_new_session(void)
{
	struct ftpd_session *session;

	session = (struct ftpd_session *)malloc(sizeof(struct ftpd_session));
	if (session != NULL) {
	    session->next = session_list;
	    session_list = session;
	}
	return session;
}

/*
 * 关闭会话
 */
static void ftpd_close_session(struct ftpd_session *session)
{
	struct ftpd_session *tmp;

	if (session_list == session) {
        session_list  = session_list->next;
        session->next = NULL;
    } else {
        tmp = session_list;
        while (tmp->next != session) {
            tmp = tmp->next;
        }
        tmp->next     = session->next;
        session->next = NULL;
    }
	free(session);
}
/*********************************************************************************************************
  FTPD 工具函数
*********************************************************************************************************/
/*
 * 获得文件的大小
 */
static int ftpd_get_filesize(const char *path)
{
    struct stat st;
    int ret;
	int fd;

	fd = open(path, O_RDONLY, 0666);
	if (fd < 0) {
	    return -1;
	}

	ret = fstat(fd, &st);
	if (ret < 0) {
	    close(fd);
	    return -1;
	}

	close(fd);

	return st.st_size;
}

/*
 * 判断路径是否绝对路径
 */
static int ftpd_is_absolute_path(const char *path)
{
	if (path[0] == '/') {
	    return TRUE;
	} else {
	    return FALSE;
	}
}

/*
 * 构建全路径
 */
static int ftpd_build_full_path(struct ftpd_session *session, const char *path, char *full_path, size_t size)
{
	if (ftpd_is_absolute_path(path) == TRUE) {
        strcpy(full_path, path);
    } else {
        sprintf(full_path, "%s/%s", session->cwd, path);
    }
    return 0;
}

/*
 * 判断字符串是否以指定的子串开始
 */
static int ftpd_str_begin_with(const char *src, const char *match)
{
    while (*match != '\0') {
        if (*src == '\0') {
            return FALSE;
        }

        if (*match != *src) {
            return FALSE;
        }

        match++;
        src++;
    }
    return TRUE;
}
/*********************************************************************************************************
  FTPD 请求处理函数
*********************************************************************************************************/
/*
 * USER 命令
 */
int ftpd_do_user(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    if (strcmp(param, "anonymous") == 0) {
        session->is_anonymous = TRUE;
        len = sprintf(buf, "331 User name okay, need password.\r\n");
        write(session->fd, buf, len);

    } else if (strcmp(param, FTPD_USER) == 0) {
        session->is_anonymous = FALSE;
        len = sprintf(buf, "331 User name okay, need password.\r\n");
        write(session->fd, buf, len);

    } else {
        len = sprintf(buf, "530 User not logged in.\r\n");
        write(session->fd, buf, len);
        return -1;
    }

    return 0;
}

/*
 * PASS 命令
 */
int ftpd_do_pass(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    if (strcmp(param, FTPD_PASSWORD) == 0 || session->is_anonymous == TRUE) {
        len = sprintf(buf, "230 User logged in, proceed.\r\n");
        write(session->fd, buf, len);
        return 0;
    } else {
        len = sprintf(buf, "530 User not logged in.\r\n");
        write(session->fd, buf, len);
        return -1;
    }
}

/*
 * LIST 命令
 */
static int ftpd_do_list_file(const char *path, int fd)
{
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char buf[LINE_MAX];
    int len;

    dir = opendir(path);
    if (dir == NULL) {
        len = sprintf(buf, "500 The command was not accepted and the requested action did not take place.\r\n");
        write(fd, buf, len);
        return 0;
    }

    while (1) {
        entry = readdir(dir);
        if (entry == NULL) {
            break;
        }

        sprintf(buf, "%s/%s", path, entry->d_name);
        if (stat(buf, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                len = sprintf(buf, "drw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", 0, entry->d_name);
            } else {
                len = sprintf(buf, "-rw-r--r-- 1 admin admin %d Jan 1 2000 %s\r\n", st.st_size, entry->d_name);
            }
            write(fd, buf, len);
        }
    }
    closedir(dir);

    return 0;
}

/*
 * LIST 命令
 */
int ftpd_do_list(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    len = sprintf(buf, "150 File status okay, about to open data connection.\r\n");
    write(session->fd, buf, len);

    ftpd_do_list_file(session->cwd, session->pasv_fd);

    close(session->pasv_fd);
    session->pasv_active = 0;

    len = sprintf(buf, "226 Closing data connection. Requested file action successful.\r\n");
    write(session->fd, buf, len);

    return 0;
}

/*
 * PWD 命令
 */
int ftpd_do_pwd(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    len = sprintf(buf, "257 \"%s\" is current directory.\r\n", session->cwd);
    write(session->fd, buf, len);

    return 0;
}

/*
 * TYPE 命令
 */
int ftpd_do_type(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    if (strcmp(param, "I") == 0) {
        len = sprintf(buf, "200 Type set to Binary mode.\r\n");
        write(session->fd, buf, len);
    } else {
        len = sprintf(buf, "200 Type set to Ascii mode.\r\n");
        write(session->fd, buf, len);
    }

    return 0;
}

/*
 * PASV 命令
 */
int ftpd_do_pasv(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    int ret;
    int dig1, dig2;
    int fd;
    int optval;
    struct sockaddr_in local, pasv_remote;
    fd_set readfds;
    struct timeval tv;

    session->pasv_port   = 10000;
    session->pasv_active = 0;

    dig1 = session->pasv_port / 256;
    dig2 = session->pasv_port % 256;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        len = sprintf(buf, "425 Can't open data connection.\r\n");
        write(session->fd, buf, len);
        goto error0;
    }

    optval = 1;
    ret  = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    ret |= setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if (ret < 0) {
        len = sprintf(buf, "425 Can't open data connection.\r\n");
        write(session->fd, buf, len);
        goto error1;
    }

    local.sin_family      = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port        = htons(session->pasv_port);

    ret = bind(fd, (struct sockaddr *)&local, sizeof(local));
    if (ret < 0) {
        len = sprintf(buf, "425 Can't open data connection.\r\n");
        write(session->fd, buf, len);
        goto error1;
    }

    ret = listen(fd, 1);
    if (ret < 0) {
        len = sprintf(buf, "425 Can't open data connection.\r\n");
        write(session->fd, buf, len);
        goto error1;
    }

    extern struct netif *netif_default;
    len = sprintf(buf, "227 Entering passive mode (%d,%d,%d,%d,%d,%d)\r\n",
            (netif_default->ip_addr.addr >> 0 ) & 0xFF,
            (netif_default->ip_addr.addr >> 8 ) & 0xFF,
            (netif_default->ip_addr.addr >> 16) & 0xFF,
            (netif_default->ip_addr.addr >> 24) & 0xFF,
            dig1, dig2);
    write(session->fd, buf, len);

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    tv.tv_sec  = 1;
    tv.tv_usec = 0;
    ret = select(fd + 1, &readfds, 0, 0, &tv);

    if (ret == 1 && FD_ISSET(fd, &readfds)) {
        socklen_t addr_len = sizeof(pasv_remote);
        session->pasv_fd = accept(fd, (struct sockaddr *)&pasv_remote, &addr_len);
        if (session->pasv_fd < 0) {
            len = sprintf(buf, "425 Can't open data connection.\r\n");
            write(session->fd, buf, len);
            goto error1;
        }
        session->pasv_active = 1;
        close(fd);
        return 0;
    } else {
        len = sprintf(buf, "425 Can't open data connection.\r\n");
        write(session->fd, buf, len);
    }

    error1:
    close(fd);

    error0:
    return 0;
}

/*
 * RETR 命令
 */
int ftpd_do_retr(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];
    int file_size;
    int fd;
    char *tmp_buf;

    if (!session->pasv_active) {
        len = sprintf(buf, "500 Internal Error.\r\n");
        write(session->fd, buf, len);
        return 0;
    }

    ftpd_build_full_path(session, param, path, PATH_MAX);

    file_size = ftpd_get_filesize(path);
    if (file_size == -1) {
        len = sprintf(buf, "550 \"%s\" : is not a regular file\r\n", path);
        write(session->fd, buf, len);
        session->offset = 0;
        return 0;
    }

    fd = open(path, O_RDONLY, 0666);
    if (fd < 0) {
        len = sprintf(buf, "500 Internal Error.\r\n");
        write(session->fd, buf, len);
        session->offset = 0;
        return 0;
    }

    if (session->offset > 0 && session->offset < file_size) {
        lseek(fd, session->offset, SEEK_SET);
        len = sprintf(buf, "150 Opening Binary mode data connection for partial \"%s\" (%d/%d bytes).\r\n",
            path, file_size - session->offset, file_size);
    } else {
        len = sprintf(buf, "150 Opening Binary mode data connection for \"%s\" (%d bytes).\r\n", path, file_size);
    }
    write(session->fd, buf, len);

    tmp_buf = malloc(FTPD_BUFFER_SIZE);
    if (tmp_buf != NULL) {
        while ((len = read(fd, tmp_buf, FTPD_BUFFER_SIZE)) > 0) {
            write(session->pasv_fd, tmp_buf, len);
        }
        free(tmp_buf);
    }

    close(fd);
    close(session->pasv_fd);
    session->pasv_active = 0;

    len = sprintf(buf, "226 Finished.\r\n");
    write(session->fd, buf, len);

    return 0;
}

/*
 * STOR 命令
 */
int ftpd_do_stor(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];
    int fd;
    char *tmp_buf;
    fd_set readfds;
    fd_set errorfds;
    struct timeval tv;

    if (session->is_anonymous == TRUE) {
        len = sprintf(buf, "550 Permission denied.\r\n");
        write(session->fd, buf, len);
        return 0;
    }

    if (!session->pasv_active) {
        len = sprintf(buf, "500 Internal Error.\r\n");
        write(session->fd, buf, len);
        return 0;
    }

    ftpd_build_full_path(session, param, path, PATH_MAX);

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        len = sprintf(buf, "550 Cannot open \"%s\" for writing.\r\n", path);
        write(session->fd, buf, len);
        return 0;
    }

    len = sprintf(buf, "150 Opening Binary mode data connection for \"%s\".\r\n", path);
    write(session->fd, buf, len);

    tmp_buf = malloc(FTPD_BUFFER_SIZE);
    if (tmp_buf != NULL) {
        do {
            int ret;

            FD_ZERO(&readfds);
            FD_ZERO(&errorfds);
            FD_SET(session->pasv_fd, &readfds);
            FD_SET(session->pasv_fd, &errorfds);
            tv.tv_sec  = 1;
            tv.tv_usec = 0;

            ret = select(session->pasv_fd + 1, &readfds, 0, &errorfds, &tv);
            if (ret <= 0) {
                break;
            }

            if (FD_ISSET(session->pasv_fd, &errorfds)) {
                break;
            }

            if (!FD_ISSET(session->pasv_fd, &readfds)) {
                break;
            }

            len = read(session->pasv_fd, tmp_buf, FTPD_BUFFER_SIZE);
            if (len <= 0) {
                break;
            }

            ret = write(fd, tmp_buf, len);
            if (ret != len) {
                break;
            }
        } while (1);
        free(tmp_buf);
    }

    close(fd);
    close(session->pasv_fd);
    session->pasv_active = 0;

    len = sprintf(buf, "226 Finished.\r\n");
    write(session->fd, buf, len);

    return 0;
}

/*
 * SIZE 命令
 */
int ftpd_do_size(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];
    int file_size;

    ftpd_build_full_path(session, param, path, PATH_MAX);

    file_size = ftpd_get_filesize(path);
    if (file_size == -1) {
        len = sprintf(buf, "550 \"%s\" : is not a regular file\r\n", path);
        write(session->fd, buf, len);
    } else  {
        len = sprintf(buf, "213 %d\r\n", file_size);
        write(session->fd, buf, len);
    }

    return 0;
}

/*
 * MDTM 命令
 */
int ftpd_do_mdtm(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    len = sprintf(buf, "550 \"/\" : not a regular file\r\n");
    write(session->fd, buf, len);
    return 0;
}

/*
 * SYST 命令
 */
int ftpd_do_syst(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    len = sprintf(buf, "215 %s\r\n", "SmileOS");
    write(session->fd, buf, len);
    return 0;
}

extern int vfs_path_normalization(char path[PATH_MAX], int sprit_end);
/*
 * CWD 命令
 */
int ftpd_do_cwd(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];

    ftpd_build_full_path(session, param, path, PATH_MAX);

    vfs_path_normalization(path, FALSE);

    len = sprintf(buf, "250 Changed to directory \"%s\"\r\n", path);
    write(session->fd, buf, len);

    strcpy(session->cwd, path);

    printf("Changed to directory %s\n", path);

    return 0;
}

/*
 * CDUP 命令
 */
int ftpd_do_cdup(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];

    sprintf(path, "%s/%s", session->cwd, "..");

    vfs_path_normalization(path, FALSE);

    len = sprintf(buf, "250 Changed to directory \"%s\"\r\n", path);
    write(session->fd, buf, len);

    strcpy(session->cwd, path);

    printf("Changed to directory %s\n", path);

    return 0;
}

/*
 * REST
 */
int ftpd_do_rest(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    if (atoi(param) >= 0) {
        session->offset = atoi(param);
        len = sprintf(buf, "350 Send RETR or STOR to start transfert.\r\n");
        write(session->fd, buf, len);
    }

    return 0;
}

/*
 * MKD 命令
 */
int ftpd_do_mkd(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];

    if (session->is_anonymous == TRUE) {
        len = sprintf(buf, "550 Permission denied.\r\n");
        write(session->fd, buf, len);
        return 0;
    }

    ftpd_build_full_path(session, param, path, PATH_MAX);

    if (mkdir(path, 0) < 0) {
        len = sprintf(buf, "550 File \"%s\" exists.\r\n", path);
        write(session->fd, buf, len);
    }  else {
        len = sprintf(buf, "257 directory \"%s\" successfully created.\r\n", path);
        write(session->fd, buf, len);
    }

    return 0;
}

/*
 * DELE 命令
 */
int ftpd_do_dele(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];

    if (session->is_anonymous == TRUE) {
        len = sprintf(buf, "550 Permission denied.\r\n");
        write(session->fd, buf, len);
        return 0;
    }

    ftpd_build_full_path(session, param, path, PATH_MAX);

    if (unlink(path) == 0) {
        len = sprintf(buf, "250 Successfully deleted file \"%s\".\r\n", path);
        write(session->fd, buf, len);
    } else {
        len = sprintf(buf, "550 Not such file or directory: %s.\r\n", path);
        write(session->fd, buf, len);
    }

    return 0;
}

/*
 * RMD 命令
 */
int ftpd_do_rmd(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;
    char path[PATH_MAX];

    if (session->is_anonymous == TRUE) {
        len = sprintf(buf, "550 Permission denied.\r\n");
        write(session->fd, buf, len);
        return 0;
    }

    ftpd_build_full_path(session, param, path, PATH_MAX);

    if (rmdir(path) == -1) {
        len = sprintf(buf, "550 Directory \"%s\" doesn't exist.\r\n", path);
        write(session->fd, buf, len);
    } else {
        len = sprintf(buf, "257 directory \"%s\" successfully deleted.\r\n", path);
        write(session->fd, buf, len);
    }

    return 0;
}

/*
 * QUIT 命令
 */
int ftpd_do_quit(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    len = sprintf(buf, "221 Bye!\r\n");
    write(session->fd, buf, len);

    return -1;
}

/*
 * UNKNOW 命令
 */
int ftpd_do_unknow(struct ftpd_session *session, char *param)
{
    char buf[LINE_MAX];
    int len;

    len = sprintf(buf, "502 Not Implemented.\r\n");
    len = write(session->fd, buf, len);

    return 0;
}
/*
 * 处理请求
 */
static int ftpd_do_request(struct ftpd_session *session, char *req)
{
	char *param, *ptr;

	/*
	 * 去掉最后的 \r\n
	 */
    ptr = req;
    while (*ptr != '\0') {
        if (*ptr == '\r' || *ptr == '\n') {
            *ptr = '\0';
        }
        ptr++;
    }

	/*
	 * 获得请求的参数
	 */
	param = strchr(req, ' ');
	if (param != NULL) {
	    param++;
	}

	/*
	 * DEBUG
	 */
	printf("%s request: \"%s\"\n", inet_ntoa(session->remote.sin_addr), req);

	/*
	 * USER 命令
	 */
	if (ftpd_str_begin_with(req, "USER") == TRUE) {
		return ftpd_do_user(session, param);

	} else if(ftpd_str_begin_with(req, "PASS") == TRUE) {
        return ftpd_do_pass(session, param);

	} else if(ftpd_str_begin_with(req, "LIST") == TRUE) {
        return ftpd_do_list(session, param);

	} else if(ftpd_str_begin_with(req, "PWD") == TRUE || ftpd_str_begin_with(req, "XPWD") == TRUE) {
        return ftpd_do_pwd(session, param);

	} else if(ftpd_str_begin_with(req, "TYPE") == TRUE) {
        return ftpd_do_type(session, param);

	} else if(ftpd_str_begin_with(req, "PASV") == TRUE) {
	    return ftpd_do_pasv(session, param);

	} else if (ftpd_str_begin_with(req, "RETR") == TRUE) {
        return ftpd_do_retr(session, param);

    } else if (ftpd_str_begin_with(req, "STOR") == TRUE) {
        return ftpd_do_stor(session, param);

    } else if (ftpd_str_begin_with(req, "SIZE") == TRUE) {
        return ftpd_do_size(session, param);

    } else if (ftpd_str_begin_with(req, "MDTM") == TRUE) {
        return ftpd_do_mdtm(session, param);

    } else if (ftpd_str_begin_with(req, "SYST") == TRUE) {
        return ftpd_do_syst(session, param);

    } else if (ftpd_str_begin_with(req, "CWD") == TRUE) {
        return ftpd_do_cwd(session, param);

    } else if (ftpd_str_begin_with(req, "CDUP") == TRUE) {
        return ftpd_do_cdup(session, param);

    } else if (ftpd_str_begin_with(req, "REST") == TRUE) {
        return ftpd_do_rest(session, param);

    } else if (ftpd_str_begin_with(req, "MKD") == TRUE) {
        return ftpd_do_mkd(session, param);

    } else if (ftpd_str_begin_with(req, "DELE") == TRUE) {
        return ftpd_do_dele(session, param);

    } else if (ftpd_str_begin_with(req, "RMD") == TRUE) {
        return ftpd_do_rmd(session, param);

    } else if (ftpd_str_begin_with(req, "QUIT") == TRUE) {
        return ftpd_do_quit(session, param);

    } else {
        return ftpd_do_unknow(session, param);
    }

	return 0;
}

void ftpd(void *arg)
{
    int ret;
    int len;
    int fd, max_fd;
    struct sockaddr_in local;
    fd_set readfds;
    fd_set errorfds;
    struct ftpd_session *session;
    struct ftpd_session *next;
    char *buffer;

    session_list = NULL;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("failed to create socket\n");
        return;
    }

    local.sin_port          = htons(FTPD_PORT);
    local.sin_family        = AF_INET;
    local.sin_addr.s_addr   = INADDR_ANY;

    ret = bind(fd, (struct sockaddr *)&local, sizeof(local));
    if (ret < 0) {
        printf("failed to bind port %d\n", FTPD_PORT);
        close(fd);
        return;
    }

    listen(fd, FTPD_MAX_CONNECTION);

    buffer = (char *)malloc(FTPD_BUFFER_SIZE);
    if (buffer == NULL) {
        printf("failed to malloc recv buffer\n");
        close(fd);
        return;
    }

    for (;;) {
        FD_ZERO(&readfds);
        FD_ZERO(&errorfds);

        FD_SET(fd, &readfds);
        FD_SET(fd, &errorfds);

        max_fd  = fd;

        session = session_list;
        while (session != NULL) {
            if (max_fd < session->fd) {
                max_fd = session->fd;
            }

            FD_SET(session->fd, &readfds);
            FD_SET(session->fd, &errorfds);
            session = session->next;
        }

        if (select(max_fd + 1, &readfds, 0, &errorfds, 0) <= 0) {
            continue;
        }

        if (FD_ISSET(fd, &errorfds)) {
            break;
        }

        if (FD_ISSET(fd, &readfds)) {
            int new_fd;
            struct sockaddr_in remote;
            socklen_t addr_len = sizeof(struct sockaddr_in);

            new_fd = accept(fd, (struct sockaddr*)&remote, &addr_len);
            if (new_fd == -1) {
                printf("failed to accept connection\n");
                continue;
            } else {
                printf("Got a new connection from %s\n", inet_ntoa(remote.sin_addr));

                write(new_fd, FTPD_WELCOME_MSG, strlen(FTPD_WELCOME_MSG));

                session = ftpd_new_session();
                if (session != NULL) {
                    strcpy(session->cwd, FTPD_SRV_ROOT);
                    session->fd     = new_fd;
                    session->remote = remote;
                }
            }
        }

        session = session_list;
        while (session != NULL) {
            next = session->next;
            if (FD_ISSET(session->fd, &errorfds)) {
                printf("Client %s disconnected\n", inet_ntoa(session->remote.sin_addr));
                FD_CLR(session->fd, &readfds);
                close(session->fd);
                ftpd_close_session(session);
            } else if (FD_ISSET(session->fd, &readfds)) {
                len = read(session->fd, buffer, FTPD_BUFFER_SIZE - 1);
                if (len <= 0) {
                    printf("Client %s disconnected\n", inet_ntoa(session->remote.sin_addr));
                    FD_CLR(session->fd, &readfds);
                    close(session->fd);
                    ftpd_close_session(session);
                } else {
                    buffer[len] = '\0';
                    if (ftpd_do_request(session, buffer) == -1) {
                        printf("Client %s disconnected\r\n", inet_ntoa(session->remote.sin_addr));
                        FD_CLR(session->fd, &readfds);
                        close(session->fd);
                        ftpd_close_session(session);
                    }
                }
            }
            session = next;
        }
    }

    session = session_list;
    while (session != NULL) {
        next = session->next;
        close(session->fd);
        ftpd_close_session(session);
        session = next;
    }

    free(buffer);
    close(fd);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
