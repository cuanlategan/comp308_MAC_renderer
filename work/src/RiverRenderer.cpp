#include "RiverRenderer.hpp"

#include <vector>
#include "opengl.hpp"
#include "cgra_math.hpp"

using namespace std;
using namespace cgra;

void renderRivers(vector<vector<riverSegment>> riverData, bool velocityMode) {
    glColor4f(0, 1, 1, 0.8);
    for (int i = 0; i < riverData.size(); i++) {
        glBegin(GL_QUADS);
        for (int j = 0; j < riverData[i].size(); j++) {
            riverSegment data = riverData[i][j];

            if (velocityMode) {
                vec2 flow = data.dir * data.flow;
                glColor3f(0, flow.x, flow.y);
            }
            
            glVertex3f(data.start.x - data.startWidth.x,
                       data.start.y - data.startWidth.y, data.start.z);
            glVertex3f(data.start.x + data.startWidth.x,
                       data.start.y + data.startWidth.y, data.start.z);
            glVertex3f(data.end.x - data.endWidth.x, data.end.y - data.endWidth.y,
                       data.end.z);
            glVertex3f(data.end.x + data.endWidth.x, data.end.y + data.endWidth.y,
                       data.end.z);
        }
        glEnd();
    }
}