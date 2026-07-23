#include <stdio.h>
#include "mesh.h"
#include "texture.h"
#include "mesh.h"

int texture_width = 64;
int texture_height = 64;

void load_png_texture_data(char* filename, mesh_t *m) {
    upng_t *png_texture = upng_new_from_file(filename);
    if (png_texture != NULL) {
        upng_decode(png_texture);
        if (upng_get_error(png_texture) == UPNG_EOK) {
            m->mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
            m->texture_width = upng_get_width(png_texture);
            m->texture_height = upng_get_height(png_texture);
        }
    }
}

tex2_t tex2_clone(tex2_t* t) {
    tex2_t result = { t->u, t->v };
    return result;
}