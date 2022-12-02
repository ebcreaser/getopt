#include <stdio.h>
#include "getopt.h"

int main(int argc, char *argv[]) {
	extern char *optarg;
	static struct option long_options[] = {
		{"add",     required_argument, 0,  0 },
		{"append",  no_argument,       0,  0 },
		{"delete",  required_argument, 0,  0 },
		{"verbose", no_argument,       0,  0 },
		{"create",  required_argument, 0, 'c'},
		{"file",    required_argument, 0,  0 },
		{0,         0,                 0,  0 }
	};
	const char *optstring = "ab::c:";
	int opt = 0;
	int longindex;
	int i;

	opt = getopt_long_only(argc, argv, optstring, long_options, &longindex);
	while (opt != -1) {
		if (longindex == -1) {
			printf("%c\n", opt);
		} else {
			printf("%c (--%s)\n", opt, long_options[longindex].name);
		}
		if (optarg != NULL) {
			printf("\t%s\n", optarg);
		}
		opt = getopt_long_only(argc, argv, optstring, long_options, &longindex);
	}
	for (i = 0; i < argc; ++i) {
		printf("%s\n", argv[i]);
	}

	return 0;
}
