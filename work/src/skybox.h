//
// Created by lategacuan on 12/06/16.
//

#ifndef CGRA_PROJECT_A3_SKYBOX_H
#define CGRA_PROJECT_A3_SKYBOX_H



#include "simple_image.hpp"
#include "simple_shader.hpp"

class Skybox {
    private:
    GLuint g_cubemap_tex = 0;
    GLuint g_nVBOskybox = 0;
    GLuint g_shader = 0;

    float points[6*6*3] = {
            -10.0f,  10.0f, -10.0f,
            -10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,
            10.0f,  10.0f, -10.0f,
            -10.0f,  10.0f, -10.0f,

            -10.0f, -10.0f,  10.0f,
            -10.0f, -10.0f, -10.0f,
            -10.0f,  10.0f, -10.0f,
            -10.0f,  10.0f, -10.0f,
            -10.0f,  10.0f,  10.0f,
            -10.0f, -10.0f,  10.0f,

            10.0f, -10.0f, -10.0f,
            10.0f, -10.0f,  10.0f,
            10.0f,  10.0f,  10.0f,
            10.0f,  10.0f,  10.0f,
            10.0f,  10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,

            -10.0f, -10.0f,  10.0f,
            -10.0f,  10.0f,  10.0f,
            10.0f,  10.0f,  10.0f,
            10.0f,  10.0f,  10.0f,
            10.0f, -10.0f,  10.0f,
            -10.0f, -10.0f,  10.0f,

            -10.0f,  10.0f, -10.0f,
            10.0f,  10.0f, -10.0f,
            10.0f,  10.0f,  10.0f,
            10.0f,  10.0f,  10.0f,
            -10.0f,  10.0f,  10.0f,
            -10.0f,  10.0f, -10.0f,

            -10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f,  10.0f,
            10.0f, -10.0f, -10.0f,
            10.0f, -10.0f, -10.0f,
            -10.0f, -10.0f,  10.0f,
            10.0f, -10.0f,  10.0f
    };

    public:
        void initSkybox(){
            Image pX("./work/res/textures/cubeMapRight.jpg");
            Image nX("./work/res/textures/cubeMapLeft.jpg");
            Image pY("./work/res/textures/cubeMapTop.jpg");
            Image nY("./work/res/textures/cubeMapBottom.jpg");
            Image nZ("./work/res/textures/cubeMapBack.jpg");
            Image pZ("./work/res/textures/cubeMapFront.jpg");

            glGenTextures(1, &g_cubemap_tex); // Generate texture ID
            glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap_tex);

            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,
                    GL_RGB, pX.w, pX.h, 0, pX.glFormat(), GL_UNSIGNED_BYTE, pX.dataPointer()
            );
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,
                    GL_RGB, nX.w, nX.h, 0, nX.glFormat(), GL_UNSIGNED_BYTE, nX.dataPointer()
            );
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,
                    GL_RGB, nY.w, nY.h, 0, nY.glFormat(), GL_UNSIGNED_BYTE, nY.dataPointer()
            );
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,
                    GL_RGB, pY.w, pY.h, 0, pY.glFormat(), GL_UNSIGNED_BYTE, pY.dataPointer()
            );
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,
                    GL_RGB, pZ.w, pZ.h, 0, pZ.glFormat(), GL_UNSIGNED_BYTE, pZ.dataPointer()
            );
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,
                    GL_RGB, nZ.w, nZ.h, 0, nZ.glFormat(), GL_UNSIGNED_BYTE, nZ.dataPointer()
            );

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glGenBuffers(1, &g_nVBOskybox);                  // Get A Valid Name
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, points[0]);         // Bind The Buffer
            // Load The Data
            glBufferDataARB(GL_ARRAY_BUFFER_ARB,  sizeof(float)*6*6*3, &points[0], GL_DYNAMIC_DRAW_ARB);

            g_shader = cgra::makeShaderProgramFromFile({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER},
                                                    {"./work/res/shaders/skybox.vert", "./work/res/shaders/skybox.frag"});

        }

};



#endif //CGRA_PROJECT_A3_SKYBOX_H
