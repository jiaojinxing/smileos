/*
 * Copyright (c) 1987, 1993
 *	The Regents of the University of California.  All rights reserved.
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char	**environ;

static void usage(void);

/*
 * printenv
 *
 * Bill Joy, UCB
 * February, 1979
 */
int
printenv_main(int argc, char *argv[])
{
	char *cp, **ep;
	size_t len;
	int ch;

    optind = 0;

	while ((ch = getopt(argc, argv, "")) != EOF)
		switch(ch) {
		case '?':
		default:
			usage();
			return -1;
		}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		for (ep = environ; *ep; ep++) {
			(void)printf("%s\n", *ep);
		}
		return 0;
	}

	len = strlen(*argv);
	for (ep = environ; *ep; ep++)
		if (!memcmp(*ep, *argv, len)) {
			cp = *ep + len;
			if (!*cp || *cp == '=') {
				(void)printf("%s\n", *cp ? cp + 1 : cp);
				return 0;
			}
		}
	return -1;
}

static void
usage(void)
{
	(void)fprintf(stderr, "usage: printenv [name]\n");
}

SMILEOS_CMD(
    printenv,    printenv_main,
    "bmp     - manipulate BMP image data\n",
    "info <imageAddr>          - display image info\n"
    "bmp display <imageAddr> [x y] - display image at x,y\n"
);
