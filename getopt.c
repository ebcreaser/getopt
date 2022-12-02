#include <stdlib.h>
#include <string.h>
#include "getopt.h"

static int _handle_opt(int argc, char *const argv[],
		const char *optstring);
static int _getopt_long_core(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts,
		int *longindex, int long_only);
static int _handle_long_opt(int argc, char *const argv[],
		const struct option *longopts,
		int *longindex);
static void _next_arg(char *const argv[]);
static void _permute(char *const argv[]);

char *optarg = NULL;
int optind = 1;
static int swapind = 0;
static int nextchar = 0;


int 
getopt(int argc, char *const argv[], 
		const char *optstring) 
{
	int opt = 0;

	if (nextchar == 0) {
		while (optind < argc) {
			if (argv[optind][0] == '-') {
				if (argv[optind][1] == '-') {
					_next_arg(argv);
				} else {
					nextchar = 1;
					break;
				}
			} else {
				if (swapind == 0) {
					swapind = optind;
				}
				++optind;
			}
		}
	}
	if (optind >= argc) {
		return -1;
	}
	opt = _handle_opt(argc, argv, optstring);

	return opt;
}

static int
_handle_opt(int argc, char *const argv[],
		const char *optstring)
{
	char *opt_ptr;
	int opt = 0;

	opt_ptr = strchr(optstring, argv[optind][nextchar]);
	optarg = NULL;
	++(nextchar);
	if (opt_ptr == NULL) {
		opt = '?';
	} else if (opt_ptr[1] != ':') {
		opt = *opt_ptr;
	} else {
		switch (argv[optind][nextchar]) {
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
			if (argv[optind][nextchar + 1] == '\0') {
				if (opt_ptr[2] != ':') {
					opt = '?';
				}
				break;
			}
			optarg = argv[optind] + nextchar + 1;
			break;
		default:
			optarg = argv[optind] + nextchar;
			break;
		}
	}
	if (argv[optind][nextchar] == '\0' && optarg != NULL) {
		_next_arg(argv);
		_next_arg(argv);
	} else if (argv[optind][nextchar] == '\0' || optarg != NULL) {
		_next_arg(argv);
	}
	if (opt != '?') {
		opt = *opt_ptr;
	}

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
	int opt;

	opt = _getopt_long_core(argc, argv, optstring, longopts, longindex, 1);

	return opt;
}

static int
_getopt_long_core(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts,
		int *longindex, int long_only)
{
	int opt = 0;

	*longindex = -1;
	if (nextchar == 0) {
		while (optind < argc) {
			if (argv[optind][0] == '-') {
				if (argv[optind][1] == '-') {
					nextchar = 2;
					break;
				} else {
					nextchar = 1;
					break;
				}
			} else {
				if (swapind == 0) {
					swapind = optind;
				}
				++optind;
			}
		}
	}
	if (optind >= argc) {
		return -1;
	}
	if (long_only && nextchar == 1) {
		opt = _handle_long_opt(argc, argv, longopts, longindex);
		if (nextchar > 0) {
			opt = _handle_opt(argc, argv, optstring);
		}
	} else if (argv[optind][1] == '-') {
		opt = _handle_long_opt(argc, argv, longopts, longindex);
	} else {
		opt = _handle_opt(argc, argv, optstring);
	}

	return opt;
}

static int 
_handle_long_opt (int argc, char *const argv[],
		const struct option *longopts,
		int *longindex)
{
	int opt;
	int diff;
	int i;
	int j = 1;

	optarg = NULL;
	for (i = 0;; ++i) {
		if (longopts[i].name == 0) {
			opt = '?';
			break;
		}
		diff = strcmp(argv[optind] + nextchar, longopts[i].name);
		if (diff == 0 || diff == '=') {
			break;
		}
	}
	if (diff == 0) {
		*longindex = i;
		opt = longopts[i].val;
		if (longopts[i].has_arg != no_argument) {
			if (optind + 1 >= argc || argv[optind + 1][0] == '-') {
				if (longopts[i].has_arg == required_argument) {
					opt = '?';
				}
			} else {
				nextchar = 0;
				optarg = argv[optind + 1];
			}
		}
	} else if (diff == '=') {
		*longindex = i;
		opt = longopts[i].val;
		if (longopts[i].has_arg != no_argument) {
			while (argv[optind][nextchar + j] != '=') {
				++j;
			}
			++j;
			if (argv[optind][nextchar + j] == '\0') {
				if (longopts[i].has_arg == required_argument) {
					opt = '?';
				}
			} else {
				nextchar += j;
				optarg = argv[optind] + nextchar;
			}
		}
	}
	if (nextchar == 0) {
		_next_arg(argv);
	}
	if (opt != '?' || nextchar != 1) {
		_next_arg(argv);
	}

	return opt;
}

static void _next_arg(char *const argv[])
{
	if (swapind > 0 && swapind != optind) {
		_permute(argv);
	}
	++optind;
	nextchar = 0;
}

static void _permute(char *const argv[])
{
	char **optarg_ptr;
	char **notoptarg_ptr;
	char *temp;

	optarg_ptr = (char **)(argv + optind);
	notoptarg_ptr = (char **)(argv + swapind);
	temp = *optarg_ptr;
	*optarg_ptr = *notoptarg_ptr;
	*notoptarg_ptr = temp;
	++(swapind);
}

