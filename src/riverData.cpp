#include "riverData.hpp"

using namespace std;
using namespace cgra;

// Quad tree implementation
void extend(vector<river> a, vector<river> b) {
    for (int i = 0; i < b.size(); i++) a.push_back(b[i]);
}

void quadtree::lookup(bbox bbox, vector<river> results) {
    extend(results, rivers);
    
    if (bbox.topleft.x < center.x && bbox.topleft.y < center.y) {
        if (ne != nullptr) ne->lookup(bbox, results);
        
        if (bbox.topleft.x + bbox.width >= center.x && nw != nullptr) nw->lookup(bbox, results);
        if (bbox.topleft.y + bbox.height >= center.y && se != nullptr) se->lookup(bbox, results);
        if (bbox.topleft.x + bbox.width >= center.x && bbox.topleft.y + bbox.height >= center.y
            && sw != nullptr) {
            sw->lookup(bbox, results);
        }
    }
    if (bbox.topleft.x >= center.x && bbox.topleft.y < center.y) {
        if (nw != nullptr) nw->lookup(bbox, results);
        
        if (bbox.topleft.y + bbox.height >= center.y && sw != nullptr) sw->lookup(bbox, results);
    }
    if (bbox.topleft.x < center.x && bbox.topleft.y >= center.y) {
        if (se != nullptr) se->lookup(bbox, results);
        
        if (bbox.topleft.x + bbox.width >= center.x && sw != nullptr) sw->lookup(bbox, results);
    }
    if (bbox.topleft.x >= center.x && bbox.topleft.y >= center.y) {
        if (sw != nullptr) sw->lookup(bbox, results);
    }
}

void quadtree::add(river river) {
    bbox bbox = river.bbox;
    
    if (bbox.topleft.x < center.x && bbox.topleft.x + bbox.width < center.x &&
        bbox.topleft.y < center.y && bbox.topleft.y + bbox.height < center.y) {
        if (ne == nullptr) ne = new quadtree(center - size/4, size/2);
        ne->add(river);
    } else if (bbox.topleft.x >= center.x &&
               bbox.topleft.y < center.y && bbox.topleft.y + bbox.height < center.y) {
        if (nw == nullptr) {
            nw = new quadtree(vec2(center.x + (size/4).x, center.y - (size/4).y), size/2);
        }
        nw->add(river);
    } else if (bbox.topleft.x < center.x && bbox.topleft.x + bbox.width < center.x
               && bbox.topleft.y >= center.y) {
        if (se == nullptr) {
            se = new quadtree(vec2(center.x - (size/4).x, center.y + (size/4).y), size/2);
        }
        se->add(river);
    } else if (bbox.topleft.x >= center.x && bbox.topleft.y >= center.y) {
        if (sw == nullptr) sw = new quadtree(center + size/4, size/2);
        sw->add(river);
    } else {
        rivers.push_back(river);
    }
}