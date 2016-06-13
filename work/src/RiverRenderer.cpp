#include "RiverRenderer.hpp"

#include <vector>
#include "opengl.hpp"
#include "cgra_math.hpp"

using namespace std;
using namespace cgra;

static const float RAISE = 0.01;

void renderRivers(vector<vector<riverPathData>> riverData, bool velocityMode) {
    glColor4f(0, 0, 1, 0.9);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < riverData.size(); i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < riverData[i].size(); j++) {
            riverPathData data = riverData[i][j];

            vec3 min = data.minCoords, max = data.maxCoords;
            vec3 start1(min.x, min.y, data.position.z+RAISE);
            vec3 start2(max.x, min.y, data.position.z+RAISE);
            vec3 end1(min.x, max.y, data.next.z+RAISE);
            vec3 end2(max.x, max.y, data.next.z+RAISE);

            vec3 dir = data.next - data.position;
            if (dir.x < 0 || dir.y < 0) {
                start1.z = start2.z = data.next.z + RAISE;
                end1.z = end2.z = data.position.z + RAISE;
            }

            if (velocityMode) {
                vec3 flow = dir * data.water;
                glColor3f(0, flow.x, flow.y);
            }
            
            glVertex3f(start1.x, start1.y, start1.z);
            glVertex3f(start2.x, start2.y, start2.z);
            glVertex3f(end1.x, end1.y, end1.z);
            glVertex3f(end2.x, end2.y, end2.z);
        }
        glEnd();
    }
}