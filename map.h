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

#define COLLISION_TYPE_BOX 0
#define COLLISION_TYPE_CYLINDER 1

#define COLLISION_TYPE_PASSTHROUGHT 2

#define COLLISION_TYPE_WALKFUNC 4
#define COLLISION_TYPE_INTERACTFUNC 8

typedef struct {
    uint8_t type;
    vec3_t base;
    float diameter;
    vec3_t end;
    void (*w_pointer)();
    void (*i_pointer)();
} collision_t;


typedef struct {
    collision_t *collide_zones;
    // rajouter des pnj plus tard
    // rajouter la mouzica plus tard
} map_t;

extern mesh_t *current_map_model;
extern map_t current_map;

void init_map();
void load_map(collision_t *col, char *mapname);
