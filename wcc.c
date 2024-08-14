#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern char *__progname;

static void usage(void);

enum states { WHITESPACE, WORD };

int main(int argc, char *argv[])
{
	int opt, exitval, c;
	int count = 0;
	int total_lines = 0;
	int total_words = 0;
	int total_bytes = 0;
	enum states state;

	int lflag = 0;
	int wflag = 0;
	int cflag = 0;

	while ((opt = getopt(argc, argv, "lwc")) != -1) {
		switch (opt) {
		case 'l':
			lflag = 1;
			break;
		case 'w':
			wflag = 1;
			break;
		case 'c':
			cflag = 1;
			break;
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	if (*argv == NULL)
		usage();

	for (exitval = 0; *argv != NULL; ++argv) {
		count++;
		int bytes = 0;
		int words = 0;
		int lines = 0;

		FILE *f = fopen(*argv, "r");
		if (f == NULL) {
			perror("fopen");
			exit(1);
		}

		while ((c = fgetc(f)) != EOF) {
			if (c == ' ' || c == '\t') {
				state = WHITESPACE;
			} else if (c == '\n') {
				lines++;
				state = WHITESPACE;
			} else {
				if (state == WHITESPACE) {
					words++;
				}
				state = WORD;
			}
			bytes++;
		}

		if (lflag) {
			fprintf(stdout, "%8d %s\n", lines, *argv);
		} else if (wflag) {
			fprintf(stdout, "%8d %s\n", words, *argv);
		} else if (cflag) {
			fprintf(stdout, "%8d %s\n", bytes, *argv);
		} else {
			fprintf(stdout, "%8d%8d%8d %s\n", lines, words, bytes,
				*argv);
		}

		total_bytes += bytes;
		total_lines += lines;
		total_words += words;
	}

	if (count > 1) {
		if (lflag) {
			fprintf(stdout, "%8d total\n", total_lines);
		} else if (wflag) {
			fprintf(stdout, "%8d total\n", total_words);
		} else if (cflag) {
			fprintf(stdout, "%8d total\n", total_bytes);
		} else {
			fprintf(stdout, "%8d%8d%8d total\n", total_lines,
				total_words, total_bytes);
		}
	}

	return exitval;
}

static void usage(void)
{
	fprintf(stderr, "usage: %s [-lcw] [file ...]\n", __progname);
	exit(1);
}
