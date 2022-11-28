#include <stdlib.h>
#include "getopt.h"

char *optarg = NULL;
int optind = 1;

int 
getopt(int argc, char *const argv[], 
		const char *optstring) 
{
	static int nextchar = 0;
	int opt = 0;
	int i;

	while (opt == 0) {
		if (optind == argc) {
			opt = -1;
			break;
		}
		switch (argv[optind][nextchar]) {
		case '\0':
			++optind;
			nextchar = 0;
			break;
		case '-':
			if (nextchar == 0) {
				++nextchar;
			} else {
				++optind;
				nextchar = 0;
			}
			break;
		case ':':
		case ';':
			opt = '?';
			++nextchar;
			break;
		default:
			if (nextchar == 0) {
				opt = -1;
				break;
			}
			optarg = NULL;
			for (i = 0; opt == 0; ++i) {
				if (optstring[i] == '\0') {
					opt = '?';
					++nextchar;
					break;
				}
				if (argv[optind][nextchar] != optstring[i]) {
					continue;
				}
				opt = argv[optind][nextchar];
				++nextchar;
				if (optstring[i + 1] != ':') {
					break;
				}
				switch (argv[optind][nextchar]) {
				case '\0':
					if (optind + 1 < argc && argv[optind + 1][0] != '-') {
						optarg = argv[optind + 1];
						optind += 2;
						nextchar = 0;
					} else if (optstring[i + 2] != ':' ) {
						opt = '?';
					}
					break;
				case '=':
					++nextchar;
					if (argv[optind][nextchar] != '\0') {
						optarg = argv[optind] + nextchar;
					} else if (optstring[i + 2] != ':' ) {
						opt = '?';
					}
					++optind;
					nextchar = 0;
					break;
				default:
					optarg = argv[optind] + nextchar;
					++optind;
					nextchar = 0;
					break;
				}
			}
			break;
		}
	}

	return opt;
}

int getopt_long(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex)
{
	return 0;
}

int getopt_long_only(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex)
{
	return 0;
}
