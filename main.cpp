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
    // Generate input
    vector_3d lowerBound = std::make_tuple(0, 0, 0);
    vector_3d upperBound = std::make_tuple(24, 24, 0);
    std::vector<Body *> particles = {
        new Body(1, 1, std::make_tuple(4, 20, 0), zero_vect(), zero_vect() ),
        new Body(2, 1, std::make_tuple(7, 1, 0), zero_vect(), zero_vect() ),
        new Body(3, 1, std::make_tuple(7, 7, 0), zero_vect(), zero_vect() ),
        new Body(4, 1, std::make_tuple(11, 13, 0), zero_vect(), zero_vect() ),
        new Body(5, 1, std::make_tuple(16, 1, 0), zero_vect(), zero_vect() ),
        new Body(6, 1, std::make_tuple(19, 10, 0), zero_vect(), zero_vect() ),
        new Body(7, 1, std::make_tuple(19, 19, 0), zero_vect(), zero_vect() ),
        new Body(8, 1, std::make_tuple(22, 8, 0), zero_vect(), zero_vect() )
    };

    // Construct OctTree
    OctTree tree = OctTree(particles, lowerBound, upperBound);

    // Print OctTree
    tree.print();

    std::cout << std::endl;
    Body::demo();
}
