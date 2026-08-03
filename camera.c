#include "camera.h"

camera_t camera = {
    .position = { 0, 3, -4 },
    .direction = { 1, 1, 0}, // -pi/6 soit
    .forward_velocity = { 0, 0, 0 },
    .yaw = 0
};
