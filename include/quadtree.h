#pragma once

#include <vector>
#include <memory>
#include <optional> 

struct Point { //TODO: check if float points fucks anything up
    float x;
    float y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct BoundingBox {
    Point center;
    float half_dimension;
};

class Quadtree {
public:
    Quadtree(float dimension, Point center);

    bool insert(Point p);
    bool remove(Point p);
    bool in_border(Point p);
    bool is_empty();

    Point search(Point p);

    void print_points();
    void print_layers(Quadtree* q, int layer);
    std::optional<Point> point;

private:
    BoundingBox border;

    void init_children();

    std::unique_ptr<Quadtree> NW;
    std::unique_ptr<Quadtree> NE;
    std::unique_ptr<Quadtree> SE;
    std::unique_ptr<Quadtree> SW;
    
    Quadtree* children[4] = {nullptr, nullptr, nullptr, nullptr}; 
};
