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
    virtual ~Node() {};
// Member variables left public for simplicity.
    Node *parent;
    int octet; // parent octet that Node consumes
}; // end class Node

class Leaf : public Node {
public:
    Leaf(Node *parent, Body &&body);
    bool isLeaf() {
        return true;
    }
    friend std::ostream& operator<<(std::ostream& out, const Leaf& f);
// Member variables left public for simplicity
    Body body;
}; // end class Leaf

class Root : public Node {
public:
    Root(Node *parent, vector_3d lowerBound, vector_3d upperBound);
    ~Root();
    bool isLeaf() {
        return false;
    }
    friend std::ostream& operator<<(std::ostream& out, const Root& r);

// Member variables left public for simplicity
    vector_3d pos;
    vector_3d lowerBound;
    vector_3d upperBound;
    double mass;
    vector_3d centerOfMass;
    int numChildren;
    Node **children;
}; // end class Root

// Data structure representing OctTree for Barnes-Hut Simulation
class OctTree {
public:
    OctTree(std::vector<Leaf *> &particles, vector_3d lowerBound,
            vector_3d upperBound);
    ~OctTree();

    // Helper functions to insert particles into Tree
    void insert(Leaf *particle);
    void insertParticle(Root *root, Leaf *particle, const int octet);
    void insertParticles(std::vector<Leaf *> &particles);

    // Helper function to find octet to insert particle into
    int findOctet(const vector_3d &rootPos, const vector_3d &bodyPos);

    // Helper function to remove Leaf from tree and rebalance tree
    void remove(Leaf *particle);

    // Helper function to maybe replace Root node with Leaf node
    void maybeReplaceRoot(Root *root);

    // Helper functions to set center of mass for each Root node
    void setCenterOfMass();
    void centerOfMass(Root *root);

    // Helper functions to print Tree
    void print();
    void printRecurse(Root *root);
private:
    Root *root;
}; // end class OctTree

#endif // _OCTTREE_DEFINED
