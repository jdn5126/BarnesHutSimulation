/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Node.cpp
 */

#include <iostream>
#include <fstream>

#include "Node.h"

Node::Node(Node *parent) {
    this->parent = parent;
}

Root::Root(Node *parent, vector_3d lowerBound, vector_3d upperBound) : Node(parent) {
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->size = std::get<X>(upperBound) - std::get<X>(lowerBound);
    this->pos = average(lowerBound, upperBound);
    this->children = new Node *[OCT_REGIONS];
    for (int i = 0; i < OCT_REGIONS; ++i) {
        this->children[i] = nullptr;
    }
    this->numChildren = 0;
    this->mass = 0;
    this->centerOfMass = zero_vect();
}

Root::~Root() {
    // Delete children
    // Commented out because causing seg fault
    // for (int i=0; i < 8; ++i) {
    //     delete this->children[i];
    // }
}

Leaf::Leaf(Node *parent, Body &&body) : Node(parent) {
    this->body = body;
}

/* Calculate the distance between this and leaf */
double
Leaf::leafDistance(Leaf *leaf) {
    // euclidean distance : d = sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
    double xDiff = (std::get<X>(leaf->body.pos) - std::get<X>(this->body.pos)) * xScale;
    double yDiff = (std::get<Y>(leaf->body.pos) - std::get<Y>(this->body.pos)) * yScale;
    double zDiff = (std::get<Z>(leaf->body.pos) - std::get<Z>(this->body.pos)) * zScale;
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));
}

/* Calculate the distance between this and root's center of mass */
double
Leaf::rootDistance(Root *root) {
    // euclidean distance : d = sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
    double xDiff = (std::get<X>(root->centerOfMass) - std::get<X>(this->body.pos)) * xScale;
    double yDiff = (std::get<Y>(root->centerOfMass) - std::get<Y>(this->body.pos)) * yScale;
    double zDiff = (std::get<Z>(root->centerOfMass) - std::get<Z>(this->body.pos)) * zScale;
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));
}

/* Calculate force vector produced on this by root's mass and center of mass values */
vector_3d
Leaf::rootForce(Root *root, double dist) {
    vector_3d f = zero_vect();
    double mag = 0;
    if (dist != 0) {
        // f_mag = (G * m1 * m2) / (d^2)
        mag = (G * this->body.mass * root->mass) / pow(dist, 2);
        // f_x = ((x1 - x2) / d) * f_mag
        std::get<X>(f) = ((std::get<X>(root->centerOfMass) - std::get<X>(this->body.pos)) * xScale) / dist * mag;
        std::get<Y>(f) = ((std::get<Y>(root->centerOfMass) - std::get<Y>(this->body.pos)) * yScale) / dist * mag;
        std::get<Z>(f) = ((std::get<Z>(root->centerOfMass) - std::get<Z>(this->body.pos)) * zScale) / dist * mag;
    }
    return f;
}

/* Override "<<" operator for printing Root details to I/O output stream */
std::ostream& operator<<(std::ostream& out, const Root& r) {
    out << "Root(" << std::get<X>(r.pos) << ", " << std::get<Y>(r.pos) << ", " <<
        std::get<Z>(r.pos) << ") mass: " << r.mass << " center of mass: (" <<
        std::get<X>(r.centerOfMass) << ", " << std::get<Y>(r.centerOfMass) << ", " <<
        std::get<Z>(r.centerOfMass) << ")";
    return out;
}

/* Override "<<" operator for printing leaf details to I/O output stream */
std::ostream& operator<<(std::ostream& out, const Leaf& l) {
    out << "Leaf " << l.body.id << ": (" << std::get<X>(l.body.pos) << ", " <<
        std::get<Y>(l.body.pos) << ", " << std::get<Z>(l.body.pos) << ")";
    return out;
}
