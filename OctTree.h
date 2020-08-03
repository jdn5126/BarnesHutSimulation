/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: OctTree.h
 */

#ifndef _OCTTREE_DEFINED
#define _OCTTREE_DEFINED

#include <vector>
#include "Body.h"

class Node {
public:
    virtual bool isLeaf() = 0;
    Node(Node *parent);
protected:
    Node *parent;
}; // end class Node

class Leaf : public Node {
public:
    Leaf(Node *parent, Body *body);
    bool isLeaf() {
        return true;
    }
    friend std::ostream& operator<<(std::ostream& out, const Leaf& f);
    // TODO: make member variables private
    Body *body;
}; // end class Leaf

class Root : public Node {
public:
    Root(Node *parent, vector_3d lowerBound, vector_3d upperBound);
    ~Root();
    bool isLeaf() {
        return false;
    }
    friend std::ostream& operator<<(std::ostream& out, const Root& r);
    // TODO: make member variables private
    vector_3d pos;
    vector_3d lowerBound;
    vector_3d upperBound;
    std::vector<Node *> children;
}; // end class Root

// Data structure representing OctTree for Barnes-Hut Simulation
class OctTree {
public:
    OctTree(std::vector<Body *> &particles, vector_3d lowerBound, vector_3d upperBound);
    ~OctTree();

    // Helper function to insert particle into Tree
    void insert(Root *root, Leaf *particle);

    // Helper function to find octet to insert particle into
    int findOctet(Root *root, Body *particle);

    // Helper function to print Tree
    void print();
    void printRecurse(Root *root);
private:
    Root *root;
}; // end class OctTree

#endif // _OCTTREE_DEFINED
