#include "mesh.h"
#include "array.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

mesh_t *load_obj_file_data(char* filename) {
    FILE* file;
    file = fopen(filename, "r");
    if (!file) SDL_Log("file does not exist");
    char line[1024];

    mesh_t baseobj = {
        .vertices = NULL,
        .faces = NULL,
        .rotation = {.x = 0, .y = 0, .z = 0},
        .scale = {.x = 1.0, .y = 1.0, .z = 1.0},
        .translation = {.x = 0, .y = 0, .z = 0},
        .mesh_texture = NULL,
        .texture_width = 0,
        .texture_height = 0,
        .exists = true
    };

    tex2_t* texcoords = NULL;

    while (fgets(line, 1024, file)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(baseobj.vertices, vertex);
        }
        // Texture coordinate information
        if (strncmp(line, "vt ", 3) == 0) {
            tex2_t texcoord;
            sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
            array_push(texcoords, texcoord);
        }
        // Face information
        if (strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );
            face_t face = {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2],
                .a_uv = texcoords[texture_indices[0] - 1],
                .b_uv = texcoords[texture_indices[1] - 1],
                .c_uv = texcoords[texture_indices[2] - 1],
                .color = 0xFFFFFFFF
            };
            //array_push(mesh.faces, face);
            array_push(baseobj.faces, face);
        }
    }
    array_free(texcoords);

    // What the fuck am I doing
    mesh_t *obj = malloc(sizeof(mesh_t));
    memcpy(obj, &baseobj, sizeof(mesh_t));
    return obj;
}

mesh_t *meshes_array[256]; // limit to 256 models at once

void init_ma() {
    for (size_t i = 0; i < 256; i++) {
    //    meshes_array[i] = malloc(sizeof(mesh_t));
    //    memset(meshes_array[i], 0, sizeof(mesh_t));
    //    meshes_array[i]->exists = false;

        meshes_array[i] = NULL;
    }
}

void add_to_ma(mesh_t *m) {
    
    for (size_t i = 0; i < 256; i++) {
        SDL_Log("a");
   //   if (!meshes_array[i]->exists) {
        if (!meshes_array[i]) {
            meshes_array[i] = m;
            SDL_Log("ok");
            return;
        }
    }
    SDL_Log("bruh");
}