//
// Created by cuan on 5/31/16.
//

#include "field.h"



void Field::BuildVBOs()
{
    // Generate And Bind The Vertex Buffer
    glGenBuffers( 1, &m_nVBOVertices);                  // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );         // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_points->size() * sizeof(float) * 3, m_points->data(), GL_STATIC_DRAW_ARB );

    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB( 1, &m_nVBOTexCoords );                 // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );        // Bind The Buffer
    // Load The Data
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_uvs->size() * sizeof(float) * 2, m_uvs->data(), GL_STATIC_DRAW_ARB );


    // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
    //m_points->clear(); delete(m_points); m_points = NULL;
    //m_uvs->clear(); delete(m_uvs); m_uvs = NULL;
}

void Field::generateCluster(int num_clusters) {

    for(int i=0; i<num_clusters; i++){
        for(int j=0; j<num_clusters; j++){

            float x = float(i)-num_clusters/2;
            float y = float(j)-num_clusters/2;
            //float x = float(i) +float((rand()%100)/200.f) -num_clusters/2;
            //float y = float(j) +float((rand()%100)/200.f) -num_clusters/2;
            Grass grass(cgra::vec3(x,y,0.f));
            grass_clusters->push_back(grass);


            for(auto& point :grass.getPoints()){
                m_points->push_back(point);

               // m_centers->push_back(grass.getPosition());
            }
            for(auto& uv :grass.getUvs()){
                m_uvs->push_back(uv);
            }
            //m_centers->push_back(grass.getPosition());


        }
    }

    BuildVBOs();

    /*for(auto &p: *m_points){
        std::cout << p << "\n";
    }*/

}

void Field::renderField(WaveGenerator* wave_gen, float time) {

    for(auto &g: *grass_clusters){

            cgra::vec2 g_pos (g.getPosition().x,g.getPosition().y);
            cgra::vec3 shear_amount = wave_gen->getGroupTotal(g_pos, time);

            g.setShear(shear_amount);
            g.renderGeometry();

    }

}

void Field::renderFieldShader(WaveGenerator *wave_gen, float time, GLint shader) {




    glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    GLint loc = glGetAttribLocationARB(shader,"height");

    glEnableVertexAttribArrayARB(loc);

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
    glVertexPointer( 3, GL_FLOAT, 0, 0);       // Set The Vertex Pointer To The Vertex Buffer

    glVertexAttribPointerARB(loc,1,GL_FLOAT,0,0,m_centers->data());

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
    glTexCoordPointer( 2, GL_FLOAT, 0, 0);     // Set The TexCoord Pointer To The TexCoord Buffer

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);


    glPushMatrix();
    {
        glRotatef(-90,1,0,0);
        glDrawArrays( GL_TRIANGLES, 0, m_points->size() );	// Draw All Of The Triangles At Once
    }
    glPopMatrix();

    // Disable Pointers
    glDisableClientState( GL_VERTEX_ARRAY );					// Disable Vertex Arrays
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );				// Disable Texture Coord Arrays

    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);

}

void Field::generateGrid(int num_clusters) {

    for(int i=0; i<num_clusters; i++){
        for(int j=0; j<num_clusters; j++){

            float x = float(i)-num_clusters/2;
            float y = float(j)-num_clusters/2;
            //float x = float(i) +float((rand()%100)/200.f) -num_clusters/2;
            //float y = float(j) +float((rand()%100)/200.f) -num_clusters/2;

            Grid grid(cgra::vec3(x,y,0.f));
            grid_clusters->push_back(grid);

        }
    }
}

void Field::renderGrid(WaveGenerator *wave_gen, float time) {
    for(auto &g: *grid_clusters){
        //glPushMatrix();
        //{

            int i = -1;
            for(auto &vert: g.m_points)
            {
                i++;
                cgra::vec2 g_pos (vert.x,vert.y);
                cgra::vec3 move_amount = wave_gen->getGroupTotal(g_pos, time);

                g.m_new_points.at(i).x = vert.x+ move_amount.x;
                g.m_new_points.at(i).y = vert.y + move_amount.y;
                g.m_new_points.at(i).z = vert.z + move_amount.z;


            }
            g.renderGeometry();
        //}
        //glPopMatrix();
    }
}
