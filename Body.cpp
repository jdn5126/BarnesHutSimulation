/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Body.cpp
 */

#include <iostream>

#include "Body.h"

/* Empty default constructor */
Body::Body() {
    this->id = 0;
    this->mass = 0.0;
    this->pos = zero_vect();
    this->acc = zero_vect();
    this->vel = zero_vect();
}

/* Partial constructor (no initial acceleration or velocity) */
Body::Body(int id, double mass, const vector_3d& pos) {
    this->id = id;
    this->mass = mass;
    this->pos = pos;
    this->acc = zero_vect();
    this->vel = zero_vect();
}

/* Full constructor */
Body::Body(int id, double mass, const vector_3d& pos, const vector_3d& acc, const vector_3d& vel) {
    this->id = id;
    this->mass = mass;
    this->pos = pos;
    this->acc = acc;
    this->vel = vel;
}

/* Override "<<" operator for printing body details to I/O output stream */
std::ostream& operator<<(std::ostream& out, const Body& b) {
    out << "Body " << b.id << ": " << std::endl;
    out << "..pos = <" << std::get<X>(b.pos) << ", " << std::get<Y>(b.pos) << ", " << std::get<Z>(b.pos) << ">" << std::endl;
    out << "..acc = <" << std::get<X>(b.acc) << ", " << std::get<Y>(b.acc) << ", " << std::get<Z>(b.acc) << ">" << std::endl;
    out << "..vel = <" << std::get<X>(b.vel) << ", " << std::get<Y>(b.vel) << ", " << std::get<Z>(b.vel) << ">" << std::endl;
    return out;
}

/* Calculate the distance between this body and body b */
double Body::distance(const Body& b) {
    // euclidean distance : d = sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
    double xDiff = (std::get<X>(b.pos) - std::get<X>(this->pos)) * xScale;
    double yDiff = (std::get<Y>(b.pos) - std::get<Y>(this->pos)) * yScale;
    double zDiff = (std::get<Z>(b.pos) - std::get<Z>(this->pos)) * zScale;
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));
}

/* Calculate the gravitational force vector on this body produced by body b,
   and apply that force on this body by updating the acceleration vector */
void Body::force(const Body& b) {
    vector_3d f = zero_vect();
    double mag = 0;
    double dist = this->distance(b);

    // first, calculate gravitational force vector if distance not zero, otherwise zero force
    if (dist != 0) {
        // f_mag = (G * m1 * m2) / (d^2)
        mag = (G * this->mass * b.mass) / pow(dist, 2);

        // f_x = ((x1 - x2) / d) * f_mag
        std::get<X>(f) = ((std::get<X>(b.pos) - std::get<X>(this->pos)) * xScale) / dist * mag;
        std::get<Y>(f) = ((std::get<Y>(b.pos) - std::get<Y>(this->pos)) * yScale) / dist * mag;
        std::get<Z>(f) = ((std::get<Z>(b.pos) - std::get<Z>(this->pos)) * zScale) / dist * mag;
    }

    // second, apply force vector f on this body by updating acceleration vector
    // f = ma --> a = f / m
    std::get<X>(this->acc) += (std::get<X>(f) / this->mass);
    std::get<Y>(this->acc) += (std::get<Y>(f) / this->mass);
    std::get<Z>(this->acc) += (std::get<Z>(f) / this->mass);
}

/* Simulate movement of this body for t seconds given initial acceleration and velocity */
void Body::move(double t) {
    // update position
    // Newton's Second Equation of Motion : x = x_0 + (v * t) + (0.5 * a * (t^2))
    double temp = 0.5 * pow(t, 2);
    std::get<X>(this->pos) += (std::get<X>(this->vel) * t) + (std::get<X>(this->acc) * temp);
    std::get<Y>(this->pos) += (std::get<Y>(this->vel) * t) + (std::get<Y>(this->acc) * temp);
    std::get<Z>(this->pos) += (std::get<Z>(this->vel) * t) + (std::get<Z>(this->acc) * temp);

    // update velocity
    // Newton's First Equation of Motion : v = v_0 + (a * t)
    std::get<X>(this->vel) += (std::get<X>(this->acc) * t);
    std::get<Y>(this->vel) += (std::get<Y>(this->acc) * t);
    std::get<Z>(this->vel) += (std::get<Z>(this->acc) * t);

    // acceleration remains constant
}

/* Demonstrates usage of Body class for gravitational force calculations and movement simulations */
void Body::demo(void) {
    constexpr int n = 4;
    constexpr int t = 1;
    constexpr int steps = 2;

    constexpr double mass[n] = {
        50000000000,
        100000000000,
        100000000000,
        150000000000
    };

    const vector_3d pos[n] = {
        std::make_tuple(0, 0, 0),
        std::make_tuple(5, 0, 0),
        std::make_tuple(0, 5, 0),
        std::make_tuple(5, 5, 0)
    };

    // generate n bodies with some mass and initial position (no initial acceleration or velocity)
    Body bodies[n];
    Body bodies_orig[n];  // for comparison after simulated movement
    for (int i = 0; i < n; i++) {
        bodies[i] = Body(i+1, mass[i], pos[i], zero_vect(), zero_vect());
        bodies_orig[i] = Body(i+1, mass[i], pos[i], zero_vect(), zero_vect());
    }

    // calculate starting center of mass position of all n bodies
    Body cm1 = center_of_mass(bodies, n);

    // simulate movement on all bodies for t seconds per time step
    for (int i = 0; i < steps; i++) {

        // sequentially calculate all pairwise gravitational forces and apply them,
        // this will update all bodies' acceleration vectors in prep for next movement sim
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (j != k) {
                    bodies[j].force(bodies[k]);
                }
            }
        }

        // simulate movement of time step
        for (int j = 0; j < n; j++) {
            bodies[j].move(t);
        }

    }

    // all time steps completed, print some before and after result
    std::cout.precision(20);
    std::cout << "Bodies Before And After Movement:\n" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << bodies_orig[i];
        std::cout << bodies[i] << std::endl;
    }

    // calculate final center of mass position of all n bodies
    Body cm2 = center_of_mass(bodies, n);

    // print center of mass before and after results
    std::cout << "Center of Mass Before And After Movement:\n" << std::endl;
    std::cout << cm1;
    std::cout << cm2 << std::endl;
}
