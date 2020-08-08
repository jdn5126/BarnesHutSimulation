/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Body.cpp
 */

#include <iostream>
#include <fstream>

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
    out << "..mass = "  << b.mass << std::endl;
    out << "..pos  = <" << std::get<X>(b.pos) << ", " << std::get<Y>(b.pos) << ", " << std::get<Z>(b.pos) << ">" << std::endl;
    out << "..acc  = <" << std::get<X>(b.acc) << ", " << std::get<Y>(b.acc) << ", " << std::get<Z>(b.acc) << ">" << std::endl;
    out << "..vel  = <" << std::get<X>(b.vel) << ", " << std::get<Y>(b.vel) << ", " << std::get<Z>(b.vel) << ">" << std::endl;
    return out;
}

/* Calculate the distance between this body and body b */
double
Body::distance(const Body& b) {
    // euclidean distance : d = sqrt((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)
    double xDiff = (std::get<X>(b.pos) - std::get<X>(this->pos)) * xScale;
    double yDiff = (std::get<Y>(b.pos) - std::get<Y>(this->pos)) * yScale;
    double zDiff = (std::get<Z>(b.pos) - std::get<Z>(this->pos)) * zScale;
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2) + pow(zDiff, 2));
}

/* Calculate the gravitational force vector on this body produced by body b */
vector_3d
Body::force(const Body& b) {
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

    return f;
}

/* Apply force vector on this body by updating acceleration vector */
void
Body::apply(const vector_3d f) {
    // f = ma --> a = f / m
    std::get<X>(this->acc) += (std::get<X>(f) / this->mass);
    std::get<Y>(this->acc) += (std::get<Y>(f) / this->mass);
    std::get<Z>(this->acc) += (std::get<Z>(f) / this->mass);
}

/* Simulate movement of this body for t seconds given initial acceleration and velocity */
void
Body::move(double t) {
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

/* Logs this body to a file */
void Body::logBody(std::ofstream& f) {
    f << this->id << " ";
    f << this->mass << " ";
    f << std::get<X>(this->pos) << " ";
    f << std::get<Y>(this->pos) << " ";
    f << std::get<Z>(this->pos) << " ";
    f << std::get<X>(this->acc) << " ";
    f << std::get<Y>(this->acc) << " ";
    f << std::get<Z>(this->acc) << " ";
    f << std::get<X>(this->vel) << " ";
    f << std::get<Y>(this->vel) << " ";
    f << std::get<Z>(this->vel) << " ";
    f << std::endl;
}
