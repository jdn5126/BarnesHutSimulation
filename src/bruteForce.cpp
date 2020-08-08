/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: main.cpp
 */

#include "Body.h"
#include "Node.h"
#include "Timer.h"
#include <fstream>
#include <string>
#include <vector>

constexpr bool DEBUG = true;  // print debug output
constexpr int DELTA = 1;      // length of time step: 1 second

/* Read simulation bounds from input file */
vector_3d getBounds(std::ifstream &f) {
    double x, y, z;
    f >> x >> y >> z;
    return std::make_tuple(x, y, z);
}

int main(int argc, char *argv[]) {
    // Get command line args:
    //  steps     - number of time steps to simulate
    //  inputFile - path to input file, which contains simulation bounds and particles
    //  outputFile - path to output file, where simulation results will be written
    if (argc < 3) {
        std::cerr << "Usage: ./barnesHut <steps> <input_filename> <output_filename>" << std::endl;
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

    if (DEBUG) {
        std::cout << "Time Steps: " << steps << std::endl;
        std::cout << "Input File: " << argv[2] << std::endl;
        std::cout << "Output File: " << argv[3] << std::endl;
    }

    // Open input file
    std::ifstream infile;
    infile.open(argv[2], std::ios::in);
    if (!infile.is_open()) {
        std::cerr << "Unable to open " << argv[2] << std::endl;
        exit(-1);
    }

    // Open output file
    std::ofstream outfile;
    outfile.open(argv[3], std::ios::out);
    if (!outfile.is_open()) {
        std::cerr << "Unable to open " << argv[3] << std::endl;
        exit(-1);
    }

    // Parse input file and construct vector of Leaf objects
    int numParticles;
    infile >> numParticles;
    getBounds(infile); // lower bounds not used
    getBounds(infile); // upper bounds not used
    std::vector<Leaf *> particles(numParticles);
    for (int i=0; i < numParticles; i++) {
        particles[i] = new Leaf(nullptr, getBody(infile));
    }
    infile.close();

    // Write initial positions
    outfile << numParticles << std::endl;
    outfile << steps << std::endl;
    for (int i = 0; i < numParticles; i++) {
        outfile << 0 << " ";
        particles[i]->body.logBody(outfile);
    }

    // Start timer
    Timer timer = Timer();
    timer.start();

    // simulate movement on all bodies for given number of time steps
    for (int i = 0; i < steps; i++) {
        // sequentially calculate all pairwise gravitational forces and apply them,
        // this will update all bodies' acceleration vectors in prep for next movement sim
        for (int j = 0; j < numParticles; j++) {
            for (int k = 0; k < numParticles; k++) {
                if (j != k) {
                    particles[j]->body.force(particles[k]->body);
                }
            }
        }
        // simulate movement of time step
        for (int j = 0; j < numParticles; j++) {
            particles[j]->body.move(DELTA);
        }
        // log new positions to file
        for (int j = 0; j < numParticles; j++) {
            outfile << i+1 << " ";
            particles[j]->body.logBody(outfile);
        }
    }

    timer.stop();
    std::cout << timer << std::endl;
    outfile << timer.duration() << std::endl;

    outfile.close();

}
