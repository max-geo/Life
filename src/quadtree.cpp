#include "quadtree.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <unordered_set>

using namespace::std;


Quadtree::Quadtree(int p_length) {
    length = p_length;
    max_level = log2(length);

    
    dead = new Node{
        {nullptr, nullptr, nullptr, nullptr}, 0, 0, nullptr
    };

    alive = new Node{
        {nullptr, nullptr, nullptr, nullptr}, 1, 0, nullptr
    };

    Node* child = dead; 

    for (int k = 0; k < max_level; k++){
        array<Node*, 4> children{child, child, child, child};
        child = join(children);
    }
    root = child;
}

Node* Quadtree::join(array<Node*, 4> arr){
    if (table.contains(arr)) return table.at(arr);
    else {
        int new_level = arr[0]->level + 1;

        int new_population = 0; 
        for (int i = 0; i < arr.size(); i++) {
            new_population += arr[i]->population;
        }
        
        Node* n = new Node{arr, new_population, new_level};
        table.emplace(arr, n);

        return n;
    }
}

void Quadtree::insert(Point p) { // NOTE: no check for put of bounds, such a value clamps to maximum, so lower right corner 
    Node* curr_quad = root; 
    Node* prev_quad;

    int curr_len = length;

    int curr_x = p.x;
    int curr_y = p.y;

    int curr_card;
    
    struct PathStep {
        Node* node;
        int cardinality;
    };

    vector<PathStep> path;  

    // goes down to node representing point p, stores in last `curr_quad`
    for (int i = max_level; i > 0; i--){
        bool right_side = curr_x >= curr_len/2;
        bool bottom_side = curr_y >= curr_len/2; 

        curr_card = right_side + 2 * bottom_side;

        prev_quad = curr_quad;

        curr_quad = curr_quad->children[curr_card];
        path.push_back({prev_quad, curr_card});

        if (right_side) curr_x -= curr_len/2;
        if (bottom_side) curr_y -= curr_len/2;
        curr_len /= 2;
    }
    
    curr_quad = alive;

    // goes back up, rebuilding nodes and emplacing them into the table
    for (int k = 0; k < max_level; k++){
        array<Node*, 4> arr;
        for (int i = 0; i < 4; i++){
            if (i == curr_card) {
                arr[i] = curr_quad;
            } else arr[i] = prev_quad->children[i];
        }
        curr_quad = join(arr);
        path.pop_back();
        if (path.size() != 0){ // NOTE: feels like this is only a patch
            prev_quad = path.back().node;
            curr_card = path.back().cardinality;
        }

        if (k == max_level - 1) root = curr_quad;
    }
}

void Quadtree::recurse(vector<Cell>& cells, Node* n, Point curr_position, float curr_length) {
    if (n->children[0] != nullptr) {
        // NOTE: idea for pruning if I remove paddings   
        // if (n->population == pow(2, 2*n->level)) {
        //     cells.push_back({curr_position.x, curr_position.y, curr_length});

        // } else 
        for (int i = 0; i < 4; i++) {
            Node* c = n->children[i];
            if (c->population > 0){
                Point new_position = {curr_position.x + (i & 1) * curr_length/2, curr_position.y + (i>>1) * curr_length/2}; // bit trick
                recurse(cells, c, new_position, curr_length/2);
            }
        }

    } else {
        cells.push_back({curr_position.x, curr_position.y, curr_length});
    }
}

vector<Cell> Quadtree::aliveCells() {
    vector<Cell> cells; 
    recurse(cells, root, {0, 0}, length);
    return cells;
}