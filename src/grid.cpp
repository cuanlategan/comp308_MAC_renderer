//
// Created by cuan on 5/26/16.
//

#include "grid.h"


using namespace std;
using namespace cgra;

void Grid::renderGeometry()
{


    glPushMatrix();
    {
        glColor3f(1.0, 0.0, 0.0);
        glRotatef(-90,1.f,0.f,0.f);
        //glPointSize(5.f);
        glBegin(GL_LINE_LOOP);
        {
            for(int i=0;i<3;i++)
            {   vec3 p = m_new_points.at(i);
                glVertex3f(p.x, p.y, p.z);
            }
        }
        glEnd();
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINE_LOOP);
        {
            for(int i=3;i<6;i++)
            {   vec3 p = m_new_points.at(i);
                glVertex3f(p.x, p.y, p.z);
            }
        }
        glEnd();


    }
    glPopMatrix();
}


Grid::Grid() : Grid(cgra::vec3(0.f,0.f,0.f)) {}

Grid::Grid(vec3 position) {
    m_points.clear();
    this->position = position;
    vec3 p = position;

    // blade 1
    m_points.push_back(vec3(1.f+p.x, 1.f+p.y, 0.f+p.z));
    m_points.push_back(vec3(0.f+p.x, 1.f+p.y, 0.f+p.z));
    m_points.push_back(vec3(1.f+p.x, 0.f+p.y, 0.f+p.z));

    m_new_points.push_back(vec3(1.f+p.x, 1.f+p.y, 0.f+p.z));
    m_new_points.push_back(vec3(0.f+p.x, 1.f+p.y, 0.f+p.z));
    m_new_points.push_back(vec3(1.f+p.x, 0.f+p.y, 0.f+p.z));

    //bottom face
    m_points.push_back(vec3(0.f+p.x, 0.f+p.y, 0.f+p.z));
    m_points.push_back(vec3(1.f+p.x, 0.f+p.y, 0.f+p.z));
    m_points.push_back(vec3(0.f+p.x, 1.f+p.y, 0.f+p.z));

    m_new_points.push_back(vec3(0.f+p.x, 0.f+p.y, 0.f+p.z));
    m_new_points.push_back(vec3(1.f+p.x, 0.f+p.y, 0.f+p.z));
    m_new_points.push_back(vec3(0.f+p.x, 1.f+p.y, 0.f+p.z));


}
