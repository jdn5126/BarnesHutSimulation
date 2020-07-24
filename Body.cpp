/*
  * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
  *
  * BarnesHutSimulation: Body.cpp
  */

#include <iostream>

#include "Body.h"

// gravitational constant
const double Body::G = 6.674 * pow(10, -11);

// static planar scales
const double Body::xScale = 1.0;
const double Body::yScale = 1.0;
const double Body::zScale = 1.0;

/* Constructor */
Body::Body(double mass, double pos[3], double acc[3], double vel[3]) {
	this->mass = mass;
	this->pos[0] = pos[0];
	this->pos[1] = pos[1];
	this->pos[2] = pos[2];
	this->acc[0] = acc[0];
	this->acc[1] = acc[1];
	this->acc[2] = acc[2];
	this->vel[0] = vel[0];
	this->vel[1] = vel[1];
	this->vel[2] = vel[2];
}

/* Print body details */
void Body::print(int i, double t) {
	std::cout << "Body " << i << " at time " << t << ": " << std::endl;
	std::cout << "..mass = " << this->mass <<std::endl;
	std::cout << "..pos = <" << this->pos[0] << ", " << this->pos[1] << ", " << this->pos[2] << ">" << std::endl;
	std::cout << "..acc = <" << this->acc[0] << ", " << this->acc[1] << ", " << this->acc[2] << ">" << std::endl;
	std::cout << "..vel = <" << this->vel[0] << ", " << this->vel[1] << ", " << this->vel[2] << ">" << std::endl;
}

/* Calculate the distance between this body and body b */
double Body::distance(Body* b) {
	// d = sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
	double xDiff = (b->pos[0] - this->pos[0]) * Body::xScale;
	double yDiff = (b->pos[1] - this->pos[1]) * Body::yScale;
	double zDiff = (b->pos[2] - this->pos[2]) * Body::zScale;
	return sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));
}

/* Calculate the gravitational force vector on body b produced by this body */
double* Body::grav_force(Body* b) {
	double* force = new double[3];
	double mag = 0;  // force magnitude
	double dist = this->distance(b);
	if (dist != 0) {
		// f_mag = (G * m1 * m2) / (d^2)
		mag = (Body::G * this->mass * b->mass) / pow(dist, 2);
		// f_x = ((x1 - x2) / d) * f_mag
		force[0] = ((this->pos[0] - b->pos[0]) * Body::xScale) / dist * mag;
		force[1] = ((this->pos[1] - b->pos[1]) * Body::yScale) / dist * mag;
		force[2] = ((this->pos[2] - b->pos[2]) * Body::zScale) / dist * mag;
	}
	else {
		force[0] = 0;
		force[1] = 0;
		force[2] = 0;
	}
	return force;
}

/* Apply force vector f on this body by updating acceleration vector */
void Body::apply_force(double f[3]) {
	// f = ma --> a = f / m
	this->acc[0] += (f[0] / this->mass);
	this->acc[1] += (f[1] / this->mass);
	this->acc[2] += (f[2] / this->mass);
}

/* Simulate movement of this body for t seconds given initial acceleration and velocity */
void Body::sim_movement(double t) {
	// update to final position (Newton's Second Equation of Motion)
	double temp = 0.5 * pow(t, 2);
	this->pos[0] += (this->vel[0] * t) + (this->acc[0] * temp);
	this->pos[1] += (this->vel[1] * t) + (this->acc[1] * temp);
	this->pos[2] += (this->vel[2] * t) + (this->acc[2] * temp);
	// update to final velocity (Newton's First Equation of Motion)
	this->vel[0] += (this->acc[0] * t);
	this->vel[1] += (this->acc[1] * t);
	this->vel[2] += (this->acc[2] * t);
	// acceleration remains constant
}

/* Calculate the center of mass position for the given array of bodies */
double* Body::center_of_mass(Body* bodies[], int n) {
	double totalMass = 0;
	double* cm = new double[3];
	cm[0] = 0;
	cm[1] = 0;
	cm[2] = 0;
	for (int i = 0; i < n; i++) {
		cm[0] += (bodies[i]->mass * bodies[i]->pos[0]);
		cm[1] += (bodies[i]->mass * bodies[i]->pos[1]);
		cm[2] += (bodies[i]->mass * bodies[i]->pos[2]);
		totalMass += bodies[i]->mass;
	}
	cm[0] /= totalMass;
	cm[1] /= totalMass;
	cm[2] /= totalMass;
	return cm;
}

/* Demonstrates usage of Body class for gravitational force calculations and movement simulations */
void Body::demo(void) {
	const int n = 4;
	const int t = 1;
	const int steps = 2;

	// generate n bodies with some mass and initial position (no initial acceleration or velocity)
	Body* bodies[n];
	Body* bodies_orig[n];
	double zero[3] = { 0, 0, 0 };
	double mass1 = 50000000000;
	double mass2 = 100000000000;
	double mass3 = 100000000000;
	double mass4 = 150000000000;
	double pos1[3] = { 0, 0, 0 };
	double pos2[3] = { 5, 0, 0 };
	double pos3[3] = { 0, 5, 0 };
	double pos4[3] = { 5, 5, 0 };
	bodies[0] = new Body(mass1, pos1, zero, zero);
	bodies_orig[0] = new Body(mass1, pos1, zero, zero);
	bodies[1] = new Body(mass2, pos2, zero, zero);
	bodies_orig[1] = new Body(mass2, pos2, zero, zero);
	bodies[2] = new Body(mass3, pos3, zero, zero);
	bodies_orig[2] = new Body(mass3, pos3, zero, zero);
	bodies[3] = new Body(mass4, pos4, zero, zero);
	bodies_orig[3] = new Body(mass4, pos4, zero, zero);

	// calculate starting center of mass position of all n bodies
	double* cm_orig = Body::center_of_mass(bodies, n);

	// simulate movement on all bodies for t seconds per time step
	for (int i = 0; i < steps; i++) {

		// sequentially calculate all pairwise gravitational forces and apply them,
		// this will update all bodies' acceleration vectors in prep for next movement sim
		for (int k = 0; k < n; k++) {
			for (int j = 0; j < n; j++) {
				if (k != j) {
					double* f = bodies[j]->grav_force(bodies[k]); // force on k produced by j
					bodies[k]->apply_force(f); // apply force on k
				}
			}
		}

		// simulate movement of time step
		for (int j = 0; j < n; j++) {
			bodies[j]->sim_movement(t);
		}

	}

	// all time steps completed, print some before and after result
	std::cout.precision(20);
	for (int i = 0; i < n; i++) {
		bodies_orig[i]->print(i, 0);
		bodies[i]->print(i, steps);
	}

	// calculate final center of mass position of all n bodies
	double* cm = Body::center_of_mass(bodies, n);
	std::cout << "CM_first = <" << cm[0] << ", " << cm[1] << ", " << cm[2] << ">" << std::endl;
	std::cout << "CM_final = <" << cm[0] << ", " << cm[1] << ", " << cm[2] << ">" << std::endl;
}