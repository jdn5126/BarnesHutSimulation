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
    // Generate particle inputs and 3D bounds
    vector_3d lowerBound = std::make_tuple(0, 0, 0);
    vector_3d upperBound = std::make_tuple(24, 24, 0);

    // Construct a set of Leaf pointers that encapsulate each particle.
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

    // Construct OctTree
    OctTree tree = OctTree(particles, lowerBound, upperBound);

    // Print OctTree
    tree.print();
    std::cout << std::endl;

    // Update particle position to change octets
    particles[0]->body.pos = std::make_tuple(10,13,0);

    // Remove particle from tree and re-insert
    tree.remove(particles[0]);
    tree.print();
    std::cout << std::endl;
    tree.insert(particles[0]);
    tree.print();
    std::cout << std::endl;
    tree.remove(particles[0]);
    tree.print();

    //std::cout << std::endl;
    //Body::demo();
}
