//
//  geom.hpp
//
//  Defines the geometry used to generate "blue noise"
//  for rendering rivers.
//
//  Created by Adrian Cochrane on 7/06/16.
//
//

#pragma once

#include "../cgra_math.hpp"

#include <vector>

/* Useful utilities, to be exposed to sample.cpp */
float randf();
float len2(cgra::vec2);

struct arc {
    cgra::vec2 P;
    float r, sign, d, θ, integralAtStart;
    float r2, d2;
    
    arc() {} /* quick fix to initialize an arc[2] */
    arc(cgra::vec2 P, float r, float sign = 1);
    void cache(cgra::vec2 pt, float angle);
    bool eq(arc other);
};

struct sector {
    cgra::vec2 P;
    float a1, a2, area;
    arc arcs[2];
    
    sector(cgra::vec2 pt, float a1, float a2, arc arc1, arc arc2);
    cgra::vec2 generate();
    void clip(cgra::vec2, float, std::vector<sector>*);

/* Only for use within the implementation */
    float calcAreaToAngle(float);

private:
    float _distToCurve(float, int);
    float calcAngleForArea(float);
    float canonizeAngle(float);
    cgra::vec2 distToCircle(float, cgra::vec2, float);
};

struct region {
    std::vector<sector> regions;
    float area;
    
    region(cgra::vec2 pt, float r);
    region() {}
    cgra::vec2 generate();
    void clip(cgra::vec2, float);
    bool isEmpty() {return regions.size() == 0;}
};