//
// Created by cuan on 5/31/16.
//

#ifndef CGRA_PROJECT_A3_FIELD_H
#define CGRA_PROJECT_A3_FIELD_H
#pragma once

#include "cgra_math.hpp"
#include "grass.h"
#include "wave_generator.h"
#include "grid.h"
#include "cuan_define.h"
#include "geometry.hpp"
#include "RiverHandler.h"

class Field {
private:
    std::vector<Grass> *grass_clusters;
    std::vector<Grid> *grid_clusters;
    std::vector<cgra::vec3> *m_centers;

    std::vector<cgra::vec3> *m_points;
    std::vector<cgra::vec2> *m_uvs;
    std::vector<cgra::vec3> *m_normals;

    float grass_width = 2.f;

    GLuint m_nVBOVertices = 0;
    GLuint m_nVBOTexCoords = 0;
    GLuint m_nVBONormals = 0;

    GLuint g_grass_tex = 0;
    GLuint g_center_attrib = 0;

public:


    float wavelength = 0.133f;
    float amplitude = 0.005f;
    float steepnes = 0.8f;
    float speed = 0.05f;
    int hasTex = 1;
    float dirX = 1.3f;
    float dirY = 0.0f;

    Field() {
        grass_clusters = new std::vector<Grass>;
        grid_clusters = new std::vector<Grid>;

        m_points = new std::vector<cgra::vec3>;
        m_uvs = new std::vector<cgra::vec2>;
        m_normals = new std::vector<cgra::vec3>;

        m_centers = new std::vector<cgra::vec3>;
        m_points->reserve(GRID_DIMENSION * GRID_DIMENSION * 3 * 2 * 3);
        m_normals->reserve(GRID_DIMENSION * GRID_DIMENSION * 3 * 2 * 3);
        m_uvs->reserve(GRID_DIMENSION * GRID_DIMENSION * 3 * 2 * 3);
        m_centers->reserve(GRID_DIMENSION * GRID_DIMENSION);

        initGrassTexture();
        srand(time(NULL));
    }

    void generateCluster(int num_clusters);

    void renderField(WaveGenerator *wave_gen, float time);

    void generateGrid(int num_clusters);

    void renderGrid(WaveGenerator *wave_gen, float time);

    void BuildVBOs();

    void renderFieldShader(WaveGenerator *wave_gen, float time, GLint shader);

    void initGrassTexture();

    cgra::vec3 getRandomVertOnFace(cgra::vec3 a, cgra::vec3 b, cgra::vec3 c);

    void generateCluster(Geometry *geo, RiverHandler *riverHandler);


    bool checkRiverFace(triangle &tri, Geometry *geo, vector<vector<vec3>> riverHandler);

    bool checkRiverFace(int triIndex, Geometry *geo, vector<int> riverTris);
};


#endif //CGRA_PROJECT_A3_FIELD_H
