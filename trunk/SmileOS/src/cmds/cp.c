/*
 * Copyright (c) 1988, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * David Hitz of Auspex Systems Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* modified by JiaoJinXing for SmileOS. */
#include "cmds/cmds.h"

#include <sys/stat.h>
#include <sys/fcntl.h>

#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>

static void usage(void);
static int copy(char *from, char *to, int dirflag);


static char *iobuf;

int
cp_main(int argc, char *argv[])
{
	int r, ch, checkch, i, iflag = 0;
	char *target;
	struct stat to_stat, tmp_stat;

    optind = 0;

	while ((ch = getopt(argc, argv, "")) != -1)
		switch(ch) {
		case '?':
		default:
			usage();
			return -1;
		}
	argc -= optind;
	argv += optind;

	if (argc < 2) {
		usage();
		return -1;
	}

	if ((iobuf = malloc(BUFSIZ)) == NULL) {
	    return -1;
	}

	target = argv[--argc];

	r = stat(target, &to_stat);
	if (r == -1 && errno != ENOENT) {
	    return -1;
	}

	if (r == -1 || !S_ISDIR(to_stat.st_mode)) {
		/*
		 * File to file
		 */
		if (argc > 1) {
			usage();
			return -1;
		}

		if (stat(argv[0], &tmp_stat) == -1) {
			warn("%s", argv[0]);
			return -1;
		}
		if (!S_ISREG(tmp_stat.st_mode)) {
			usage();
			return -1;
		}

		/* interactive mode */
		if (r != -1 && iflag) {
			(void)fprintf(stderr, "overwrite %s? ", target);
			checkch = ch = getchar();
			while (ch != '\n' && ch != EOF)
				ch = getchar();
			if (checkch != 'y')
			    return 0;
		}
		r = copy(argv[0], target, 0);
	} else {
		/*
		 * File(s) to directory
		 */
		r = 0;
		for (i = 0; i < argc; i++)
			r = copy(argv[i], target, 1);
	}
	free(iobuf);
	return -r;
}

static void
usage(void)
{
	fprintf(stderr, "usage: cp src target\n"
		"       cp src1 ... srcN directory\n");
	/* NOTREACHED */
}

static int
copy(char *from, char *to, int dirflag)
{
	char path[PATH_MAX];
	int fold, fnew, n;
	struct stat stbuf;
	mode_t mode;
	char *p;

	if (dirflag) {
		p = strrchr(from, '/');
		p = p ? p + 1 : from;
		strlcpy(path, to, sizeof(path));
		if (strcmp(to, "/"))
			strlcat(path, "/", sizeof(path));
		strlcat(path, p, sizeof(path));
		to = path;
	}

	if ((fold = open(from, O_RDONLY)) == -1) {
		warn("%s", from);
		return 1;
	}
	fstat(fold, &stbuf);
	mode = stbuf.st_mode;

	if ((fnew = creat(to, mode)) == -1) {
		warn("%s", to);
		close(fold);
		return 1;
	}
	while ((n = read(fold, iobuf, BUFSIZ)) > 0) {
		if (write(fnew, iobuf, (size_t)n) != n) {
			warn("%s", to);
			close(fold);
			close(fnew);
			return 1;
		}
	}
	close(fold);
	close(fnew);
	return 0;
}

SMILEOS_CMD(
    cp,    cp_main,
    "bmp     - manipulate BMP image data\n",
    "info <imageAddr>          - display image info\n"
    "bmp display <imageAddr> [x y] - display image at x,y\n"
);

