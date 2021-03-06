#include "vVertexPoint.h"

// Constructors/Destructor

vVertexPoint::vVertexPoint() {
	coords = vec2(0, 0);
	zValue = 0.0f; 
	edges.clear();
	neighbours.clear();
	border = false;
	river = false;
	water = 0.0f;
	zValue = 0.0f;
	polyCenter = 0;
	downstream = nullptr;
}

vVertexPoint::vVertexPoint(vec2 coords) {
	this->coords = coords;
	zValue = 0.0f;
	edges.clear();
	neighbours.clear();
	border = false;
	river = false;
	water = 0.0f;
	zValue = 0.0f;
	polyCenter = 0;
	downstream = nullptr;
}

vVertexPoint::vVertexPoint(float x, float y) {
	this->coords = vec2(x, y);
	this->zValue = 0.0f;
	edges.clear();
	neighbours.clear();
	/*
	border = false;
	river = false;
	water = 0.0f;
	zValue = 0.0f;
	polyCenter = 0;
	downstream = nullptr;
	*/
}

vVertexPoint::vVertexPoint(vec2 coords, float zValue) {
	this->coords = coords;
	this->zValue = zValue;
	edges.clear();
	neighbours.clear();
	/*border = false;
	river = false;
	water = 0.0f;
	zValue = 0.0f;
	polyCenter = 0;
	downstream = nullptr;*/
}

vVertexPoint::vVertexPoint(float x, float y, float zValue) {
	this->coords = vec2(x, y);
	this->zValue = zValue;
	edges.clear();
	neighbours.clear();
	//border = false;
	//river = false;
	//water = 0.0f;
	//zValue = 0.0f;
	//polyCenter = 0;
	//downstream = nullptr;
}

vVertexPoint::~vVertexPoint() {

	for (vVertexPoint* n : neighbours) {
		n->removeNeighbour(this);
	}

	for (vTriangle *p : polys) {
		p->removeCorner(this);
	}

	for (vEdge *e : edges) {
		e->removeVertex(this);
	}

} 

vector<vVertexPoint*> vVertexPoint::getNeighbours() {
	return this->neighbours;
}

void vVertexPoint::addNeighbour(vVertexPoint* neighbour) {
	vector<vVertexPoint*>::iterator itr;
	itr = find(neighbours.begin(), neighbours.end(), neighbour);
	if (itr == neighbours.end())  this->neighbours.push_back(neighbour);
}

void vVertexPoint::removeNeighbour(vVertexPoint* neighbour) {
	vector<vVertexPoint*>::iterator itr;
	itr = find(neighbours.begin(), neighbours.end(), neighbour);
	if (itr != neighbours.end()) neighbours.erase(itr);
	// if (polys.size() == 0) delete this;
}

void vVertexPoint::removeEdge(vEdge* e) {
	vector<vEdge*>::iterator itr;
	itr = find(edges.begin(), edges.end(), e);
	if (itr != edges.end()) edges.erase(itr);
	// if (edges.size() == 0) delete this; // a vertex can be created without an edge, but can't be left without one.
}

void vVertexPoint::updateFlow(float flow) {
	water += flow;
	if (!border && downstream != nullptr) {
		//cout << "downstream = " << downstream;
		downstream->updateFlow(flow);
	}
}

// Set/Get
float vVertexPoint::getZValue() {
	return this->zValue;
}

void vVertexPoint::setZValue(float zValue) {
	this->zValue = zValue;
}

void vVertexPoint::applyWater(int waterScalar) {
	zValue = zValue - (water*waterScalar);
}

vec2 vVertexPoint::getCoords() {
	//cout << "Coords: " << coords << endl;
	return this->coords;
}
void vVertexPoint::setCoords(vec2 coords) {
	this->coords = coords;

}
void vVertexPoint::setCoords(float x, float y) {
	this->coords = vec2(x, y);
}

vector<vEdge*> vVertexPoint::getEdges() {
	return this->edges;
}
void vVertexPoint::addEdge(vEdge *edge) {
	this->edges.push_back(edge);
}

vector<vTriangle*> vVertexPoint::getPolys() {
	return this->polys;
}


void vVertexPoint::addPoly(vTriangle* poly) {
	this->polys.push_back(poly);
}

void vVertexPoint::removePoly(vTriangle* poly) {
	vector<vTriangle*>::iterator itr;
	itr = find(polys.begin(), polys.end(), poly);
	if (itr != polys.end()) polys.erase(itr);
	// if (polys.size() == 0) delete this;
}


void vVertexPoint::setPolyCenter(vTriangle* poly) {
	polyCenter = poly;
}
void vVertexPoint::removePolyCenter(vTriangle* poly) {
	polyCenter = 0;
}
vTriangle* vVertexPoint::getPolyCenter() {
	return polyCenter;
}

void vVertexPoint::setBorder(bool border) {
	this->border = border;
}

bool vVertexPoint::isBorder() {
	return this->border;
}

void vVertexPoint::setRiver(bool river) {
	this->river = river;
}

bool vVertexPoint::isRiver() {
	return this->river;
}

void vVertexPoint::setDownstream(vVertexPoint* downstream) {
	this->downstream = downstream;
}

vVertexPoint* vVertexPoint::getDownstream() {
	return this->downstream;
}

void vVertexPoint::setWater(float water) {
	this->water = water;
}
float vVertexPoint::getWater() {
	return this->water;
}

void vVertexPoint::setScreenCoords(int imageSize) {
	int x = coords.x * (imageSize - 1);
	int y = coords.y * (imageSize - 1);

	screenCoords = vec2(x, y);
}

float vVertexPoint::sampleWater() {

	float waterVal = 0;
	float maxWater = 0;
	if (!river) {
		int riverCount = 0;
		for (vVertexPoint *n : neighbours) {
			if ((n->isRiver() && n->getWater() > 0)) {
				riverCount++;
				waterVal += n->getWater();
				maxWater = max(maxWater, n->getWater());
				//cout << "Found a river neighbour, max water = " << maxWater << ", waterVal = " << waterVal << endl;
			}
				if (!n->isRiver() && n->getWater() > 0){
					waterVal += n->getWater();
					maxWater = max(maxWater, n->getWater());
					}
				}
			if (border) return maxWater;
			if (riverCount > 0) return (waterVal / riverCount);
			//return (waterVal / riverCount);
			return (maxWater / 3);
		}
		return water;
}