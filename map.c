#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "upng.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "triangle.h"
#include "camera.h"
#include "clipping.h"

#include "map.h"

map_t current_map;
mesh_t *current_map_model;

void init_map() {
    current_map_model = load_obj_file_data("./assets/cube.obj");
    current_map_model->translation = (vec3_t){.x = 0.0f, .y = 0.0f, .z = 0.0f};
    current_map_model->scale = (vec3_t){0.5f,0.5f,0.5f};
    load_png_texture_data("./assets/p.png", current_map_model);
    add_to_ma(current_map_model); 
    current_map_model->exists = true;
}


void load_map(collision_t *col, char *mapname) {

    // map
    current_map_model->exists = false; 

    char *mapfilepath = malloc(256);
    snprintf(mapfilepath, 256, "%s%s", "./assets/maps/", mapname);
    strcat(mapfilepath, ".obj");
    SDL_Log(mapfilepath);
    char *maptexpath = malloc(256);
    snprintf(maptexpath, 256, "%s%s", "./assets/maps/", mapname);
    strcat(maptexpath, ".png");
    SDL_Log(maptexpath);
    current_map_model = load_obj_file_data(mapfilepath);
    SDL_Log(maptexpath);
    load_png_texture_data(maptexpath, current_map_model);
    SDL_Log(maptexpath);

    // la mémoire c bien
    free(mapfilepath);
    free(maptexpath);

    // colisions
    current_map.collide_zones = col;

    // autre trucs
    current_map_model->translation = (vec3_t){.x = 0.0f, .y = 0.0f, .z = 0.0f};
    current_map_model->scale = (vec3_t){1.0f,1.5f,1.5f};
    current_map_model->rotation.x = 1;

    //add_to_ma(current_map_model); 
    current_map_model->exists = true; 
}