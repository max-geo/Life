#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <memory>
#include <optional> 
#include <unordered_map>
#include <array>

struct Point {
    float x;
    float y;
};

struct Cell {
    float x, y, length;
};

struct Node {
    std::array<Node*, 4> children;

    int population;
    int level;

    Node* result; //middle half-size square after 2^(k-2) generations
};

struct NodeArrayHash {
    std::size_t operator()(const std::array<Node*, 4>& a) const noexcept {
        std::size_t h = 0;
        for (Node* p : a) {
            h ^= std::hash<Node*>{}(p) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        }
        return h;
    }
}; 

class Quadtree {
public:
    Quadtree(int p_length);
    
    void insert(Point p);
    
    std::vector<Cell> aliveCells();

private:
    int length;
    int max_level;

    Node* dead;
    Node* alive;
    Node* root = nullptr;
    
    Node* join(std::array<Node*, 4> arr);

    void recurse(std::vector<Cell>& cells, Node* n, Point curr_position, float curr_length);

    std::unordered_map<std::array<Node*, 4>, Node*, NodeArrayHash> table;
};
