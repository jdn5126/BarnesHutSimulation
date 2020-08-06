#include "Body.h"
#include <math.h>
#include <fstream>

#define XRANGE (290*pow(10, 12)) // apx. width of the solar system in meters
#define YRANGE (290*pow(10, 12))
#define ZRANGE (290*pow(10, 12))

#define MASS_MAX (1.9*pow(10,27)) // mass of jupiter in kg ( make it the max )

#define MAXR (1.0*pow(10, 9)) //min allowable distance generated between masses 1,000,000 km

using namespace std;


/*Creates a Random Double Floating Point Integer in the 
 *specified range*/
inline double randDoubleRange(double a, double b) {
    return ((b - a) * ((double)rand() / RAND_MAX)) + a;
}

/*Checks if inputs were generated already in the
 *bodies array that are in the range by MAXR*/
bool bodiesInMinRange(Body bodies[], int index, Body newBody) {
    int i;

    // TODO: could parallelize this loop
    for (i = 0; i < index; i++) {
        if (bodies[i].distance(newBody) < MAXR) {
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
    // Open output file
    ofstream outfilep;
    char *outputfile = argv[1];
    outfilep.open(outputfile, ios::out);

    if (!outfilep.is_open()) {
        cout << "ERROR: could not open file " << outputfile << endl;
        exit(-1);
    }
    // Write number of bodies to be generated to output file
    int num_bodies = atoi(argv[2]);
    outfilep << num_bodies << endl;
    // Write simulation bounds to output file
    outfilep << 0 << " " << 0 << " " << 0 << endl;
    outfilep << XRANGE << " " << YRANGE << " " << 0 << endl;

    // Generate Body objects and write to output file
    Body *bodies = new Body[num_bodies];
    vector_3d randPos, randVel, randAcc;
    double randMass, x, y, z;
    int randExp;

    int i = 0;
    while (i < num_bodies) {
        /* make random coordinates/mass */
        x = randDoubleRange(0, XRANGE);
        y = randDoubleRange(0, YRANGE);
        z = 0;
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

    return 0;
}
