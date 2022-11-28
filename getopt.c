#include <stdlib.h>
#include <string.h>
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
	static int nextchar = 0;
	int opt = 0;
	int i;
	int cont = 1;

	while (cont) {
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
			if (nextchar != 0) {
				++nextchar;
				opt = '?';
			}
			++nextchar;
			if (argv[optind][nextchar] != '-') {
				break;
			}
			++nextchar;
			for (i = 0;; ++i) {
				if (longopts[i].name == NULL) {
					opt = '?';
					break;
				}
				if (strcmp(argv[optind] + nextchar, longopts[i].name) != 0) {
					continue;
				}
				opt = longopts[i].val;
				*longindex = i;
				if (longopts[i].has_arg == no_argument) {
					break;
				}
				do {
					++nextchar;
				} while (argv[optind][nextchar] != '=' && argv[optind][nextchar] != '\0');
				switch (argv[optind][nextchar]) {
				case '=':
					++nextchar;
					if (argv[optind][nextchar] == '\0' && longopts[i].has_arg == required_argument) {
						opt = '?';
					} else {
						optarg = argv[optind] + nextchar;
					}
					break;
				case '\0':
					if ((optind + 1 >= argc || argv[optind + 1][0] == '-') 
							&& longopts[i].has_arg == required_argument) {
						opt = '?';
					} else {
						optarg = argv[optind + 1];
					}
					break;
				}
				break;
			}
			++optind;
			nextchar = 0;
			cont = 0;
			break;
		case ':':
		case ';':
			opt = '?';
			++nextchar;
			break;
		default:
			if (nextchar == 0) {
				++optind;
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
			cont = 0;
			break;
		}
		if (opt == '?') {
			break;
		}
	}

	return opt;
}

int getopt_long_only(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex)
{
	return 0;
}
