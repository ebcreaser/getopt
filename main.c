#include <stdio.h>
#include "getopt.h"

int main(int argc, char *argv[]) {
	const char *optstring = "ab::c:";
	int opt = 0;

	opt = getopt(argc, argv, optstring);
	while (opt != -1) {
		printf("%c\n", opt);
		if (optarg != NULL) {
			printf("\t%s\n", optarg);
		}
		opt = getopt(argc, argv, optstring);
	}

	return 0;
}
