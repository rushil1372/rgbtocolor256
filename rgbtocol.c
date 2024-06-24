#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// palette check function 
bool color_in_palette(unsigned char *palette, int palette_size, int color_index, unsigned char *image_buf) {
    for (int i = 0; i < palette_size; i++) {
        if (palette[i * 3] == image_buf[color_index] &&
            palette[i * 3 + 1] == image_buf[color_index + 1] &&
            palette[i * 3 + 2] == image_buf[color_index + 2]) {
            return true;
        }
    }
    return false;
}

// palette-color index grab function 
int get_color_index(unsigned char *palette, int palette_size, int color_index, unsigned char *image_buf) {
    for (int i = 0; i < palette_size; i++) {
        if (palette[i * 3] == image_buf[color_index] &&
            palette[i * 3 + 1] == image_buf[color_index + 1] &&
            palette[i * 3 + 2] == image_buf[color_index + 2]) {
            return i;
        }
    }
    return -1;
}

unsigned char *image_rgb_to_color256(unsigned int dim_x, unsigned int dim_y, unsigned char *image_buf) {
    unsigned int total_pixels = dim_x * dim_y;
    unsigned char palette[256 * 3];
    int palette_size = 0;

    unsigned char *color_indices = (unsigned char *)malloc(total_pixels);

    // populating palette
    for (unsigned int i = 0; i < total_pixels; i++) {
        int color_index = i * 3;

        // palette check 
        if (!color_in_palette(palette, palette_size, color_index, image_buf)) {
            if (palette_size >= 256) {
                free(color_indices);
                return NULL;
            }
            
            palette[palette_size * 3] = image_buf[color_index];
            palette[palette_size * 3 + 1] = image_buf[color_index + 1];
            palette[palette_size * 3 + 2] = image_buf[color_index + 2];
            palette_size++;
        }

        // grabbing index from palette
        int palette_index = get_color_index(palette, palette_size, color_index, image_buf);
        color_indices[i] = palette_index;
    }

    // total output size
    int output_size = (256 * 3) + total_pixels;
    unsigned char *output_buf = (unsigned char *)malloc(output_size);

    // palette copy
    for (int i = 0; i < palette_size; i++) {
        output_buf[i * 3] = palette[i * 3];
        output_buf[i * 3 + 1] = palette[i * 3 + 1];
        output_buf[i * 3 + 2] = palette[i * 3 + 2];
    }

    // 0 fill 
    for (int i = palette_size; i < 256; i++) {
        output_buf[i * 3] = 0;
        output_buf[i * 3 + 1] = 0;
        output_buf[i * 3 + 2] = 0;
    }

    // index copy 
    for (unsigned int i = 0; i < total_pixels; i++) {
        output_buf[(256 * 3) + i] = color_indices[i];
    }

    free(color_indices);
    return output_buf;
}

int main() {
    unsigned int dim_x = 1;
    unsigned int dim_y = 3;
    unsigned char image_rgb[] = {255, 0, 0, 0, 255, 0, 0, 0, 255};

    unsigned char *image_color256 = image_rgb_to_color256(dim_x, dim_y, image_rgb);

    if (image_color256 != NULL) {
        for (int i = 0; i < (256 * 3) + (dim_x * dim_y); i++) {
            printf("%d ", image_color256[i]);
        }

        free(image_color256);
    } else {
        return -1;
    }

    return 0;
}
