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

struct coord {
    int x, y;
    short z;
    
    coord(int _x, int _y, int _z) {
        x = _x; y = _y; z = _z;
    }
    
    cell *lookup(cell (*grid)[MAX_PER_CELL], int rowSize) {
        return &grid[y*rowSize + x][z];
    }
    
    bool eq(coord other) {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct sampler {
    short int *fill;
    cell (*grid)[MAX_PER_CELL];
    int gridSize;
    float gridCellSize;
    float R;

    std::vector<coord> candidates;
    bool hasPoints;
    
    sampler(float radius);
    std::vector<cgra::vec2> allPoints();
    void fillSpace();
    
private:
    std::vector<coord> findNeighbours(cgra::vec2, float);
    void add(cgra::vec2);
    void getGridXY(cgra::vec2, int*, int*);
};