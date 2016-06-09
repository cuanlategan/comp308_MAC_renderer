//
//  geom.cpp
//  
//
//  Created by Adrian Cochrane on 7/06/16.
//
//

#include "geom.hpp"

#include <math.h>
#include <float.h>
#include <cstdlib>

using namespace cgra;
using namespace std;

/* Utility functions */
static const float 𝞃 = float(M_PI*2);

inline float len2(vec2 v) {
    return v.x*v.x + v.y*v.y;
}

inline float anglev(vec2 v) {
    return atan2(v.y, v.x);
}

inline float randf() {
    return float(rand())/RAND_MAX;
}

inline float range(float lo, float hi) {
    return lo + (hi-lo)*randf();
}

inline vec2 toVec2(float θ, float l) {
    return vec2(cos(θ)*l, sin(θ)*l);
}

inline float clamp(float x, float lo, float hi) {
    if (x <= lo) {
        return lo;
    } else if (x >= hi) {
        return hi;
    } else {
        return x;
    }
}

float integralOfDistToCircle(float x, arc arc) {
    if (arc.r < FLT_EPSILON) return 0.0;
    
    float sin_x = sin(x);
    float dsin_x = arc.d*sin_x;
    float y = clamp(sin_x*arc.d/arc.r, -1, 1);
    float θ = asin(y);
    
    float k = arc.sign, r = arc.r, d = arc.d;
    return (r*(r*(x + k*θ) + k*cos(θ)*dsin_x) + d*cos(x)*dsin_x)*0.5;
}

/* arc methods */
arc::arc(vec2 _P, float _r, float _sign) {
    P = _P;
    r = _r;
    sign = _sign;
}

void arc::cache(vec2 pt, float 𝛼) {
    vec2 v = P - pt;
    d2 = len2(v);
    d = sqrt(d2);
    r2 = r*r;
    θ = anglev(v);
    integralAtStart = integralOfDistToCircle(𝛼 - θ, *this);
}

bool arc::eq(arc other) {
    return P == other.P && r == other.r && sign == other.sign;
}

/* sector methods */
sector::sector(vec2 pt, float _a1, float _a2, arc arc1, arc arc2) {
    P = pt;
    a1 = _a1;
    a2 = _a2;
    
    arc1.cache(P, a1);
    arc2.cache(P, a1);
    arcs[0] = arc1;
    arcs[1] = arc2;
    
    area = calcAreaToAngle(a2);
}

float sector::_distToCurve(float angle, int index) {
    float 𝛼 = angle - arcs[index].θ;
    float sin_𝛼 = sin(𝛼);
    float t0 = arcs[index].r2 - arcs[index].d2*sin_𝛼*sin_𝛼;
    
    if (t0 < 0) {
        return arcs[index].d*cos(𝛼);
    } else {
        return arcs[index].d*cos(𝛼) + arcs[index].sign*sqrt(t0);
    }
}


// generate() & utils
//
float sector::calcAreaToAngle(float 𝛼) {
    float underInner = integralOfDistToCircle(𝛼 - arcs[0].θ, arcs[0])
                - arcs[0].integralAtStart;
    float underOuter = integralOfDistToCircle(𝛼 - arcs[1].θ, arcs[1])
                - arcs[1].integralAtStart;

    return underOuter - underInner;
}

float sector::calcAngleForArea(float area) {
    float lo = a1, hi = a2;
    float cur = range(hi, lo);
    
    for (int i = 0; i < 10; i++) {
        if (calcAreaToAngle(cur) < area) {
            lo = cur;
            cur = (cur + hi)*0.5;
        } else {
            hi = cur;
            cur = (lo + cur)*0.5;
        }
    }
    
    return cur;
}

vec2 sector::generate() {
    float 𝛼 = calcAngleForArea(area*randf());
    float d1 = _distToCurve(𝛼, 0);
    float d2 = _distToCurve(𝛼, 1);
    float d = sqrt(range(d1*d1, d2*d2));
    
    return P + toVec2(𝛼, d);
}

// clip() & utils
//
float sector::canonizeAngle(float 𝛼) {
    float Δ = fmod(𝛼 - a1, 𝞃);
    if (Δ < 0) Δ += 𝞃;
    return Δ + a1;
}

vec2 sector::distToCircle(float angle, vec2 C, float r) {
    vec2 v = C - P;
    float d2 = len2(v);
    float θ = anglev(v);
    float 𝛼 = angle - θ;
    float sin_𝛼 = sin(𝛼);
    float x2 = r*r - d2*sin_𝛼*sin_𝛼;
    
    if (x2 < 0) {
        return vec2(-10000000000, -10000000000);
    } else {
        float a = sqrt(d2)*cos(𝛼);
        float x = sqrt(x);
        return vec2(a-x, a+x);
    }
}

void sector::clip(vec2 C, float r, vector<sector> *regions) {
    vector<float> angles;
    
    vec2 v = C - P;
    float d = length(v);
    
    if (r < d) {
        float θ = anglev(v);
        float x = sqrt(d*d - r*r);
        float 𝛼 = asin(r/d);
        
        float angle = canonizeAngle(θ+𝛼);
        if (a1 < angle && angle < a2) {
            if (_distToCurve(angle, 0) < x && x < _distToCurve(angle, 1)) {
                angles.push_back(angle);
            }
        }
        
        angle = canonizeAngle(θ-𝛼);
        if (a1 < angle && angle < a2) {
            if (_distToCurve(angle, 0) < x && x < _distToCurve(angle, 1)) {
                angles.push_back(angle);
            }
        }
    }
    
    for (int i = 0; i < 2; i++) {
        vec2 C2 = arcs[i].P;
        float R = arcs[i].r;
        vec2 v = C - C2;
        float d = length(v);
        
        if (d > FLT_EPSILON) {
            float invD = 1.0f/d;
            float x = (d*d - r*r + R*R)*invD*0.5;
            float k = R*R - x*x;
            
            if (k>0) {
                float y = sqrt(k);
                vec2 V = v*invD;
                
                vec2 w(C2.x + V.x*x - V.y*y - P.x, C2.y + V.y*x + V.x*y - P.y);
                float angle = canonizeAngle(anglev(w));
                if (a1 < angle && angle < a2) angles.push_back(angle);
                
                w = vec2(C2.x + V.x*x + V.y*y - P.x, C2.y + V.y*x - V.x*y - P.y);
                angle = canonizeAngle(anglev(w));
                if (a1 < angle && angle < a2) angles.push_back(angle);
            }
        }
    }
    
    sort(angles.begin(), angles.end());
    angles.insert(angles.begin(), a1);
    angles.push_back(a2);
    
    for (int i = 1; i < angles.size(); i++) {
        float a1 = angles[i-1], a2 = angles[i];
        float midA = (a1+a2)*0.5;
        float inner = _distToCurve(midA, 0);
        float outer = _distToCurve(midA, 1);
        vec2 d = distToCircle(midA, C, r);
        
        if (d.y < inner || d.x > outer) {
            regions->push_back(sector(P, a1, a2, arcs[0], arcs[1]));
        } else {
            if (inner < d.x) {
                regions->push_back(sector(P, a1, a2, arcs[0], arc(C, r, -1)));
            }
            if (d.y > outer) {
                regions->push_back(sector(P, a1, a2, arc(C, r), arcs[1]));
            }
        }
    }
}

/* region methods */
static const float MIN_AREA = 0.00000001;

region::region(vec2 p, float r) {
    regions.push_back(sector(p, 0, 𝞃, arc(p, 2*r), arc(p, 4*r)));
    area = regions[0].area;
}

// generate()
//
vec2 region::generate() {
    float a = area*randf();
    for (int i = 0; i < regions.size(); i++) {
        if (a < regions[i].area) {
            return regions[i].generate();
        }
        a -= regions[i].area;
    }
    
    // UNREACHABLE
    if (!regions.size()) {
        cerr << "Fatal error, sampled from empty region." << endl;
    } else {
        cerr << "Fatal error, placed point " << a << " beyond boundaries, " << area << " of this region" << endl;
    }
    exit(1);
    return vec2(0, 0);
}

// clip()
//
void region::clip(vec2 C, float r) {
    vector<sector> newRegions;
    
    area = 0;
    for (int i = 0; i < regions.size(); i++) {
        sector region = regions[i];
        vector<sector> tmp;
        
        region.clip(C, r, &tmp);
        
        for (int j = 0; j < tmp.size(); j++) {
            sector ns = tmp[i];
            
            if (ns.area > MIN_AREA) {
                area += ns.area;
                
                // Collapse sectors if possible
                if (newRegions.size()) {
                    sector &last = newRegions[newRegions.size()-1];
                    if (last.a2 == ns.a1 &&
                        last.arcs[0].eq(ns.arcs[0]) && last.arcs[1].eq(ns.arcs[1])) {
                        last.a2 = ns.a2;
                        last.area = last.calcAreaToAngle(last.a2);
                        continue;
                    }
                }
                
                newRegions.push_back(ns);
            }
        }
    }
    
    regions = newRegions;
}