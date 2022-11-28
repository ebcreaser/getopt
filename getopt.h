#ifndef GETOPT_H
#define GETOPT_H

enum {
	no_argument = 0,
	required_argument,
	optional_argument
};

struct option {
	const char *name;
	int         has_arg;
	int        *flag;
	int         val;
};

extern char *optarg;
extern int optind, opterr, optopt;

int getopt(int argc, char *const argv[], 
		const char *optstring);
int getopt_long(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex);
int getopt_long_only(int argc, char *const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex);

#endif
