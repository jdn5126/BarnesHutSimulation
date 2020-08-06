/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: main.cpp
 */

#include "Body.h"
#include "OctTree.h"
#include <fstream>
#include <string>
#include <vector>

// Read simulation bounds from input file
vector_3d
getBounds(std::ifstream &f) {
    double x, y, z;
    f >> x >> y >> z;
    return std::make_tuple(x, y, z);
}

int main(int argc, char *argv[]) {
    // Get command line args:
    //  steps     - number of time steps to simulate
    //  inputFile - path to input file, which contains simulation bounds and particles
    if (argc < 3) {
        std::cerr << "Usage: ./barnesHut <steps> <filename>" << std::endl;
        exit(-1);
    }
    // Parse number of time steps to execute simulation for
    int steps;
    try {
        steps = std::stoi(argv[1]);
    } catch (std::invalid_argument const &e) {
        std::cerr << "invalid integer: " << argv[1] << std::endl;
        exit(-1);
    } catch (std::out_of_range const &e) {
        std::cerr << "integer out of range" << std::endl;
        exit(-1);
    }
    // Open input file
    std::ifstream infile;
    infile.open(argv[2], std::ios::in);
    if (!infile.is_open()) {
        std::cerr << "Unable to open " << argv[2] << std::endl;
        exit(-1);
    }

    // Parse input file and construct vector of Leaf objects
    int numParticles;
    infile >> numParticles;
    vector_3d lowerBound = getBounds(infile);
    vector_3d upperBound = getBounds(infile);
    std::vector<Leaf *> particles(numParticles);
    for (int i=0; i < numParticles; i++) {
        particles[i] = new Leaf(nullptr, getBody(infile));
    }
    infile.close();

    // Construct OctTree from vector of particles
    OctTree tree = OctTree(particles, lowerBound, upperBound);

    // Update OctTree to cache center of mass for each octet at Root node
    tree.setCenterOfMass();

    tree.print();

    // Perform Barnes-Hut simulation for given number of time steps
    //
    // After each iteration, update OctTree structure and cached centers of mass
}
