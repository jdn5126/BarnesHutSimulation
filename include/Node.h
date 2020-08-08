/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Node.h
 */

#ifndef _NODE_DEFINED
#define _NODE_DEFINED

#include "Body.h"

constexpr int OCT_REGIONS = 8;

/* calculate average of 2 vectors */
inline vector_3d average(const vector_3d &lowerBound, const vector_3d &upperBound) {
    double xPos = (std::get<X>(lowerBound) + std::get<X>(upperBound)) / 2;
    double yPos = (std::get<Y>(lowerBound) + std::get<Y>(upperBound)) / 2;
    double zPos = (std::get<Z>(lowerBound) + std::get<Z>(upperBound)) / 2;
    return (vector_3d)std::make_tuple(xPos, yPos, zPos);
}

/* Generic OctTree node */
class Node {

public:
    Node *parent;  // parent node
    int octet;     // parent octect that Node consumes

    Node(Node *parent);
    virtual ~Node() {};

    /* Is this node a leaf? */
    virtual bool isLeaf() = 0;

};

/* OctTree root node */
class Root : public Node {

public:
    vector_3d pos;           // middle position of section
    vector_3d lowerBound;    // lower bound of section
    vector_3d upperBound;    // upper bound of section
    double size;             // width region bounds
    double mass;             // total mass of bodies within section
    vector_3d centerOfMass;  // center of mass position of all bodies within section
    int numChildren;         // number of children nodes
    Node **children;         // children nodes (leaves and/or internal roots)

    Root(Node *parent, vector_3d lowerBound, vector_3d upperBound);
    ~Root();

    /* Override "<<" operator for printing Root details to I/O output stream */
    friend std::ostream& operator<<(std::ostream& out, const Root& r);

    /* This is not a leaf */
    bool isLeaf() { return false; }

};

/* OctTree leaf node */
class Leaf : public Node {

public:
    Body body;  // physical body representation

    Leaf(Node *parent, Body &&body);

    /* Override "<<" operator for printing leaf details to I/O output stream */
    friend std::ostream& operator<<(std::ostream& out, const Leaf& l);

    /* This is a leaf */
    bool isLeaf() { return true; }

    /* Calculate the distance between this and leaf */
    double leafDistance(Leaf *leaf);

    /* Calculate the distance between this and root's center of mass */
    double rootDistance(Root *root);

    /* Calculate force vector produced on this by root's mass and center of mass values */
    vector_3d rootForce(Root *root, double dist);

};

#endif // _NODE_DEFINED
