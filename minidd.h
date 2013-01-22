#include <limits.h>

struct dd_operands {
	char	dd_if[PATH_MAX];	/* input file */
	char	dd_of[PATH_MAX];	/* output file */
	int		dd_count;			/* count of writes */
};

void parse_operands(int, const char **);
void init_operands();

struct dd_operands OPERANDS;
