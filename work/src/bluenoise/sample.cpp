//
//  sample.cpp
//  
//
//  Created by Adrian Cochrane on 8/06/16.
//
//

#include "sample.hpp"

using namespace cgra;
using namespace std;

// Constructor
//
sampler::sampler(float radius) {
    R = radius;
    
    gridSize = int(ceil(2.0/(4.0*R)));
    if (gridSize) gridSize = 2;
    
    gridCellSize = 2.0/gridSize;
    grid = new cell[gridSize*gridSize][MAX_PER_CELL];
    fill = new short int[gridSize*gridSize];
}


// Vector utilities
//
int find(vector<coord> haystack, coord needle) {
    for (int i = 0; i < haystack.size(); i++) {
        if (haystack[i].eq(needle)) {
            return i;
        }
    }
    return -1;
}

void remove(vector<coord> *list, int i) {
    list[i] = list[list->size()-1];
    list->pop_back();
}

// Largely internal utilities
//
void sampler::getGridXY(vec2 pt, int *gx, int *gy) {
    *gx = int(0.5*(pt.x+1)*gridSize);
    *gy = int(0.5*(pt.y+1)*gridSize);

    if (*gx < 0 || *gx >= gridSize || *gy < 0 || *gy >= gridSize) {
        cerr << "Internal error, point outside grid was generated, ignoring" << endl;
    }
}

vector<coord> sampler::findNeighbours(vec2 pt, float dist) {
    float dist2 = dist*dist;
    int gx, gy, N = int(ceil(dist/gridCellSize));
    if (N>(gridSize*gridSize)) N = gridSize*gridSize;
    
    vector<coord> neighbours;
    getGridXY(pt, &gx, &gy);
    
    for (int j=-N; j<=N; j++) {
        for (int i=-N; i<=N; i++) {
            int cx = (gx+i+gridSize)%gridSize, cy = (gy+j+gridSize)%gridSize;
            cell *bucket = grid[cy*gridSize + cx];
            
            for (int k = 0; k < fill[cy*gridSize+cx]; k++) {
                if (len2(pt - bucket[k].P) < dist2) {
                    neighbours.push_back(coord(cx, cy, k));
                }
            }
        }
    }
    
    return neighbours;
}

void sampler::add(vec2 pt) {
    region rgn(pt, R);
    
    vector<coord> neighbours = findNeighbours(pt, 8*R);
    for (int i = 0; i < neighbours.size(); i++) {
        coord index = neighbours[i];
        cell *neigh = index.lookup(grid, gridSize);
        rgn.clip(neigh->P, 4*R);
        neigh->boundary.clip(pt, 2*R);
        
        int c;
        if (neigh->boundary.isEmpty() && (c = find(candidates, index)) != -1) {
            remove(&candidates, c);
        }
    }
    
    int gx, gy;
    getGridXY(pt, &gx, &gy);
    short int *i = &fill[gy*gridSize + gx];
    if (*i < MAX_PER_CELL) {
        grid[gy*gridSize + gx][*i] = cell(pt, region(pt, R));
        (*i)++;
    }
    
    if (!rgn.isEmpty()) {
        candidates.push_back(coord(gx, gy, (*i)-1));
    }
}

// Public interface
//
void sampler::fillSpace() {
    if (!hasPoints) {
        add(vec2(2*randf()-1, 2*randf()-1));
    }


    int i = 0;
    while (candidates.size() && i < 100) {
        // TODO weight towards larger surface areas
        int c = rand()%candidates.size();
        cell *candidate = candidates[c].lookup(grid, gridSize);

        add(candidate->boundary.generate());
        i++;

    }
}

vector<vec2> sampler::allPoints() {
    vector<vec2> points;
    for (int i = 0; i < gridSize*gridSize; i++) {
        for (int j = 0; j < fill[i]; j++) {
            points.push_back(grid[i][j].P);
        }
    }
    return points;
}