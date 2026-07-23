#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"
#include <stdbool.h>

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 faces with 2 triangles each

extern vec3_t cube_vertices[N_CUBE_VERTICES]; // Defined externally in ("in mesh.c")
extern face_t cube_faces[N_CUBE_FACES];

// Defines a struct for dynamic size meshes, with array of verticesa and faces
typedef struct {
  vec3_t* vertices;   // Dynamic array of vertices
  face_t* faces;      // Dynamic array of faces
  vec3_t rotation;    // rotation with x,y,z values (Euler angles)
  vec3_t scale;       // scale with x,y and z values
  vec3_t translation; // translation with x,y and z values
  uint32_t* mesh_texture;
  uint32_t texture_width;
  uint32_t texture_height ;
  bool exists;
} mesh_t;


void load_cube_mesh_data(void);
mesh_t *load_obj_file_data(char* filename);

extern mesh_t *meshes_array[256]; // If there are more than 256 models : why ?

#endif