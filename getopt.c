#include <stdlib.h>
#include <string.h>
#include "getopt.h"

static int _handle_opt(int argc, char *const argv[],
		const char *opt_ptr, int *nextchar_ptr);

char *optarg = NULL;
int optind = 1;

int 
getopt(int argc, char *const argv[], 
		const char *optstring) 
{
	char *opt_ptr;
	static int nextchar = 0;
	int opt = 0;

	if (optind < argc && argv[optind][nextchar] == '\0') {
		++optind;
		nextchar = 0;
	}
	if (nextchar == 0) {
		for (;; ++optind) {
			if (optind >= argc) {
				return -1;
			}
			if (argv[optind][0] == '-') {
				if (argv[optind][1] == '-') {
					continue;
				}
				break;
			}
		}
		++nextchar;
	}
	opt_ptr = strchr(optstring, argv[optind][nextchar]);
	opt = _handle_opt(argc, argv, opt_ptr, &nextchar);

	return opt;
}

static int
_handle_opt(int argc, char *const argv[],
		const char *opt_ptr, int *nextchar_ptr)
{
	int opt = 0;

	optarg = NULL;
	if (opt_ptr == NULL) {
		++(*nextchar_ptr);
		return '?';
	}
	if (opt_ptr[1] != ':') {
		++(*nextchar_ptr);
		return *opt_ptr;
	}
	switch (argv[optind][*nextchar_ptr + 1]) {
	case '\0':
		if (optind + 1 >= argc || argv[optind + 1][0] == '-') {
			if (opt_ptr[2] != ':') {
				opt = '?';
			}
			break;
		}
		optarg = argv[optind + 1];
		break;
	case '=':
		if (argv[optind][*nextchar_ptr + 2] == '\0') {
			if (opt_ptr[2] != ':') {
				opt = '?';
			}
			break;
		}
		optarg = argv[optind] + *nextchar_ptr + 2;
		break;
	default:
		optarg = argv[optind] + *nextchar_ptr + 1;
		break;
	}
	if (opt != '?') {
		opt = *opt_ptr;
	}
	++optind;
	*nextchar_ptr = 0;

	return opt;
}

int getopt_long(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex)
{
	int opt = 0;

	return opt;
}

int getopt_long_only(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex)
{
	return 0;
}
