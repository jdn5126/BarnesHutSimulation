/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: main.cpp
 */

#include "Body.h"
#include "OctTree.h"
#include <vector>

int main()
{
    // Get command line args:
    //  steps     - number of time steps to simulate
    //  inputFile - path to input file, which contains simulation bounds and particles

    // Parse input file and construct vector of Leaf objects

    // Memory allocated for Leaf is owned by OctTree and is freed when OctTree is destructed.
    std::vector<Leaf *> particles = {
        new Leaf(nullptr,
                 Body(1, 1, std::make_tuple(4, 20, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(2, 1, std::make_tuple(7, 1, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(3, 1, std::make_tuple(7, 7, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(4, 1, std::make_tuple(11, 13, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(5, 1, std::make_tuple(16, 1, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(6, 1, std::make_tuple(19, 10, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(7, 1, std::make_tuple(19, 19, 0), zero_vect(), zero_vect())),
        new Leaf(nullptr,
                 Body(8, 1, std::make_tuple(22, 8, 0), zero_vect(), zero_vect()))
    };

    // Construct OctTree from vector of particles
    OctTree tree = OctTree(particles, std::make_tuple(0, 0, 0), std::make_tuple(24, 24, 0));

    // Update OctTree to cache center of mass for each octet at Root node
    tree.setCenterOfMass();

    tree.print();

    // Perform Barnes-Hut simulation for given number of time steps
    //
    // After each iteration, update OctTree structure and cached centers of mass
}
