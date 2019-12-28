#include "file_io.h"
#include "image.h"

#define IMGARC_MAX_PASS_LEN 256

void imgarc_io_print_file_meta(const imgarc_file *fd);
void imgarc_io_print_sequence(const int16_t *sequence);
void imgarc_io_print_image_png_meta(imgarc_image *img);
void imgarc_io_get_input(char *storage, int max_size);