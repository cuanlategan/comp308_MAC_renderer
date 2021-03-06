#pragma once
#include <vector>
#include <map>
#include <random>
#include <cmath>


#include "RiverPoint.h"
#include "vVertexPoint.h"
#include "VoronoiHandler.h"
#include "simple_image.hpp"
#include "CImg.h"
#include "splineHandler.hpp"
#include "geometry.hpp"

using namespace std;
using namespace cgra;
using namespace cimg_library;

struct riverPathData {
	vec3 position;
	vec3 minCoords;
	vec3 maxCoords;
	float water;
	vec3 next;
};

class RiverHandler {
private:
	int density = 50;
	int numberOfRivers = 1;
	int maxRivers = numberOfRivers * 2;
	int cutoffPercent = 5;
	float startWater = 0;
	float waterScalar = 1;
	int widthScalar = 10;
	float zScalar = 0.3;
	int smoothPasses = 1;
	int maxPasses = 2;
	int complexMultiplier = density / 10;
	int riverSamples = max(5, ((smoothPasses * complexMultiplier) + 1));
	vector<int> sourcesUsed;
	vector<int> riverTriIndex;



	
	// int finalDensity = 600 - density - (numberOfRivers * (density / 4));
	int finalDensity = 50;

	//Image heightMap = Image("./work/res/textures/test_heightmap.png");
	Image *heightMap;
	//CImg<unsigned char> drawheightMap = CImg<unsigned char>("./work/res/textures/simplebump.pgm");
	//int imageSize = drawheightMap.width();
	int imageSize = 1024;
	Geometry *meshDisplay = nullptr;

	VoronoiHandler *graph;
	splineHandler *splineMaker;
	vector<vVertexPoint*> riverPoints;
	vector<vVertexPoint*> riverSources;
	vector<vector<vVertexPoint*>> rivers;
	vector<vTriangle*> riverTris;

	vector<vector<riverPathData>> riverData;

	CImg<unsigned char> lowRezDisplay;
	CImg<unsigned char> riverDisplay;
	CImg<unsigned char> fullMeshDisplay;

	int drawRadius = 2;
	//bool sortByZ(vVertexPoint*, vVertexPoint*) const;
	
	vector<vVertexPoint*> findSourceCandidates(vector<vVertexPoint*>);
	vector<vVertexPoint*> makeRiverPath(vVertexPoint*);
	vector<vector<vVertexPoint*>> makeRivers(int, vector<vVertexPoint*>);

	vVertexPoint* getNextRiverPoint(vVertexPoint*, vector<vVertexPoint*>*);
	//vVertexPoint* dealWithLocalMin(vVertexPoint*, vector<vVertexPoint*>*);

	
	void drawEdges(vector<vEdge*>, CImg<unsigned char>*, const unsigned char[]);
	void drawPoints(vector<vVertexPoint*>, CImg<unsigned char>*, const unsigned char[], const unsigned char[],int);
	
	void drawPolygons(vector<vTriangle*>, CImg<unsigned char>*, const unsigned char[], const unsigned char[], int);

	Geometry* makeGeo(vector<vTriangle*>);

	void carveRivers(vector<vector<vVertexPoint*>>, vector<vTriangle*>);

	vector<vVertexPoint*> makeRiverSpline(vector<vVertexPoint*>, vector<vec4>);

	void rebuildHeightData(Image*);

	vector<vector<riverPathData>> makeRiverPathData();
	
	

public:
	RiverHandler(int mapNum, int smoothPass );
	void drawAll();

	void drawLowRezMesh();
	void drawRiverMesh();
	void drawFullMesh();

	Geometry* getGeo();
	Geometry* makeGeo();
	vector<vector<vec3>> returnRiverTris();
	vector<int> returnRiverTriIndex();
	vector<vector<riverPathData>> returnRiverPathData();

	bool addRiver();
	bool subdivide();
	

	//vector <vector<river>> returnRiverData(vector<vector<vVertexPoint*>>);


};