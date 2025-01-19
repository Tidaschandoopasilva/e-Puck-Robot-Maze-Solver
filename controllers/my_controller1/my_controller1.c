#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/camera.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28
#define NO_WALL_COUNT_THRESHOLD 50
#define COLOR_COUNT 5

//colours
const int RED_THRESHOLD[3] = {255, 0, 0};
const int YELLOW_THRESHOLD[3] = {255, 255, 0};
const int PINK_THRESHOLD[3] = {255, 0, 255};
const int BROWN_THRESHOLD[3] = {165, 105, 30};
const int GREEN_THRESHOLD[3] = {0, 255, 0};

const char *COLOR_NAMES[COLOR_COUNT] = {"Red", "Yellow", "Pink", "Brown", "Green"};//order
int found_colors[COLOR_COUNT] = {0};  


bool is_color_match(int r, int g, int b, const int threshold[3]) {
  return abs(r - threshold[0]) < 50 && abs(g - threshold[1]) < 50 && abs(b - threshold[2]) < 50;
}//colour analysing with tolerance due to lighting and res of camera

int main(int argc, char **argv) {
  wb_robot_init();

  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");

  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(right_motor, 0.0);
  wb_motor_set_velocity(left_motor, 0.0);

  WbDeviceTag prox_sensors[8];
  char prox_sensor_name[50];
  for (int i = 0; i < 8; ++i) {
    sprintf(prox_sensor_name, "ps%d", i);
    prox_sensors[i] = wb_robot_get_device(prox_sensor_name);
    wb_distance_sensor_enable(prox_sensors[i], TIME_STEP);
  }

  WbDeviceTag camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;
  int no_wall_count = 0;
  int current_color_index = 0;  

  while (wb_robot_step(TIME_STEP) != -1) {
    bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 100;
    bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 100;
    bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 100;

    
    int width = wb_camera_get_width(camera);
    int height = wb_camera_get_height(camera);
    const unsigned char *image = wb_camera_get_image(camera);
    bool color_found = false;

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int r = wb_camera_image_get_red(image, width, x, y);
        int g = wb_camera_image_get_green(image, width, x, y);
        int b = wb_camera_image_get_blue(image, width, x, y);

        if (current_color_index == 0 && is_color_match(r, g, b, RED_THRESHOLD)) {
          found_colors[0] = 1;
          color_found = true;
        } else if (current_color_index == 1 && is_color_match(r, g, b, YELLOW_THRESHOLD)) {
          found_colors[1] = 1;
          color_found = true;
        } else if (current_color_index == 2 && is_color_match(r, g, b, PINK_THRESHOLD)) {
          found_colors[2] = 1;
          color_found = true;
        } else if (current_color_index == 3 && is_color_match(r, g, b, BROWN_THRESHOLD)) {
          found_colors[3] = 1;
          color_found = true;
        } else if (current_color_index == 4 && is_color_match(r, g, b, GREEN_THRESHOLD)) {
          found_colors[4] = 1;
          color_found = true;
        }
      }
    }//pixel by pixel analysing for colours following the colour index order

    if (color_found) {
      printf("Found %s\n", COLOR_NAMES[current_color_index]);
      current_color_index++;
      if (current_color_index >= COLOR_COUNT) {
        printf("All colors found!\n");
        wb_motor_set_velocity(left_motor, 0);
        wb_motor_set_velocity(right_motor, 0);
        break;
      }
    } // stopping after all colours detected

    
    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
      no_wall_count = 0;
    } else if (left_wall) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED;
      no_wall_count = 0;
    } else {
      no_wall_count++;
      if (no_wall_count > NO_WALL_COUNT_THRESHOLD) {
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED;
      } else {
        left_speed = MAX_SPEED / 8;
        right_speed = MAX_SPEED;
      }
    }//movement logic - left wall following with a fail safe incase wall not found.

    if (left_corner) {
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED / 8;
    }

    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
  }

  wb_robot_cleanup();
  return 0;
}
