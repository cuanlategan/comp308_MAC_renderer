#include "cgra_math.hpp"

struct riverSegment {
    cgra::vec3 start;
    cgra::vec2 startWidth;
    cgra::vec3 end;
    cgra::vec2 endWidth;
    float flow;
    cgra::vec2 dir;
    
    riverSegment(cgra::vec3 _start, cgra::vec2 _startWidth,
                 cgra::vec3 _end, cgra::vec2 _endWidth, 
                 float _flow, cgra::vec2 _dir) {
        start = _start;
        startWidth = _startWidth;
        end = _end;
        endWidth = _endWidth;
        flow = _flow;
        dir = _dir;
    }
};

void renderRivers(std::vector<std::vector<riverSegment>> riverData,
                  bool velocityMode = false);