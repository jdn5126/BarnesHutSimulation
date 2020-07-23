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
	for (int i = 0; i < 3; i++) {
		this->pos[i] = pos[i];
		this->acc[i] = acc[i];
		this->vel[i] = vel[i];
	}
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
