/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: inputGen.cpp
 */

#include "Body.h"
#include "Timer.h"
#include <ctime>
#include <math.h>
#include <fstream>

#define XRANGE (290*pow(10, 12)) // approximate width of solar system in meters
#define YRANGE (290*pow(10, 12))
#define ZRANGE (290*pow(10, 12))

#define MASS_MAX (1.9*pow(10, 27)) // mass of jupiter in kg
#define MAX_RADIUS (1.0*pow(10, 9)) // minimum allowable distance between particles in km

using namespace std;

// Creates random double in specified range
inline double randDoubleRange(double a, double b) {
    return ((b - a) * ((double)rand() / RAND_MAX)) + a;
}

// Checks that no Body is within MAX_RADIUS of another Body
bool bodiesInMinRange(Body bodies[], int index, Body newBody) {
    int i;
    for (i = 0; i < index; i++) {
        if (bodies[i].distance(newBody) < MAX_RADIUS) {
            return true;
        }
    }
    return false;
}

// Generates input and writes objects to a file
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "ERROR: please use the usage:" << endl;
        cout << "./inputGen <filename> <numbodies>" << endl;
        exit(-1);
    }

    Timer timer = Timer();
    timer.start();

    // Open output file
    ofstream outfilep;
    char *outputfile = argv[1];
    outfilep.open(outputfile, ios::out);

    if (!outfilep.is_open()) {
        cout << "ERROR: could not open file " << outputfile << endl;
        exit(-1);
    }

    // Write number of bodies to be generated to output file
    int num_bodies = stoi(argv[2]);
    outfilep << num_bodies << endl;
    // Write simulation bounds to output file
    outfilep << 0 << " " << 0 << " " << 0 << endl;
    outfilep << XRANGE << " " << YRANGE << " " << ZRANGE << endl;

    // Seed random number generator and print seed
    char *seed = getenv("RAND_SEED");
    if (seed == NULL) {
        time_t seedTime = time(NULL);
        cout << "Random Seed: " << seedTime << endl;
        srand(seedTime);
    } else {
        cout << "Random Seed: " << seed << endl;
        srand(stoi(seed));
    }

    // Generate Body objects and write to output file
    Body *bodies = new Body[num_bodies];
    vector_3d randPos, randVel, randAcc;
    double randMass, x, y, z;

    int i = 0;
    while (i < num_bodies) {
        /* make random coordinates/mass */
        x = randDoubleRange(0, XRANGE);
        y = randDoubleRange(0, YRANGE);
        z = randDoubleRange(0, ZRANGE);
        randPos = make_tuple(x, y, z);
        randMass = randDoubleRange(1, MASS_MAX);

        /* set velocity and accelation to 0 to begin with*/
        x = 0;
        y = 0;
        z = 0;
        randAcc = make_tuple(x, y, z);
        randVel = make_tuple(x, y, z);

        bodies[i] = Body(i + 1, randMass, randPos, randAcc, randVel);

        /* If Body is in range of another body, regenerate input
         * else log the generation*/
        if (bodiesInMinRange(bodies, i, bodies[i])) {
            continue;
        } else {
            bodies[i].logBody(outfilep);
            i++;
        }
    }
    outfilep.close();
    delete []bodies;

    timer.stop();
    cout << timer << endl;

    return 0;
}
