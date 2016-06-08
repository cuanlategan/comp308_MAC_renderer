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

sampler::sampler(float radius) {
    R = radius;
    
    gridSize = int(ceil(2.0/(4.0*R)));
    if (gridSize) gridSize = 2;
    
    gridCellSize = 2.0/gridSize;
    grid = new cell[gridSize*gridSize][MAX_PER_CELL];
    fill = new int[gridSize*gridSize];
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

void remove(vector<*cell> cs, int i) {
    candidates[c] = candidates[candidates.size()-1];
    candidates.pop_back();
}

void sampler::fillSpace() {
    if (!hasPoints) {
        add(vec2(2*random()-1, 2*random()-1));
	candidates.push_back((int) points.size()-1);
	// continue on to loop
    }

    while (candidates.size()) {
        int c = rand()%candidates.size();
	cell cell = candidates[c];
	remove(candidates, c);
	
	
    }
}