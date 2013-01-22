/*
 * minidd - minimalistic clone of dd, supports copy operations
 * Copyright 2013 Kostis Karantias
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "minidd.h"

static void usage(const char *);

int
main(int argc, const char *argv[])
{
	const char *error_format = "Failed to open %s";
	char error_string[PATH_MAX + strlen(error_format)];
	int input, output;

	/*
	 * 1MB per read is a sane value for most computers.  Configure this to your
	 * liking.  
	 */
	const int BUFFER_SIZE = 1024 * 1024;
	char buffer[BUFFER_SIZE];

	size_t last_read;	/* Bytes read on last read() execution.  */
	int bytes_read;		/* Total number of bytes read.  */

	struct stat stat_buffer;
	int input_size;

	init_operands();
	parse_operands(argc, argv);

	/* Open the files.  */
	if (strlen(OPERANDS.dd_if) == 0) {
		input = STDIN_FILENO;
	}
	else {
		input = open(OPERANDS.dd_if, O_RDONLY);
		if (input < 0) {
			sprintf(error_string, error_format, OPERANDS.dd_if);
			perror(error_string);
			exit(EXIT_FAILURE);
		}
	}

	if (strlen(OPERANDS.dd_of) == 0) {
		output = STDOUT_FILENO;
	}
	else {
		output = open(OPERANDS.dd_of, O_WRONLY | O_TRUNC | O_APPEND);
		if (output < 0) {
			sprintf(error_string, error_format, OPERANDS.dd_of);
			perror(error_string);
			exit(EXIT_FAILURE);
		}
	}

	/* Get file size.  */
	fstat(input, &stat_buffer);
	input_size = stat_buffer.st_size;

	/* Copy the file.  */
	bytes_read = 0;
	while ((last_read = read(input, buffer, BUFFER_SIZE))) {
		/* Check if read is successful/device still intact.  */
		if (last_read < 0) {
			perror("Read failed");

			close(output);
			exit(EXIT_FAILURE);
		}

		/* If everything goes right.  */
		bytes_read += last_read;
		if (write(output, buffer, last_read) < 0) {
			perror("Write failed");

			close(input);
			exit(EXIT_FAILURE);
		}
	}
	putchar('\n');

	close(input);
	close(output);

	exit(EXIT_SUCCESS);
}

static void
usage(const char *name)
{
	fprintf(stderr, "usage: %s SOURCE DEST\n", name);
	exit(EXIT_FAILURE);
}

/* vim: set noexpandtab */
