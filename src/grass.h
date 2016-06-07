//
// Created by cuan on 5/26/16.
//

#ifndef CGRA_PROJECT_A3_GRASS_H
#define CGRA_PROJECT_A3_GRASS_H

#include "opengl.hpp"
#include "cgra_math.hpp"


class Grass {
private:
    std::vector<cgra::vec3> m_points;	// Point list
    std::vector<cgra::vec2> m_uvs;
    cgra::vec3 position;

    GLfloat m_shear[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

public:
    Grass();
    Grass(cgra::vec3 position);
    void renderGeometry();
    const cgra::vec3& getPosition(){return position;}

    std::vector<cgra::vec3> getPoints(){ return m_points;}
    std::vector<cgra::vec2> getUvs(){ return m_uvs;}

    void setShear(const cgra::vec3 &shear);
};


#endif //CGRA_PROJECT_A3_GRASS_H
