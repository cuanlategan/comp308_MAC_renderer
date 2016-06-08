//
// Created by cuan on 5/26/16.
//

#ifndef CGRA_PROJECT_A3_GRID_H
#define CGRA_PROJECT_A3_GRID_H

#include "opengl.hpp"
#include "cgra_math.hpp"


class Grid {
private:

    std::vector<cgra::vec2> m_uvs;
    cgra::vec3 position;

public:
    Grid();
    Grid(cgra::vec3 position);
    void renderGeometry();

    std::vector<cgra::vec3> m_new_points;
    std::vector<cgra::vec3> m_points;	// Point list
};


#endif //CGRA_PROJECT_A3_GRID_H
