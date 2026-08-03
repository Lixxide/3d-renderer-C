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

mesh_t *player_char;

float player_angle = -3.14159265358979323/2; // normalement c'est assez proche

void move_player(SDL_Keycode keycode, float dt) {
    if (keycode == SDLK_UP || keycode == SDLK_w || keycode == SDLK_z) {
        player_char->translation.z -= sin(player_angle) * dt * 5.0;
        player_char->translation.x -= cos(player_angle) * dt * 5.0;
        camera.position.x = player_char->translation.x + cos(player_angle)*5.0f;
        camera.position.z = player_char->translation.z + sin(player_angle)*5.0f;
    }
    else if (keycode == SDLK_DOWN || keycode == SDLK_s) {
        camera.position.z += sin(player_angle) * dt * 5.0;
        camera.position.x += cos(player_angle) * dt * 5.0;
        camera.position.x = player_char->translation.x + cos(player_angle)*5.0f;
        camera.position.z = player_char->translation.z + sin(player_angle)*5.0f;
    }
    else if (keycode == SDLK_LEFT || keycode == SDLK_q || keycode == SDLK_a) {
        camera.yaw -= 1.0 * dt;
        player_angle += 1.0 * dt;
        camera.position.x = player_char->translation.x + cos(player_angle)*5.0f;
        camera.position.z = player_char->translation.z + sin(player_angle)*5.0f;
        player_char->rotation.y = -player_angle + 3.14159265358979323;
    }
    else if (keycode == SDLK_RIGHT || keycode == SDLK_d) {
        camera.yaw += 1.0 * dt;
        player_angle -= 1.0 * dt;
        camera.position.x = player_char->translation.x + cos(player_angle)*5.0f;
        camera.position.z = player_char->translation.z + sin(player_angle)*5.0f;
        player_char->rotation.y = -player_angle + 3.14159265358979323;
    }
}