//
// Created by cuan on 5/31/16.
//

#include "field.h"
#include "simple_image.hpp"
#include "geometry.hpp"
#include "RiverHandler.h"




void Field::BuildVBOs() {

    // Generate And Bind The Vertex Buffer
    glGenBuffers(1, &m_nVBOVertices);                  // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);         // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_points->size() * sizeof(float) * 3, m_points->data(), GL_DYNAMIC_DRAW_ARB);

    glGenBuffers(1, &m_nVBONormals);                  // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBONormals);         // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_normals->size() * sizeof(float) * 3, m_normals->data(), GL_DYNAMIC_DRAW_ARB);

    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB(1, &m_nVBOTexCoords);                 // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords);        // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_uvs->size() * sizeof(float) * 2, m_uvs->data(), GL_STATIC_DRAW_ARB);


    glGenBuffers(1, &g_center_attrib);                  // Get A Valid Name
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_center_attrib);         // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_centers->size() * sizeof(float) * 3, m_centers->data(), GL_DYNAMIC_DRAW_ARB);


    // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
    //m_points->clear(); delete(m_points); m_points = NULL;
    //m_uvs->clear(); delete(m_uvs); m_uvs = NULL;
}

cgra::vec3 Field::getRandomVertOnFace(cgra::vec3 p1, cgra::vec3 p2, cgra::vec3 p3) {

    //srand((unsigned) time(0));
    //srand(time(NULL));

    float a = float(rand()) / float(RAND_MAX);
    float b = float(rand()) / float(RAND_MAX);
    if (a + b > 1.0) {
        a = 1 - a;
        b = 1 - b;
    }
    float c = 1 - a - b;

    /*std::cout <<"cuan rand a: " << a << "\n";
    std::cout <<"cuan rand b: " << b << "\n";
    std::cout <<"cuan rand c: " << c << "\n";*/

    cgra::vec3 r1(p1.x * a, p1.y * a, p1.z * a);
    cgra::vec3 r2(p2.x * b, p2.y * b, p2.z * b);
    cgra::vec3 r3(p3.x * c, p3.y * c, p3.z * c);
    cgra::vec3 result(r1.x + r2.x + r3.x, r1.y + r2.y + r3.y, r1.z + r2.z + r3.z);


    return result;
}

bool Field::checkRiverFace(int triIndex,Geometry *geo , vector<int> riverTris) {


    if (find(riverTris.begin(), riverTris.end(), triIndex) != riverTris.end()) {
        //cout << "checkRiverFace: River face found\n";
        return true;
    }

    /*for(auto & tri: riverTris){
        cgra::vec3 rivPoint1 = tri.at(0);
        cgra::vec3 rivPoint2 = tri.at(1);
        cgra::vec3 rivPoint3 = tri.at(2);

        //cout << "rivPoint1: " << rivPoint1 << "  triPoint1: " << triPoint1 << endl;
        //cout << "rivPoint2: " << rivPoint2 << endl;
        //cout << "rivPoint3: " << rivPoint3 << endl;

        if((triPoint3.x == rivPoint3.x ))
        {
            cout << "checkRiverFace: River face found\n";
            return true;
        }
    }
*/

    return false;
}


void Field::generateCluster(Geometry *geo, RiverHandler *riverHandler) {

    int index = -1;
    for (auto &tri: geo->getTriangles()) {
        index++;

        cgra::vec3 triPoint1 = geo->getPoints().at(tri.v[0].p);
        cgra::vec3 triPoint2 = geo->getPoints().at(tri.v[1].p);
        cgra::vec3 triPoint3 = geo->getPoints().at(tri.v[2].p);

        if(!checkRiverFace(index, geo, riverHandler->returnRiverTriIndex()))
        {
            //float height = float(fabs(triPoint1.y - triPoint2.y));
            //float base = float(fabs(triPoint3.x - triPoint2.x));
            //float area = 0.5f * base * height;

            cgra::vec3 e1(triPoint2.x-triPoint1.x,triPoint2.y-triPoint1.y,triPoint2.z-triPoint1.z);
            cgra::vec3 e2(triPoint3.x-triPoint1.x,triPoint3.y-triPoint3.y,triPoint3.z-triPoint1.z);
            cgra::vec3 e3 = cgra::cross(e1,e2);
            float area = 0.5*cgra::length(e3);

            /*std::cout << "height: " << height << "\n";
            std::cout << "base: " << base << "\n";
            std::cout << "area: " << area << "\n";*/

            //cgra::vec3 ran = getRandomVertOnFace(triPoint1, triPoint2, triPoint3);
            for (float i = 0; i < area; i += 0.00003f) {
                cgra::vec3 ran = getRandomVertOnFace(triPoint1, triPoint2, triPoint3);

                cgra::vec3 rotatedPoint(ran.x, ran.y, ran.z);
                //std::cout <<"cuan rotated and scaled: " << rotatedPoint << "\n";

                Grass grass(rotatedPoint);
                grass_clusters->push_back(grass);


                for (auto &point :grass.getPoints()) {
                    m_points->push_back(point);

                    cgra::vec3 center(grass.getPosition().x,
                                      grass.getPosition().y,
                                      grass.getPosition().z);
                    m_centers->push_back(center);
                    //std::cout << center << "\n";
                }

                for (auto &uv :grass.getUvs()) {
                    m_uvs->push_back(uv);
                }
            }


        }

    }


    BuildVBOs();


}

void Field::generateCluster(int num_clusters) {

    for (int i = 0; i < num_clusters; i++) {
        for (int j = 0; j < num_clusters; j++) {

            float x = float(i);
            float y = float(j);
            //float x = float(i) - num_clusters / 2;
            //float y = float(j) - num_clusters / 2;
            //float x = float(i) +float((rand()%100)/200.f) -num_clusters/2;
            //float y = float(j) +float((rand()%100)/200.f) -num_clusters/2;
            Grass grass(cgra::vec3(x, y, 0.f));
            grass_clusters->push_back(grass);


            for (auto &point :grass.getPoints()) {
                m_points->push_back(point);

                cgra::vec3 center(grass.getPosition().x,
                                  grass.getPosition().y,
                                  grass.getPosition().z);
                m_centers->push_back(center);
                m_normals->push_back(cgra::vec3(0.f,1.f,0.f));
                //std::cout << center << "\n";
            }

            for (auto &uv :grass.getUvs()) {
                m_uvs->push_back(uv);
            }
            //m_centers->push_back(grass.getPosition());


        }
    }
    int max_attributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &max_attributes);
    std::cout << "GL_MAX_VERTEX_ATTRIBS_ARB " << max_attributes << "\n";
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
    std::cout << "GL_MAX_VERTEX_ATTRIBS " << max_attributes << "\n";

    BuildVBOs();

    /*for(auto &p: *m_points){
        std::cout << p << "\n";
    }*/

}

void Field::renderField(WaveGenerator *wave_gen, float time) {

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    for (auto &g: *grass_clusters) {

        cgra::vec2 g_pos(g.getPosition().x, g.getPosition().y);
        cgra::vec3 shear_amount = wave_gen->getGroupTotal(g_pos, time);

        g.setShear(shear_amount);
        g.renderGeometry();

    }

    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);


}

void Field::renderFieldShader(WaveGenerator *wave_gen, float time, GLint shader) {

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);


    glUniform1f(glGetUniformLocation(shader, "time"), time); // set time uniform in shaders
    glUniform1f(glGetUniformLocation(shader, "wavelength"), wavelength);
    glUniform1f(glGetUniformLocation(shader, "amplitude"), amplitude);
    glUniform1f(glGetUniformLocation(shader, "steepnes"), steepnes);
    glUniform1f(glGetUniformLocation(shader, "speed"), speed);
    glUniform1i(glGetUniformLocation(shader, "hasTex"), 1);

    //GLfloat dir[2] = {1.f, 1.f};
    //glUniform2fv(glGetUniformLocation(shader, "direction"), 2, dir);

    glUniform1f(glGetUniformLocation(shader, "dirX"), dirX);
    glUniform1f(glGetUniformLocation(shader, "dirY"), dirY);

    glEnableClientState(GL_VERTEX_ARRAY);                        // Enable Vertex Arrays
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices);
    glVertexPointer(3, GL_FLOAT, 0, 0);       // Set The Vertex Pointer To The Vertex Buffer

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBONormals);
    glNormalPointer(GL_FLOAT, 0, 0);     // Set The Normal Pointer To The TexCoord Buffer

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);     // Set The TexCoord Pointer To The TexCoord Buffer

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, g_center_attrib);
    glEnableVertexAttribArrayARB(6);
    glVertexAttribPointerARB(6, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_grass_tex);
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);

    /* glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, g_centers_tex);
     glUniform1i(glGetUniformLocation(shader, "centers"), 1);
 */

    glPushMatrix();
    {
        //glTranslatef(0,0,-.5);
        glRotatef(-90, 1, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, m_points->size());    // Draw All Of The Triangles At Once
    }
    glPopMatrix();

    // Disable Pointers
    glDisableClientState(GL_VERTEX_ARRAY);                    // Disable Vertex Arrays
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);// Disable Texture Coord Arrays

    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);


}

void Field::generateGrid(int num_clusters) {

    for (int i = 0; i < num_clusters; i++) {
        for (int j = 0; j < num_clusters; j++) {

            float x = float(i) - num_clusters / 2;
            float y = float(j) - num_clusters / 2;
            //float x = float(i) +float((rand()%100)/200.f) -num_clusters/2;
            //float y = float(j) +float((rand()%100)/200.f) -num_clusters/2;

            Grid grid(cgra::vec3(x, y, 0.f));
            grid_clusters->push_back(grid);

        }
    }
}

void Field::renderGrid(WaveGenerator *wave_gen, float time) {
    for (auto &g: *grid_clusters) {
        //glPushMatrix();
        //{

        int i = -1;
        for (auto &vert: g.m_points) {
            i++;
            cgra::vec2 g_pos(vert.x, vert.y);
            cgra::vec3 move_amount = wave_gen->getGroupTotal(g_pos, time);

            g.m_new_points.at(i).x = vert.x + move_amount.x;
            g.m_new_points.at(i).y = vert.y + move_amount.y;
            g.m_new_points.at(i).z = vert.z + move_amount.z;


        }
        g.renderGeometry();
        //}
        //glPopMatrix();
    }
}

void Field::initGrassTexture() {


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





