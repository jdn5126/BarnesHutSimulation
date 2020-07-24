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
	double pos[3];  // center of mass position vector: <x, y, z>
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

    /* Calculate the center of mass position for the given array of bodies */
    static double* center_of_mass(Body* bodies[], int n);

	/* Demonstrates usage of Body class */
	static void demo(void);

};

#endif // _BODY_DEFINED