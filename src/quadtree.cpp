#include "quadtree.h"

#include <iostream>
#include <vector>
#include <memory>

using namespace::std;


Quadtree::Quadtree(float dimension, Point center) {
    border = BoundingBox { center, dimension/2.0f};
};

bool Quadtree::insert(Point p) {

    Quadtree* curr_quad = this;
    
    while (curr_quad->children[0] != nullptr){
            for (auto c : curr_quad->children) {
                if (c->in_border(p)) {
                    curr_quad = c;
                    break;
                }
            }
    } // guarantees we're at lowest level
    
    if (!curr_quad->point.has_value()) { // just place it here if its empty
        curr_quad->point = p;
        return true; 
        
    } else { // otherwise split into 4 children and redistribute
        Point old_point = curr_quad->point.value();
        if (old_point == p) return false;
        curr_quad->init_children();
        curr_quad->point.reset();
        curr_quad->insert(old_point);
        
        return curr_quad->insert(p);
    }
};

bool Quadtree::remove(Point p) {
    if (!in_border(p)) return false;

    Quadtree* curr_quad = this;
    while (curr_quad->children[0] != nullptr) { 
        for (auto c : curr_quad->children) {
            if (c->in_border(p)) {
                curr_quad = c; 
            }
        }
    }
    curr_quad->point.reset();
        
    return true;
}

// point Quadtree::search(point p) {};

void Quadtree::init_children() {
    float hs = border.half_dimension;
    Point center = border.center;
    float offset = hs / 2;

    NW = make_unique<Quadtree>(hs, Point{center.x - offset, center.y + offset});
    NE = make_unique<Quadtree>(hs, Point{center.x + offset, center.y + offset});
    SE = make_unique<Quadtree>(hs, Point{center.x + offset, center.y - offset});
    SW = make_unique<Quadtree>(hs, Point{center.x - offset, center.y - offset});

    children[0] = NW.get();
    children[1] = NE.get();
    children[2] = SE.get();
    children[3] = SW.get();
};

bool Quadtree::in_border(Point p) {
    float offset = border.half_dimension;
    Point center = border.center;

    Point NW = {center.x - offset, center.y + offset};
    Point SE = {center.x + offset, center.y - offset};

    if (NW.x <= p.x && p.x < SE.x && SE.y <= p.y && p.y < NW.y) {
        return true;
    }

    return false;
};

bool Quadtree::is_empty() {
    return !point.has_value();
}

void Quadtree::print_points() {
    if (point) {
        cout << "x: " << point->x << " y: " << point->y << endl; 
    }
}

void Quadtree::print_layers(Quadtree* q, int layer) {
    if (q == nullptr) return;

    cout << "Layer: " << layer
         << ", points: " << q->point.has_value() << endl;

    q->print_points();

    for (auto c : q->children) {
        if (c == nullptr) continue;
        print_layers(c, layer + 1);
    }
}

