#define SDL_MAIN_HANDLED

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

#include "player.h"

// Array of triangles that should be rendere frame by frame
#define MAX_TRIANGLES 10000
triangle_t triangles_to_render[MAX_TRIANGLES];
int num_triangles_to_render = 0; // Used to move away from the dynamic allocation of the triangles_to_render array

mat4_t proj_matrix;
mat4_t view_matrix;
mat4_t world_matrix;

bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0;

bool CULL_BACKFACE = false;
bool RENDER_WIREFRAME = false;
bool RENDER_FILL = true;
bool RENDER_VERTICES = false;
bool RENDER_TEXTURED = false;


void setup(void){
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) *  window_width * window_height);
  z_buffer = (float*) malloc(sizeof(float) * window_width * window_height);

  // Creating an SDL texture that is used to display the color_buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  // Initialize the perspective projection matrix
  float aspect_y = (float)window_height / (float)window_width;
  float aspect_x = (float)window_width / (float)window_height;
  float fov_y = 3.141592 / 3.0; // the same as 180/3, or 60deg
  float fov_x = atan(tan(fov_y / 2) * aspect_x) * 2;
  float z_near = 1.0;
  float z_far = 20.0;


  // Initialize frustum planes with a point and a normal
  init_frustum_planes(fov_x, fov_y, z_near, z_far);

  proj_matrix = mat4_make_projection(fov_y, aspect_x, z_near, z_far);

}

void handle_key_press(SDL_Keycode keycode) {
  switch(keycode){
    case SDLK_ESCAPE:
      is_running = false;
      break;
    /* case SDLK_1:
      CULL_BACKFACE = !CULL_BACKFACE;
      break;
    case SDLK_2:
      RENDER_WIREFRAME = !RENDER_WIREFRAME;
      break;
    case SDLK_3:
      RENDER_FILL = !RENDER_FILL;
      break;
    case SDLK_4:
      RENDER_VERTICES = !RENDER_VERTICES;
      break;
    case SDLK_5:
      RENDER_TEXTURED = !RENDER_TEXTURED;
      break;
    case  SDLK_UP:
      camera.position.y += 3.0 * delta_time;
      break;
    case  SDLK_DOWN:
      camera.position.y -= 3.0 * delta_time;
      break;
    case  SDLK_a:
      camera.yaw -= 1.0 * delta_time;
      break;
    case  SDLK_d:
      camera.yaw += 1.0 * delta_time;
      break;
    case  SDLK_w :
      camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time); 
      camera.position = vec3_add(camera.position, camera.forward_velocity);
      break;
    case  SDLK_s:
      camera.forward_velocity = vec3_mul(camera.direction, 5.0 * delta_time); 
      camera.position = vec3_sub(camera.position, camera.forward_velocity);
      break;*/
    break;
  } 
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      handle_key_press(event.key.keysym.sym);
      move_player(event.key.keysym.sym, delta_time);
      break;
  }
}

void update(void) {
  /*
  //Locking the execution if not enough time passed
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if(time_to_wait>0 && time_to_wait <= FRAME_TARGET_TIME){
    SDL_Delay(time_to_wait);
  }
  */
    //SDL_Log("m140");
  // Get delta_time factor converted to seconds to be used to update my objects
  delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

  previous_frame_time = SDL_GetTicks();

  // Initialize the counter of triangles to render for the current frame
  num_triangles_to_render = 0;

  // Rotate the cube
  // mesh.rotation.x += 0.01 * delta_time;
  // mesh.rotation.y += 0.92 * delta_time;
  // mesh.rotation.z += 0.01 * delta_time;

  // mesh.scale.x = 0.5;
  // mesh.scale.y = 0.5;
  // mesh.scale.z = 0.5;
  // mesh.translation.z = 5;
  // mesh.translation.y +=0.001;

  // Initialize the target looking at the positive z-axis
  vec3_t target = { 0, -0.5, 1 };
  mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
  camera.direction = vec3_from_vec4(mat4_mul_vec4(camera_yaw_rotation, vec4_from_vec3(target)));

  // Offset the camera position in the direction where the camera is pointing at
  target = vec3_add(camera.position, camera.direction);
  vec3_t up_direction = { 0, 1, 0 };

  // Create the view matrix
  view_matrix = mat4_look_at(camera.position, target, up_direction);
    //SDL_Log("m171");
 
  for (size_t msh_i = 0; msh_i < 256; msh_i++) {
    mesh_t *meshp = meshes_array[msh_i];
//  if (!meshp->exists) continue;
    if (!meshp) continue;

    mesh_t mesh = *meshp;
    
    // Create matrices that will be used to multiply mesh vertices
    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
        //SDL_Log("m186");
        // Loop all triangle faces of our mesh
        int num_faces = array_length(mesh.faces);
        for (int i = 0; i < num_faces; i++) {

            // if(i!= 4) continue;

            
            face_t mesh_face = mesh.faces[i];

            

            vec3_t face_vertices[3];
            face_vertices[0] = mesh.vertices[mesh_face.a - 1];
            face_vertices[1] = mesh.vertices[mesh_face.b - 1];
            face_vertices[2] = mesh.vertices[mesh_face.c - 1];

            vec3_t transformed_vertices[3];

            // Loop all three vertices of this current face and apply transformations
            //SDL_Log("m206");
            for (int j = 0; j < 3; j++) {
                vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

                // World Matrix combining scale, rotation and translation matrices
                world_matrix = mat4_identity();
                // Order matters. First scale, then rotate, and then translate. [T]*[R]*[S]*v
                world_matrix = mat4_mul_mat4(scale_matrix,world_matrix);
                world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
                world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
                world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
                world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);
            
                transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

                // Multiply the view matrix by the vector to transform the scene to camera space
                transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

                transformed_vertices[j] = vec3_from_vec4(transformed_vertex);
            }
            
            //SDL_Log("m227");
            // Check backface culling
            vec3_t vector_a = transformed_vertices[0]; /*   A   */
            vec3_t vector_b = transformed_vertices[1]; /*  / \  */
            vec3_t vector_c = transformed_vertices[2]; /* C---B */

            // Get the vector subtraction of B-A and C-A
            //SDL_Log("vector subtraction of B-A and C-A");
            vec3_t vector_ab = vec3_sub(vector_b, vector_a);
            vec3_t vector_ac = vec3_sub(vector_c, vector_a);
            vec3_normalize(&vector_ab);
            vec3_normalize(&vector_ac);

            ///SDL_Log("m240");

            // Compute the face normal (using cross product to find perpendicular)
            vec3_t normal = vec3_cross(vector_ab, vector_ac);
            vec3_normalize(&normal);

            // Find the vector between vertex A in the triangle and the camera origin
            vec3_t origin = { 0, 0, 0 };
            vec3_t camera_ray = vec3_sub(origin, vector_a);

            //SDL_Log("m250");

            // How aligned the camera ray is with the face normal
            float dot_normal_camera = vec3_dot(normal, camera_ray);

            // Don't render if not facing camera
            if (dot_normal_camera < 0) {
                continue;
            }
            
            //SDL_Log("m254");

        // Create a polygon from the original transformed triangle to be clipped
            polygon_t polygon = polygon_from_triangle(
                transformed_vertices[0],
                transformed_vertices[1],
                transformed_vertices[2],
                mesh_face.a_uv,
                mesh_face.b_uv,
                mesh_face.c_uv
            );
            
            // Clip the polygon and returns a new polygon with potential new vertices
            clip_polygon(&polygon);

            //SDL_Log("m275");

        // Break the clipped polygon apart back into individual triangles
            triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
            int num_triangles_after_clipping = 0;

            triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);
            
            //SDL_Log("m283");
            // Loops all the assembled triangles after clipping
            for (int t = 0; t < num_triangles_after_clipping; t++) {
                triangle_t triangle_after_clipping = triangles_after_clipping[t];

                vec4_t projected_points[3];

                // Loop all three vertices to perform projection and conversion to screen space
                for (int j = 0; j < 3; j++) {
                    // Project the current vertex using a perspective projection matrix
                    projected_points[j] = mat4_mul_vec4(proj_matrix, triangle_after_clipping.points[j]);

                    // Perform perspective divide
                    if (projected_points[j].w != 0) {
                        projected_points[j].x /= projected_points[j].w;
                        projected_points[j].y /= projected_points[j].w;
                        projected_points[j].z /= projected_points[j].w;
                    }

                    // Flip vertically since the y values of the 3D mesh grow bottom->up and in screen space y values grow top->down
                    projected_points[j].y *= -1;

                    // Scale into the view
                    projected_points[j].x *= (window_width / 2.0);
                    projected_points[j].y *= (window_height / 2.0);

                    // Translate the projected points to the middle of the screen
                    projected_points[j].x += (window_width / 2.0);
                    projected_points[j].y += (window_height / 2.0);
                }

                // Calculate the shade intensity based on how aliged is the normal with the flipped light direction ray
                float light_intensity_factor = -vec3_dot(normal, light.direction);

                // Calculate the triangle color based on the light angle
                uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

                // Create the final projected triangle that will be rendered in screen space
                //SDL_Log("m321");
                triangle_t triangle_to_render = {
                    .points = {
                        { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
                        { projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
                        { projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w }
                    },
                    .texcoords = {
                        { triangle_after_clipping.texcoords[0].u, triangle_after_clipping.texcoords[0].v },
                        { triangle_after_clipping.texcoords[1].u, triangle_after_clipping.texcoords[1].v },
                        { triangle_after_clipping.texcoords[2].u, triangle_after_clipping.texcoords[2].v }
                    },
                    .color = triangle_color,
                    .tex = mesh.mesh_texture,
                };

                // Save the projected triangle in the array of triangles to render
                if (num_triangles_to_render < MAX_TRIANGLES) {
                    triangles_to_render[num_triangles_to_render++] = triangle_to_render;
                }
            }
        
        }
    }

    //SDL_Log("m346");
}

void render(void){
  // draw_grid(0xFF0000FF, 15);

  // Fill background with gray color
  clear_color_buffer(0xFF999999);

  // Loop all projected triangles and render them

  for (int i = 0; i< num_triangles_to_render; i++) {
      triangle_t triangle = triangles_to_render[i];

      // Draw textured triangle
      draw_textured_triangle(
      triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v, // vertex A
      triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v, // vertex B
      triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v, // vertex C
      triangle.tex);
  }

  //SDL_Log("m370");

  render_color_buffer();
  clear_color_buffer(0xFF000000);
  clear_z_buffer();
  SDL_RenderPresent(renderer);

  //SDL_Log("m377");
}

// Free memory 
void free_resources(void){
//  free(meshes_array);

    for (int i = 0; i < 256; ++i)
    {
        if(!meshes_array[i])
            continue;

        array_free(meshes_array[i]->vertices);
        array_free(meshes_array[i]->faces);
        unload_png_texture_data(meshes_array[i]);
        free(meshes_array[i]);
    }

    free(color_buffer);
    free(z_buffer);
}

extern float player_angle;

int main(int argc, char *argv[]) {
    is_running = initialize_window();

    setup();
    init_ma(); // ma = meshes array 
    init_map();

    player_char = load_obj_file_data("./assets/polet.obj");
    player_char->translation = (vec3_t){.x = 0.0f, .y = 0.0f, .z = 0.0f};
    player_char->scale = (vec3_t){0.5f,0.5f,0.5f};
    load_png_texture_data("./assets/p.png", player_char);
    add_to_ma(player_char); // this part works as it prints "ok" in the console 
    player_char->exists = true;

    load_map(NULL, "map01");


    while (is_running){
        process_input();
        update();
        //SDL_Log("for some reason update worked");
        render();
        //SDL_Log("l");
        // camera.direction.x -= 0.1;
        player_char->rotation.x += 0.1f;
    }

    free_resources();
    destroy_window();

    return 0;
}