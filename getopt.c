#include <stdlib.h>
#include <string.h>
#include "getopt.h"

static int _handle_opt(int argc, char *const argv[],
		const char *optstring, int *nextchar_ptr);
static int _getopt_long_core(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts,
		int *longindex, int long_only);
static int _handle_long_opt (int argc, char *const argv[],
		const struct option *longopts,
		int *longindex, int *nextchar);

char *optarg = NULL;
int optind = 1;

int 
getopt(int argc, char *const argv[], 
		const char *optstring) 
{
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
		nextchar = 1;
	}
	opt = _handle_opt(argc, argv, optstring, &nextchar);

	return opt;
}

static int
_handle_opt(int argc, char *const argv[],
		const char *optstring, int *nextchar_ptr)
{
	char *opt_ptr;
	int opt = 0;

	opt_ptr = strchr(optstring, argv[optind][*nextchar_ptr]);
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
	int opt;

	opt = _getopt_long_core(argc, argv, optstring, longopts, longindex, 0);

	return opt;
}

int getopt_long_only(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex)
{
	return 0;
}

static int
_getopt_long_core(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts,
		int *longindex, int long_only)
{
	static int nextchar = 0;
	int opt = 0;
	int is_long = 0;

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
					is_long = 1;
					nextchar = 2;
					break;
				}
				is_long = 0;
				nextchar = 1;
				break;
			}
		}
	}
	if (is_long) {
		opt = _handle_long_opt(argc, argv, longopts, longindex, &nextchar);
	} else {
		opt = _handle_opt(argc, argv, optstring, &nextchar);
	}

	return opt;
}

static int 
_handle_long_opt (int argc, char *const argv[],
		const struct option *longopts,
		int *longindex, int *nextchar)
{
	int opt;
	int i;
	int diff;

	optarg = NULL;
	for (i = 0;; ++i) {
		if (longopts[i].name == 0) {
			opt = '?';
			break;
		}
		diff = strcmp(argv[optind] + *nextchar, longopts[i].name);
		if (diff == 0) {
			*longindex = i;
			opt = longopts[i].val;
			if (longopts[i].has_arg == no_argument) {
				break;
			}
			if (optind + 1 >= argc || argv[optind + 1][0] == '-') {
				if (longopts[i].has_arg == required_argument) {
					opt = '?';
				}
				break;
			}
			optarg = argv[optind + 1];
			break;
		} else if (diff == '=') {
			*longindex = i;
			opt = longopts[i].val;
			if (longopts[i].has_arg == no_argument) {
				break;
			}
			while (argv[optind][*nextchar] != '=') {
				++(*nextchar);
			}
			++(*nextchar);
			if (argv[optind][*nextchar] == '\0') {
				if (longopts[i].has_arg == required_argument) {
					opt = '?';
				}
				break;
			}
			optarg = argv[optind] + *nextchar;
			break;
		}
	}
	++optind;
	*nextchar = 0;

	return opt;
}
