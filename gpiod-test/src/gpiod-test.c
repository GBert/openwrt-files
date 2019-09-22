// ---------------------------------------------------------------------
// low-level-gpio.c
//
//   This is an example of GPIO access with the libGPIOd library.
//
// Copyright 2018 Christophe BLAESS (christophe@blaess.fr)
// License GPL v2.
// ---------------------------------------------------------------------

#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	struct gpiod_chip *output_chip;
	struct gpiod_line *output_line;
	int output_offset;

	if (argc != 3)  {
		fprintf(stderr, "usage: %s <output_device> <output-offset>\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	if (sscanf(argv[2], "%d", &output_offset) != 1) {
		fprintf(stderr, "%s: invalid <output-offset> value.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	output_chip = gpiod_chip_open_lookup(argv[1]);
	if (output_chip == NULL) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	output_line = gpiod_chip_get_line(output_chip, output_offset);
	if (output_line == NULL) {
		perror(argv[2]);
		exit(EXIT_FAILURE);
	}

	if (gpiod_line_request_output(output_line, argv[0], 0) < 0) {
		perror(argv[2]);
		exit(EXIT_FAILURE);
	}

	for (;;) {
		gpiod_line_set_value(output_line, 0);
		gpiod_line_set_value(output_line, 1);
	}
	gpiod_line_release(output_line);

	return EXIT_FAILURE;
}

