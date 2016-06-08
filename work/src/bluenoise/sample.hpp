//
//  sample.hpp
//
//  Generates blue noise to place river sprites on the water surface.
//
//  Created by Adrian Cochrane on 8/06/16.
//
//

#pragma once

#include <vector>

#include "../cgra_math.hpp"
#include "geom.hpp"

#define MAX_PER_CELL 9

struct cell {
    cgra::vec2 P;
    region boundary;

    cell(cgra::vec2 _P, region _boundary) {
        P = _P; boundary = _boundary;
    }
    cell() {}
};

struct sampler {
    short int *fill;
    cell (*grid)[MAX_PER_CELL];
    int gridSize;
    float gridCellSize;
    float R;

    std::vector<cell*> candidates;
    bool hasPoints;
    
    sampler(float radius);
    std::vector<cgra::vec2> allPoints();
    void fillSpace();
    
private:
    std::vector<cell*> findNeighbours(cgra::vec2, float);
    void add(cgra::vec2);
    void getGridXY(cgra::vec2, int*, int*);
};