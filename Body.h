/*
  * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
  *
  * BarnesHutSimulation: Body.h
  */

#ifndef _BODY_DEFINED
#define _BODY_DEFINED

#include <iostream>
#include <cmath>

class Body {

public:
	const static double G;       // gravitational constant
	const static double xScale;  // scaling factor for x plane
	const static double yScale;  // scaling factor for y plane
	const static double zScale;  // scaling factor for z plane

	double mass;    // mass in kg
	double pos[3];  // position vector: <x, y, z>
	double acc[3];  // acceleration vector: <a_x, a_y, a_z>
	double vel[3];  // velocity vector: <v_x, v_y, v_z>

	Body(double mass, double pos[3], double acc[3], double vel[3]);

    /* Print body details */
    void print(int i, double t);

	/* Calculate the distance between this body and boyd b */
	double distance(Body* b);

	/* Calculate the gravitational force vector on body b produced by this body */
	double* grav_force(Body* b);

	/* Apply force vector f on this body by updating acceleration vector */
	void apply_force(double f[3]);

	/* Simulate movement of this body for t seconds given initial acceleration and velocity */
	void sim_movement(double t);

	/* Demonstrates usage of Body class for gravitational force calculations and movement simulations */
	static void demo(void) {
        const int n = 10;
        const int t = 1;
        double zero[3] = { 0, 0, 0 };

        // generate n bodies with some mass and initial position (no initial acceleration or velocity)
        Body* bodies[n];
        Body* bodies_orig[n];
        for (int i = 0; i < n; i++) {
            double mass = ((double)i + 1) * pow(10, 10);
            double pos[3] = { ((double)i + 1) * pow(10, i/2), ((double)i + 1) * pow(10, i/2), ((double)i + 1) * pow(10, i/2) };
            bodies[i] = new Body(mass, pos, zero, zero);
            bodies_orig[i] = new Body(mass, pos, zero, zero);
        }

        // sequentially calculate all pairwise gravitational forces and apply them,
        // this will update all bodies' acceleration vectors in prep for next movement sim
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    double* f = bodies[j]->grav_force(bodies[i]); // force on i produced by j
                    bodies[i]->apply_force(f); // apply force on i
                }
            }
        }

        // simulate movement on all bodies for t seconds
        for (int i = 0; i < n; i++) {
            bodies[i]->sim_movement(t);
        }

        // single timestep completed, print some before and after result
        std::cout.precision(20);
        for (int i = 0; i < n; i++) {
            bodies_orig[i]->print(i, 0);
            bodies[i]->print(i, t);
        }
	}

};

#endif // _BODY_DEFINED