//
// Created by cuan on 19/05/16.
//

#ifndef CGRA_PROJECT_A3_DEFINE_MATERIALS_H
#define CGRA_PROJECT_A3_DEFINE_MATERIALS_H

#include "opengl.hpp"

//=====================================================================
//                      MATERIALS
//=====================================================================
GLfloat mat_red_plastic_ambient[] = {0.f, 0.f, 0.f, 1.f};
GLfloat mat_red_plastic_specular[] = { .7f, .6f, .6f, 1.f };
GLfloat mat_red_plastic_diffuse[] = { .5f, .0f, .0f, 1.f};
GLfloat mat_red_plastic_shininess[] = { 32.f };

GLfloat mat_bronze_ambient[] = {.2125f, .1275f, .054f, 1.f};
GLfloat mat_bronze_specular[] = {0.992157f, 0.941176f, 0.807843f, 1.f};
GLfloat mat_bronze_diffuse[] = {.714f, .4284f, .18144f, 1.f};
GLfloat mat_bronze_shininess[] = {25.6f};

GLfloat mat_white_plastic_ambient[] = {0.f, 0.f, 0.f, 1.f};
GLfloat mat_white_plastic_specular[] = {.7f, .7f, .7f, 1.f};
GLfloat mat_white_plastic_diffuse[] = {.85f, .85f, .85f, 1.f};
GLfloat mat_white_plastic_shininess[] = {32.f};

GLfloat mat_chrome_ambient[] = {0.25f, 0.25f, 0.4f, 1.f};
GLfloat mat_chrome_diffuse[] = {0.4f, 0.4f, 0.4f, 1.f};
GLfloat mat_chrome_specular[] = {0.774597f, 0.774597f, 0.774597f, 1.f};
GLfloat mat_chrome_shininess[] = {76.8f};

GLfloat mat_white_ground_ambient[] = { 0.6f, 0.6f, 0.6f, 1.f };
GLfloat mat_white_ground_specular[] = { 0.5f, 0.5f, 0.5f, 1.f };
GLfloat mat_white_ground_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.f };
GLfloat mat_white_ground_shininess[] = { 5.f };

//=====================================================================
//                      LIGHTING
//=====================================================================
GLfloat full_white[] = {1.f,1.f,1.f,1.f};

// weak ambient
GLfloat light_0_position[] = {0.0f, 1.0f, 1.0f, 0.0f}; // TODO wheres position??
GLfloat light_0_diffintensity[] = {0.7, 0.7, 0.7, 1.0, 1.0f};
GLfloat light_0_ambient[] = {0.05, 0.05, 0.05 , 1.0f};
//GLfloat light_0_ambient[] = {1.f, 1.f, 1.f, 1.f};

// spot light
GLfloat light_1_direction[] = {-0.4f, -0.65f, -0.628765f, 0.f};
GLfloat light_1_position[] = {5.f, 7.f, 6.f, 1.0f};
GLfloat light_1_diffintensity[] = {.65f, .65f, .65f, 1.0f};
GLfloat light_1_ambient[] = {.65f, .65f, .65f, 1.0f};
GLfloat light_1_specular[] = {1.f, 1.f, 1.f, 1.0f};

// directional light
GLfloat light_2_position[] = {0.0f, 0.0f, 1.0f, 0.0f};
GLfloat light_2_direction[] = {0.0f, 1.0f, -1.0f, 0.0f };
GLfloat light_2_diffintensity[] = {0.35f, 0.35f, 0.35f, 1.0f };
GLfloat light_2_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light_2_specular[] = {1.f, 1.f, 1.f, 1.0f };

// Point light
//GLfloat light_3_direction[] = {-0.286648f, -0.62188f, -0.728765f, 0.f};
//GLfloat light_3_direction[] = {0.f, 1.f, 0.f, 0.f};
GLfloat light_3_position[] = {5.f, 13.f, 14.f, 1.0f};
//GLfloat light_3_position[] = {-10.f, 10.f, 5.f, 1.0f};
GLfloat light_3_diffintensity[] = {.65f, .65f, .65f, 1.0f};
GLfloat light_3_ambient[] = {.65f, .65f, .65f, 1.0f};
GLfloat light_3_specular[] = {1.f, 1.f, 1.f, 1.0f};

#endif //CGRA_PROJECT_A3_DEFINE_MATERIALS_H


//=====================================================================
//                      MARK LIGHTS
//=====================================================================
 

GLfloat mark_light_direction[] = { 0.0f, -2.0f, 1.0f, 0.0f };

GLfloat mark_light_diffintensity[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat mark_light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat mark_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };



GLfloat mark_point_direction[] = { 0.0f, 10.0f, 0.0f, 1.0f };
GLfloat mark_point_diffintensity[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat mark_point_specular[] = { 0.3f, 0.2f, 0.2f, 1.0f };

GLfloat mark_dir_direction[] = { 0.f, 0.5f, -1.0f, 0.0f };
GLfloat mark_dir_diffintensity[] = { 0.1f, 0.1f, 0.3f, 1.0f };
GLfloat mark_dir_specular[] = { 0.1f, 0.1f, 0.3f, 1.0f };

GLfloat mark_ambient[] = { 0.00001f, 0.00001f, 0.0f, 1.0f };
