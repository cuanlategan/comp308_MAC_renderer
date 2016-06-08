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
    fill = new int[gridSize*gridSize];
}

// Accessor
//
vector<vec2> sampler::allPoints() {
    vector<vec2> points;
    for (int i = 0; i < gridSize*gridSize; i++) {
        for (int j = 0; j < fill[i]; j++) {
            points.push_back(grid[i][j].P);
        }
    }
    return points;
}

// Vector utilities
//
int find(vector<*cell> haystack, cell needle) {
    for (int i = 0; i < haystack.size(); i++) {
        if (haystack[i] == needle) {
            return i;
        }
    }
    return -1;
}

void remove(vector<*cell> *list, int i) {
    list[i] = list[list.size()-1];
    list->pop_back()
}

// Largely internal utilities
//
vector<*cell> findNeighbours(vec2 pt, float dist) {
    float dist2 = dist*dist;
    int gx, gy, N = int(ceil(dist/gridCellSize));
    if (N>(gridSize*gridSize)) N = gridSize*gridSize;
    
    vector<*cell> neighbours;
    getGridXY(pt, &gx, &gy);
    
    for (int j=-N; j<=N; j++) {
        for (int i=-N; i<=N; i++) {
            int cx = (gx+i+gridSize)%gridSize, cy = (gy+j+gridSize)%gridSize;
            cell *bucket = grid[cy*gridSize + cx];
            
            for (int k = 0; k < fill[cy*gridSize+cx]; k++) {
                if (getDist2(pt, bucket[k]) < dist2) {
                    neighbours.push_back(&bucket[k])
                }
            }
        }
    }
    
    return neighbours;
}

void sampler::add(vec2 pt) {
    region rgn(pt, R);
    
    vector<*cell> neighbours = findNeighbours();
    for (int i = 0; i < neighbours.size(); i++) {
        cell *neigh = neighbours[i];
        rgn.clip(neigh->P, 4*R);
        neigh.boundary->clip(rgn, 2*R);
        
        int c = find(candidates, neigh);
        if (neigh.boundary->isEmpty() && c != -1) {
            remove(candidates, c);
        }
    }
    
    int gx, gy;
    getGridXY(pt, &gx, &gy);
    int *i = fill[gy*gridSize + gx];
    if (*i < MAX_PER_CELL) {
        grid[gy*gridSize + gx][*i] = cell(pt, region(pt, R));
        (*i)++;
    }
    
    if (!rgn.isEmpty()) {
        candidates.push_back(&item);
    }
}

// Public interface
//
void sampler::fillSpace() {
    if (!hasPoints) {
        add(vec2(2*randf()-1, 2*randf()-1));
        candidates.push_back(pt);
    }

    while (candidates.size()) {
        // TODO weight towards larger surface areas
        int c = rand()%candidates.size();
        cell cell = candidates[c];
	
        add(candidate.sample());
    }
}