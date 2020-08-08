/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: OctTree.h
 */

#ifndef _OCTTREE_DEFINED
#define _OCTTREE_DEFINED

#include <vector>
#include "Node.h"

constexpr double THETA = 0.9;  // Barnes-Hut Parameter

// Data structure representing OctTree for Barnes-Hut Simulation
class OctTree {

private:
    Root *root;
    bool parallel;

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

    // Helper functions to calculate force on particle using tree
    vector_3d treeForce(Leaf *particle);
    vector_3d partialTreeForce(Leaf *particle, Node *node);

    // Helper function to check if a particle has moved out of its root's bounds
    bool checkParticleBounds(Leaf *particle);

    // Helper functions to print Tree
    void print();
    void printRecurse(Root *root);

};

#endif // _OCTTREE_DEFINED
