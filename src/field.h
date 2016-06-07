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

class Field {
private:
    std::vector<Grass>* grass_clusters;
    std::vector<Grid>* grid_clusters;


    std::vector<cgra::vec3>*m_points;
    std::vector<cgra::vec2>*m_uvs;


    float grass_width = 2.f;

    GLuint m_nVBOVertices = 0;
    GLuint m_nVBOTexCoords = 0;
    GLuint m_nVBOCenters = 0;

public:

    std::vector<cgra::vec3>* m_centers; // TODO make private and add getter


    Field() {
        grass_clusters = new std::vector<Grass>;
        grid_clusters = new std::vector<Grid>;

        m_points = new std::vector<cgra::vec3>;
        m_uvs = new std::vector<cgra::vec2>;
        m_centers = new std::vector<cgra::vec3>;
        m_points->reserve(GRID_DIMENSION * GRID_DIMENSION * 3 * 2 * 3);
        m_uvs->reserve(GRID_DIMENSION * GRID_DIMENSION * 3 * 2 * 3);
        m_centers->reserve(GRID_DIMENSION*GRID_DIMENSION);
    }

    void generateCluster(int num_clusters);
    void renderField(WaveGenerator *wave_gen, float time);

    void generateGrid(int num_clusters);
    void renderGrid(WaveGenerator *wave_gen, float time);

    void BuildVBOs();


    void renderFieldShader(WaveGenerator *wave_gen, float time, GLint shader);
};


#endif //CGRA_PROJECT_A3_FIELD_H
