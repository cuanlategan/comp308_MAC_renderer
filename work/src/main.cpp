//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>


#include "cgra_geometry.hpp"
#include "cgra_math.hpp"
#include "simple_image.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"
#include "geometry.hpp"
#include "define.h"
#include "cuan_define.h"
#include "wave_generator.h"
#include "field.h"
#include "RiverHandler.h"

using namespace std;
using namespace cgra;

// Window
//
GLFWwindow *g_window;


// Projection values
// 
float g_fovy = 60.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
vec2 g_mousePosition;
float g_pitch = 0;
float g_yaw = 0;
float g_zoom = 1.0;

vec3 g_camera_eye(0.f,0.f,0.f);

// Values and fields to showcase the use of shaders
// Remove when modifying main.cpp for Assignment 3
//
bool g_useShader = true;
GLuint g_phong_sdr = 0;
GLuint g_grass_tex = 0;


// Objects to be rendered
WaveGenerator *g_wave_generator;
Field field;
// Marks River Gen
RiverHandler *g_riverHandler;


float t = 0.f;

bool draw_lights = true;
bool draw_ambiant_light = true;
bool draw_spot_light = true;
bool draw_directional_light = true;
bool draw_point_light = true;

bool draw_points = false;
bool draw_geometry = true;

float spot_cutoff = 6.5f;
float table_rotation = 0.f;
bool rotate_table = false;




// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
//
void cursorPosCallback(GLFWwindow *win, double xpos, double ypos) {
    // cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;
    if (g_leftMouseDown) {
        g_yaw -= g_mousePosition.x - xpos;
        g_pitch -= g_mousePosition.y - ypos;

    }
    g_mousePosition = vec2(xpos, ypos);
}


// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
//
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
    // cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        g_leftMouseDown = (action == GLFW_PRESS);
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (g_useShader) {
            g_useShader = false;
            cout << "Using the default OpenGL pipeline" << endl;
        }
        else {
            g_useShader = true;
            cout << "Using a shader" << endl;
        }
    }
}


// Scroll callback
// Called for scroll event on since the last glfwPollEvents
//
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
    // cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
    g_zoom -= yoffset * g_zoom * 0.2;
}


// Keyboard callback
// Called for every key event on since the last glfwPollEvents
//
void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
    //cout << "Key Callback :: key=" << key << "scancode=" << scancode
    //<< "action=" << action << "mods=" << mods << endl;

    if (key == 49 && action == 1) { // key 1
        draw_ambiant_light = !draw_ambiant_light;
        cout << "ambiant: " << draw_ambiant_light << endl;
    }
    if (key == 50 && action == 1) { // key 2
        draw_point_light = !draw_point_light;
        cout << "draw_point_light: " << draw_point_light << endl;
    }
    if (key == 51 && action == 1) { // key 3
        draw_directional_light = !draw_directional_light;
        cout << "draw_directional_light: " << draw_directional_light << endl;
    }
    if (key == 52 && action == 1) { // key 4
        draw_spot_light = !draw_spot_light;
        cout << "draw_spot_light: " << draw_spot_light << endl;
    }
    if (key == 70 && action == 1) { // key f
        draw_lights = !draw_lights;
    }
    if (key == 69 && action == 1) { // key e
        draw_points = !draw_points;
        cout << "draw_points: " << draw_points << endl;
    }
    if (key == 81 && action == 1) { // key q
        draw_geometry = !draw_geometry;
        cout << "draw_geometry: " << draw_geometry << endl;
    }
    if (key == 84) { // key t

    }
    if (key == 87) { // key w
        //g_camera_eye.z += 0.5f;

        g_camera_eye.x += float(sin(radians(g_yaw)));
        g_camera_eye.z -= float(cos(radians(g_yaw)));
        g_camera_eye.y -= float(sin(radians(g_pitch)));

    }
    if (key == 65) { // key a
        //g_camera_eye.x += 0.5f;

        g_camera_eye.x -= float(cos(radians(g_yaw))) * 0.2;
        g_camera_eye.z -= float(sin(radians(g_yaw))) * 0.2;
    }
    if (key == 83) { // key s
        //g_camera_eye.z -= 0.5f;

        g_camera_eye.x -= float(sin(radians(g_yaw)));
        g_camera_eye.z += float(cos(radians(g_yaw)));
        g_camera_eye.y += float(sin(radians(g_pitch)));
    }
    if (key == 68) { // key d
        //g_camera_eye.x -= 0.5f;

        g_camera_eye.x += float(cos(radians(g_yaw))) * 0.2;
        g_camera_eye.z += float(sin(radians(g_yaw))) * 0.2;
    }
    if (key == 341) { // key ctrl

    }
    if (key == 32) { // key spacebar

    }
    if (key == 265) { // key up-arrow

    }
    if (key == 264) { // key down-arrow

    }
    if (key == 263) { // key left-arrow

    }
    if (key == 262) { // key right-arrow

    }

    //TODO Clean Up
    /*vec4 v(light_1_direction[0], light_1_direction[1], light_1_direction[2], light_1_direction[3]);
    //vec3 v(light_1_direction[0],light_1_direction[1],light_1_direction[2]);
    v = normalize(v);
    cout << "l1 pos: " << light_1_position[0] << " " << light_1_position[1] << " " << light_1_position[2] << "   v: " <<
    v << endl;*/
}

// Character callback
// Called for every character input event on since the last glfwPollEvents
//
void charCallback(GLFWwindow *win, unsigned int c) {
    // cout << "Char Callback :: c=" << char(c) << endl;
    // Not needed for this assignment, but useful to have later on
}


// Sets up where and what the light is
// Called once on start up
// 
void initLight() {

    if (draw_ambiant_light) {
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_0_ambient);
        //glLightfv(GL_LIGHT0, GL_POSITION, light_0_position); //keep default z dir //TODO
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffintensity);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);
        glEnable(GL_LIGHT0);
    } else { glDisable(GL_LIGHT0); }

    if (draw_spot_light) {
        glLightfv(GL_LIGHT1, GL_POSITION, light_1_position);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_1_direction);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.5f);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_1_diffintensity);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_1_ambient);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_1_specular);
        glEnable(GL_LIGHT1);
    } else { glDisable(GL_LIGHT1); }

    if (draw_directional_light) {
        //glLightfv(GL_LIGHT2, GL_POSITION, light_1_position);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_2_direction);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_2_diffintensity);
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_2_ambient);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_2_specular);
        glEnable(GL_LIGHT2);
    } else { glDisable(GL_LIGHT2); }

    if (draw_point_light) {
        glLightfv(GL_LIGHT3, GL_POSITION, light_3_position);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light_3_diffintensity);
        glLightfv(GL_LIGHT3, GL_AMBIENT, light_3_ambient);
        glLightfv(GL_LIGHT3, GL_SPECULAR, light_3_specular);
        glEnable(GL_LIGHT3);
    } else { glDisable(GL_LIGHT3); }

}


// An example of how to load a texure from a hardcoded location
//
void initTexture() {



    Image tex_grass("./work/res/textures/tall-grass3.png");
    glGenTextures(1, &g_grass_tex); // Generate texture ID

    glBindTexture(GL_TEXTURE_2D, g_grass_tex); // Bind it as a 2D texture
    // Finnaly, actually fill the data into our texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex_grass.w, tex_grass.h, tex_grass.glFormat(), GL_UNSIGNED_BYTE,
                      tex_grass.dataPointer());

    //glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,tex_grass.w,tex_grass.h,0,GL_RGBA,GL_UNSIGNED_BYTE,tex_grass.dataPointer());

    // Setup sampling strategies
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

}




// An example of how to load a shader from a hardcoded location
//
void initShader() {
    // To create a shader program we use a helper function
    // We pass it an array of the types of shaders we want to compile
    // and the corrosponding locations for the files of each stage
    g_phong_sdr = makeShaderProgramFromFile({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER},
                                         {"./work/res/shaders/phongVert.vert", "./work/res/shaders/phongFrag.frag"});
}


// Sets up where the camera is in the scene
// 
void setupCamera(int width, int height) {
    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(g_fovy, width / float(height), g_znear, g_zfar);

    // Set up the view part of the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(g_pitch, 1, 0, 0);
    glRotatef(g_yaw, 0, 1, 0);
    glTranslatef(-g_camera_eye.x,-g_camera_eye.y, -g_camera_eye.z);



    //cout << "g_pitch: " << g_pitch << "  g_yaw: " << g_yaw << endl;
}




void drawLights();

// Draw function
//
void render(int width, int height) {

    // Grey/Blueish background
    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Enable flags for normal rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);


    setupCamera(width, height);

    // Without shaders
    // Uses the default OpenGL pipeline
    if (!g_useShader) {


        t+=0.02;
        if(t > 32.f) {cout << "time reset\n"; t = 0.f;}

        // cuans
        if(draw_geometry){ field.renderField(g_wave_generator,t); }
        if(draw_points){ field.renderGrid(g_wave_generator,t); }

        glFlush();

    }


    else {




        t+=0.02;
        if(t > 32.f) {cout << "time reset\n"; t = 0.f;}


        glUseProgram(g_phong_sdr);
        field.renderFieldShader(g_wave_generator, t, g_phong_sdr);
        glUseProgram(0); // Unbind our shader

        glFlush();


    }



    if (draw_lights) {
        drawLights();
    }

    // Disable flags for cleanup (optional)
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_NORMALIZE);

}


void drawLights() {

    glPushMatrix();
    {
        glColor3f(1.f, 1.f, 1.f);
        glMaterialfv(GL_FRONT, GL_AMBIENT, full_white);

        //rotate cone
        glPushMatrix();
        {
            vec3 li(light_1_direction[0],
                    light_1_direction[1],
                    light_1_direction[2]
            );

            li = normalize(li);
            vec3 z(0.f, 0.f, 1.f);
            float theta = degrees(acos(dot(z, li))); //angle between bone and z-axis
            vec3 rotationAxis = cross(z, li); // normal of bone and z-axis
            glTranslatef(light_1_position[0], light_1_position[1], light_1_position[2]);
            glRotatef(theta, rotationAxis.x, rotationAxis.y, rotationAxis.z); // rotate around normal

            // spot light cutoff shade
            float r = 10.f * tan(radians(spot_cutoff));
            GLUquadricObj *theQuadric;
            theQuadric = gluNewQuadric();
            gluQuadricDrawStyle(theQuadric, GLU_LINE);
            gluQuadricNormals(theQuadric, GLU_SMOOTH);
            gluCylinder(theQuadric, 0.f, r, 5.f, 10, 1);
            gluDeleteQuadric(theQuadric);

            glPopMatrix();
        }

        glTranslatef(light_1_position[0], light_1_position[1], light_1_position[2]);


        glPushMatrix();
        {
            glTranslatef(0.f, .8f, 0.f);
            glRotatef(90, 1.f, 0.f, 0.f);
            cgraCylinder(.1, .1, 1.6f);
        }
        glPopMatrix();

        cgraSphere(.35);

        glBegin(GL_LINES);
        {
            glVertex3f(0, 0, 0);
            glVertex3f(light_1_direction[0]*1000.f, light_1_direction[1]*1000.f, light_1_direction[2]*1000.f);
        }
        glEnd();
    }
    glPopMatrix();
}




// Forward decleration for cleanliness (Ignore)
void APIENTRY debugCallbackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *, GLvoid *);


//Main program
// 
int main(int argc, char **argv) {

    // Initialize the GLFW library
    if (!glfwInit()) {
        cerr << "Error: Could not initialize GLFW" << endl;
        abort(); // Unrecoverable error
    }

    // Get the version for GLFW for later
    int glfwMajor, glfwMinor, glfwRevision;
    glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

    // Create a windowed mode window and its OpenGL context
    g_window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!g_window) {
        cerr << "Error: Could not create GLFW window" << endl;
        abort(); // Unrecoverable error
    }

    // Make the g_window's context is current.
    // If we have multiple windows we will need to switch contexts
    glfwMakeContextCurrent(g_window);



    //Initialize GLEW
    // must be done after making a GL context current (glfwMakeContextCurrent in this case)
    glewExperimental = GL_TRUE; // required for full GLEW functionality for OpenGL 3.0+
    GLenum err = glewInit();
    if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
        cerr << "Error: " << glewGetErrorString(err) << endl;
        abort(); // Unrecoverable error
    }



    // Print out our OpenGL verisions
    cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
    cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    cout << "Using GLFW " << glfwMajor << "." << glfwMinor << "." << glfwRevision << endl;



    // Attach input callbacks to g_window
    glfwSetCursorPosCallback(g_window, cursorPosCallback);
    glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
    glfwSetScrollCallback(g_window, scrollCallback);
    glfwSetKeyCallback(g_window, keyCallback);
    glfwSetCharCallback(g_window, charCallback);



    // Enable GL_ARB_debug_output if available. Not nessesary, just helpful
    if (glfwExtensionSupported("GL_ARB_debug_output")) {
        // This allows the error location to be determined from a stacktrace
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        // Set the up callback
        glDebugMessageCallbackARB(debugCallbackARB, nullptr);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
        cout << "GL_ARB_debug_output callback installed" << endl;
    }
    else {
        cout << "GL_ARB_debug_output not available. No worries." << endl;
    }

    initLight();
    initTexture();
    initShader();


    // Initialize Geometry/Material/Lights
    // YOUR CODE GOES HERE
    // ...

    g_riverHandler = new RiverHandler();
    g_wave_generator = new WaveGenerator();

    g_wave_generator->addGerstnerWave(4.567,0.3,.8,1.5,vec2(-.3f, 1.f));
    g_wave_generator->addGerstnerWave(7.685,0.2,.8,1.5,vec2(-.35f, 0.8f));
    g_wave_generator->addGerstnerWave(2.2,0.1,.1,1.5,vec2(0.f, -1.0f));
    g_wave_generator->addGerstnerWave(16.2,1.0,.75,2.5,vec2(0.f, 1.0f));



    field.generateCluster(GRID_DIMENSION);
    field.generateGrid(GRID_DIMENSION);



    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(g_window)) {

        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame: %f\n", 1000.0/double(nbFrames), double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Make sure we draw to the WHOLE window
        int width, height;
        glfwGetFramebufferSize(g_window, &width, &height);

        // Main Render
        initLight();
        render(width, height);

        // Swap front and back buffers
        glfwSwapBuffers(g_window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
}






//--------------------------------------------------------------------------
// Fancy debug stuff
//--------------------------------------------------------------------------

// function to translate source to string
string getStringForSource(GLenum source) {

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            return ("API");
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return ("Window System");
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return ("Shader Compiler");
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return ("Third Party");
        case GL_DEBUG_SOURCE_APPLICATION:
            return ("Application");
        case GL_DEBUG_SOURCE_OTHER:
            return ("Other");
        default:
            return ("n/a");
    }
}

// function to translate severity to string
string getStringForSeverity(GLenum severity) {

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            return ("HIGH!");
        case GL_DEBUG_SEVERITY_MEDIUM:
            return ("Medium");
        case GL_DEBUG_SEVERITY_LOW:
            return ("Low");
        default:
            return ("n/a");
    }
}

// function to translate type to string
string getStringForType(GLenum type) {
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            return ("Error");
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return ("Deprecated Behaviour");
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return ("Undefined Behaviour");
        case GL_DEBUG_TYPE_PORTABILITY:
            return ("Portability Issue");
        case GL_DEBUG_TYPE_PERFORMANCE:
            return ("Performance Issue");
        case GL_DEBUG_TYPE_OTHER:
            return ("Other");
        default:
            return ("n/a");
    }
}

// actually define the function
void APIENTRY debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar *message,
                               GLvoid *) {
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) return;

    cerr << endl; // extra space

    cerr << "Type: " <<
    getStringForType(type) << "; Source: " <<
    getStringForSource(source) << "; ID: " << id << "; Severity: " <<
    getStringForSeverity(severity) << endl;

    cerr << message << endl;

    if (type == GL_DEBUG_TYPE_ERROR_ARB) throw runtime_error("");
}