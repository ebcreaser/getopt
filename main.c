#include <stdio.h>
#include "getopt.h"

int main(int argc, char *argv[]) {
	const char *optstring = "ab::c:";
	int opt = 0;
	int longindex = 0;
	struct option long_options[] = {
		{"add",     required_argument, 0,  0 },
		{"append",  no_argument,       0,  0 },
		{"delete",  required_argument, 0,  0 },
		{"verbose", no_argument,       0,  0 },
		{"create",  required_argument, 0, 'c'},
		{"file",    required_argument, 0,  0 },
		{0,         0,                 0,  0 }
    };

	opt = getopt_long(argc, argv, optstring, long_options, &longindex);
	while (opt != -1) {
		printf("%c\n", opt);
		if (optarg != NULL) {
			printf("\t%s\n", optarg);
		}
		opt = getopt_long(argc, argv, optstring, long_options, &longindex);
	}

	return 0;
}
