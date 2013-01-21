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

static void usage(const char *);

int
main(int argc, const char *argv[])
{
	const char *error_format = "Failed to open %s";
	char error_string[PATH_MAX + strlen(error_format)];
	char input_file[PATH_MAX], output_file[PATH_MAX];
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

	if (argc < 3) {
		fprintf(stderr, "Not enough arguments given.\n");
		usage(argv[0]);
	}

	strcpy(input_file, argv[1]);
	strcpy(output_file, argv[2]);

	/* Open the files.  */
	input = open(input_file, O_RDONLY);
	if (input < 0) {
		sprintf(error_string, error_format, input_file);
		perror(error_string);
		exit(EXIT_FAILURE);
	}

	output = open(output_file, O_WRONLY | O_TRUNC | O_APPEND);
	if (output < 0) {
		sprintf(error_string, error_format, output_file);
		perror(error_string);
		exit(EXIT_FAILURE);
	}

	/* Get file size.  */
	fstat(input, &stat_buffer);
	input_size = stat_buffer.st_size;

	/* Copy the file.  */
	for (bytes_read = 0; (last_read = read(input, buffer, BUFFER_SIZE)); bytes_read += last_read) {
		write(output, buffer, last_read);

		/* Update progress.  */
		fprintf(stdout, "\rProgress: %d%%", (int)((float)(bytes_read + last_read) / input_size * 100));
		fflush(stdout);
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
