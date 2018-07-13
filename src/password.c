#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "password.h"

int16_t *imgarc_pass_to_sequence(char *password)
{
	int8_t cv;
	int16_t *sequence = malloc(1024);
	int progress = 0;
	memset(sequence, -1, 1024);

	size_t pass_length = strlen(password);
	for (size_t i=0; i<pass_length; ++i) {
		cv = password[i];
		progress = imgarc_conv_b10_b3(cv, progress, sequence);
	}

	return sequence;
}

int imgarc_conv_b10_b3(int8_t n, int progress, int16_t *sequence)
{
	while (n != 0) {
		sequence[progress] = n % 3;
		n = n / 3;
		++progress;
	}
	return progress;
}
