
#include <sys/stat.h>
#include <sys/fcntl.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void do_cat(int rfd);

static char *stdbuf;
static char *filename;
static int rval;

int
cat_main(int argc, char *argv[])
{
	int ch, fd;

    optind = 0;

	while ((ch = getopt(argc, argv, "")) != -1)
		switch(ch) {
		case '?':
		default:
			printf("usage: cat [-] [file ...]\n");
            return 0;
			/* NOTREACHED */
		}
	argv += optind;

	if ((stdbuf = malloc(BUFSIZ)) == NULL) {
        return -1;
	}

	fd = fileno(stdin);
	filename = "stdin";
	do {
		if (*argv) {
			if (!strcmp(*argv, "-"))
				fd = fileno(stdin);
			else if ((fd = open(*argv, O_RDONLY, 0)) < 0) {
				++argv;
				rval = 1;
				continue;
			}
			filename = *argv++;
		}
		do_cat(fd);
		if (fd != fileno(stdin))
			(void)close(fd);
	} while (*argv);
	free(stdbuf);
	return rval;
}

static void
do_cat(int rfd)
{
	int nr, nw, off, wfd;
	struct stat sbuf;

	wfd = fileno(stdout);
	if (fstat(wfd, &sbuf))
	    ;

	while ((nr = read(rfd, stdbuf, BUFSIZ)) > 0)
		for (off = 0; nr; nr -= nw, off += nw)
			if ((nw = write(wfd, stdbuf + off, (size_t)nr)) < 0)
			    ;
	if (nr < 0) {
		rval = 1;
	}
}
