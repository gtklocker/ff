#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ff.h"

struct dd_operands OPERANDS;

int is_valid_operand(const char *arg)
{
	return strchr(arg, '=') != NULL;
}

void init_operands()
{
	/* Default to STDIN/STDOUT. */
	strcpy(OPERANDS.dd_if, "");
	strcpy(OPERANDS.dd_of, "");

	/* Copy the whole file by default. */
	OPERANDS.dd_count = -1;
}

void parse_operands(int argc, const char *argv[])
{
	int i;
	char *option, *setting;

	for (i = 1; i < argc; ++i) {
		if(!is_valid_operand(argv[i])) {
			continue;
		}

		option = strtok((char *)argv[i], "=");
		setting = strtok(NULL, "=");

		#ifndef NDEBUG
		printf("setting %s to %s\n", option, setting);
		#endif

		if (strcmp(option, "if") == 0) {
			strncpy(OPERANDS.dd_if, setting, PATH_MAX);
		}
		if (strcmp(option, "of") == 0) {
			strncpy(OPERANDS.dd_of, setting, PATH_MAX);
		}
		if (strcmp(option, "count") == 0) {
			OPERANDS.dd_count = atoi(setting);
		}
	}

	#ifndef NDEBUG
	printf("input file: \t%s\n", OPERANDS.dd_if);
	printf("output file: \t%s\n", OPERANDS.dd_of);
	printf("count: \t\t%d\n", OPERANDS.dd_count);
	#endif
}
