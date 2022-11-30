#include <stdlib.h>
#include <string.h>
#include "getopt.h"

static int _handle_opt(int argc, char *const argv[],
		const char *optstring,
		int *nextchar_ptr, int *swapind_ptr);
static int _getopt_long_core(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts,
		int *longindex, int long_only);
static int _handle_long_opt(int argc, char *const argv[],
		const struct option *longopts,
		int *longindex, int *nextchar_ptr,
		int *swapind_ptr, int long_only);
static void _next_arg(char *const argv[],
		int *swapind_ptr, int *nextchar_ptr);
static void _permute(char *const argv[], int *swapind_ptr);

char *optarg = NULL;
int optind = 1;

int 
getopt(int argc, char *const argv[], 
		const char *optstring) 
{
	static int nextchar = 0;
	static int swapind = 0;
	int opt = 0;

	while (nextchar == 0 && optind < argc) {
		if (argv[optind][0] == '-') {
			if (argv[optind][1] == '-') {
				_next_arg(argv, &swapind, &nextchar);
			} else if (argv[optind][1] != '\0') {
				nextchar = 1;
				break;
			}
		} else if (swapind == 0) {
			swapind = optind;
		}
		++optind;
	}
	if (optind >= argc) {
		return -1;
	}
	opt = _handle_opt(argc, argv, optstring, &nextchar, &swapind);


	return opt;
}

static int
_handle_opt(int argc, char *const argv[],
		const char *optstring,
		int *nextchar_ptr, int *swapind_ptr)
{
	char *opt_ptr;
	int opt = 0;

	opt_ptr = strchr(optstring, argv[optind][*nextchar_ptr]);
	optarg = NULL;
	++(*nextchar_ptr);
	if (opt_ptr == NULL) {
		opt = '?';
	} else if (opt_ptr[1] != ':') {
		opt = *opt_ptr;
	} else {
		switch (argv[optind][*nextchar_ptr]) {
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
			if (argv[optind][*nextchar_ptr + 1] == '\0') {
				if (opt_ptr[2] != ':') {
					opt = '?';
				}
				break;
			}
			optarg = argv[optind] + *nextchar_ptr + 1;
			break;
		default:
			optarg = argv[optind] + *nextchar_ptr;
			break;
		}
	}
	if (argv[optind][*nextchar_ptr] == '\0' && optarg != NULL) {
		_next_arg(argv, swapind_ptr, nextchar_ptr);
		_next_arg(argv, swapind_ptr, nextchar_ptr);
	} else if (argv[optind][*nextchar_ptr] == '\0' || optarg != NULL) {
		_next_arg(argv, swapind_ptr, nextchar_ptr);
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
	static int nextchar = 0;
	static int swapind = 0;
	int opt = 0;
	int is_long;

	*longindex = -1;
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
				is_long = long_only;
				nextchar = 1;
				break;
			}
			if (swapind == 0) {
				swapind = optind;
			}
		}
	}
	if (is_long) {
		opt = _handle_long_opt(argc, argv, longopts, longindex, &nextchar, &swapind, long_only);
		if (long_only && opt == '?') {
			opt = _handle_opt(argc, argv, optstring, &nextchar, &swapind);
		}
	} else {
		opt = _handle_opt(argc, argv, optstring, &nextchar, &swapind);
	}

	return opt;
}

static int 
_handle_long_opt (int argc, char *const argv[],
		const struct option *longopts,
		int *longindex, int *nextchar_ptr,
		int *swapind_ptr, int long_only)
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
		diff = strcmp(argv[optind] + *nextchar_ptr, longopts[i].name);
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
			*nextchar_ptr = 0;
			optarg = argv[optind + 1];
			break;
		} else if (diff == '=') {
			*longindex = i;
			opt = longopts[i].val;
			if (longopts[i].has_arg == no_argument) {
				break;
			}
			while (argv[optind][*nextchar_ptr + j] != '=') {
				++j;
			}
			++j;
			if (argv[optind][*nextchar_ptr + j] == '\0') {
				if (longopts[i].has_arg == required_argument) {
					opt = '?';
				}
				break;
			}
			*nextchar_ptr += j;
			optarg = argv[optind] + *nextchar_ptr;
			break;
		}
	}
	if (long_only && opt == '?') {
		*longindex = -1;
	} else {
		if (*nextchar_ptr == 0) {
			_next_arg(argv, swapind_ptr, nextchar_ptr);
		}
		_next_arg(argv, swapind_ptr, nextchar_ptr);
	}

	return opt;
}

static void _next_arg(char *const argv[],
		int *swapind_ptr, int *nextchar_ptr)
{
	if (*swapind_ptr > 0) {
		_permute(argv, swapind_ptr);
	}
	++optind;
	*nextchar_ptr = 0;
}

static void _permute(char *const argv[], int *swapind_ptr)
{
	char **optarg_ptr;
	char **notoptarg_ptr;
	char *temp;

	optarg_ptr = (char **)(argv + optind);
	notoptarg_ptr = (char **)(argv + *swapind_ptr);
	temp = *optarg_ptr;
	*optarg_ptr = *notoptarg_ptr;
	*notoptarg_ptr = temp;
	++(*swapind_ptr);
}

